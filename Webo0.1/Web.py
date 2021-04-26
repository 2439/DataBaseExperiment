from flask import Flask, render_template, redirect, url_for
from flask import request, session
import datetime
import mysql.connector
import os
from decorators import login_required

conn = mysql.connector.connect(user="root", password="mysql", database="web")    # 数据库连接
cursor = conn.cursor()

app = Flask(__name__)     # 引入Flask
app.config['SECRET_KEY'] = os.urandom(24)


# 跳转至login.html，请求方式GET
@app.route('/', methods=['GET'])
def show():
    return render_template('login.html')


# 登录
@app.route('/login', methods=['POST', 'GET'])
def login():
    if request.method == 'GET':
        return render_template('login.html')
    else:
        username = request.form['username']     # 界面传值
        password = request.form['password']     # 界面传值
        if len(username) == 0 | len(password) == 0:
            return render_template('login.html')

        pw = sql_first_one("user_password", "userinfo", "user_name='"+username+"'")
        if pw:
            if password == pw:
                session['user_name'] = username
                return redirect(url_for('index', choose='all'))
            return render_template('login.html', text="密码错误")
        return render_template('login.html', text="用户名不存在")


# 注册
@app.route('/regist', methods=['POST', 'GET'])
def regist():
    if request.method == 'GET':
        return render_template('regist.html')
    else:
        user = request.form.get('username')
        pw = request.form.get('password1')

        if len(user) == 0 | len(pw) == 0:
            return render_template('login.html')

        if pw != request.form.get('password2'):
            return render_template('regist.html', text="密码不一致", username=user)

        if sql_first_one("user_name", "userinfo", "user_name='"+user+"'"):
            return render_template('regist.html', text="用户名已存在")
        else:
            # 把注册信息加入test表
            sql_commit("insert into userinfo(user_name, user_password, user_state, user_register)\
                           values ('%s', '%s', %s, '%s')"
                       % (user, pw, 1, str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")),))
            return render_template('login.html', text="注册成功，请重新登陆")


# 退出登录
@app.route('/logout', methods=['POST', 'GET'])
def logout():
    session.pop('user_name')
    return render_template('login.html')


# 首页
# 1111111111111111111111111111111111
@app.route('/index/<choose>', methods=['POST', 'GET'])
@login_required
def index(choose):
    if len(choose) == 0:
        choose = 'all'

    if choose == 'all':
        lists = sql_article()
        return render_template('index.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               lists=lists,
                               praise=sql_praise_with_message_id(lists, 8),
                               choose=choose
                               )
    if choose == 'focus':
        focus_id = sql_first_one("user_id", "userinfo", "user_name='" + session.get('user_name') + "'")
        lists = sql_select("select article_detail.* "
                           "from article_detail "
                           "where article_detail.user_name in "
                           "(select userinfo.user_name "
                           "from userinfo, focus "
                           "where focus_id=%s "
                           "and userinfo.user_id=focus.focused_id) "
                           % (focus_id,))
        return render_template('index.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               lists=lists,
                               praise=sql_praise_with_message_id(lists, 8),
                               choose=choose
                               )

    if choose == 'find_message':
        if request.method == 'GET':
            lists = sql_article()
            return render_template('index.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   lists=lists,
                                   praise=sql_praise_with_message_id(lists, 8),
                                   choose=choose
                                   )
        else:
            find_message = request.form['find_article']
            lists = sql_select("select * "
                               "from article_detail "
                               "where article_title like '%%%s%%' "
                               "or article_text like '%%%s%%' "
                               "order by message_time desc "
                               % (find_message, find_message, ))
            return render_template('index.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   lists=lists,
                                   praise=sql_praise_with_message_id(lists, 8),
                                   choose=choose
                                   )

    lists = sql_select("select * "
                       "from article_detail "
                       "where type_name='%s' "
                       "order by message_time desc "
                       % (choose, ))
    return render_template('index.html',
                           user=session.get('user_name'),
                           types=sql_article_type(),
                           lists=lists,
                           praise=sql_praise_with_message_id(lists, 8),
                           choose=choose
                           )


# 发布文章
@app.route('/make_article', methods=['POST', 'GET'])
@login_required
def make_article():
    if request.method == 'GET':
        return render_template('makeArticle.html', user=session.get('user_name'), types=sql_article_type())
    else:
        title = request.form['title']
        content = request.form['content']
        type_name = request.form['article_type']
        if len(content) == 0:
            return render_template('makeArticle.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   text="内容为空")
        type_id = sql_first_one("type_id", "article_type", "type_name='"+type_name+"'")
        username = session.get('user_name')
        user_id = sql_first_one("user_id", "userinfo", "user_name='"+username+"'")
        time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        sql_commit("insert into out_message(user_id, message_time, message_newtime, message_flag) "
                   "values (%d, '%s', '%s', %d) "
                   % (user_id, time, time, 1, ))
        message_id = sql_identity_id()
        sql_commit("insert into article(message_id, type_id, article_title, article_text) "
                   "values(%d, %d, '%s', '%s') "
                   % (message_id, type_id, title, content))
    return index('all')


# 用户详细信息
# 1111111111111111111111111111111111
@app.route('/user_detail/<user_name>/<choose>', methods=['POST', 'GET'])
@login_required
def user_detail(user_name, choose):
    if choose is None:
        choose = 'detail'

    if choose == 'detail':  # 显示用户信息
        detail = sql_select("select * "
                            "from user_detail, userinfo "
                            "where userinfo.user_name = '%s' "
                            "and userinfo.user_id = user_detail.user_id "
                            % (user_name, ))
        focused_id = sql_first_one("user_id", "userinfo", "user_name='"+user_name+"'")
        if request.method == 'POST':
            group_id = request.form['focus_group']
            time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
            focus_id = sql_first_one("user_id", "userinfo", "user_name='" + session.get('user_name') + "'")
            sql_commit("insert focus(focus_id, focused_id, group_id, focus_time) "
                       "values(%s, %s, %s, '%s') "
                       % (focus_id, focused_id, group_id, time,))

        focus = sql_focus(focused_id)
        focus_groups = sql_group_list(session.get('user_name'))

        if detail:
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose='detail',
                                   user_sex=detail[0][2],
                                   user_birthday=detail[0][3] if detail[0][3] else "无",
                                   user_email=detail[0][4] if detail[0][4] else "无",
                                   user_address=detail[0][5] if detail[0][5] else "无",
                                   user_text=detail[0][6] if detail[0][6] else "无",
                                   focus=focus,
                                   focus_groups=focus_groups)
        else:
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose='detail',
                                   user_sex='男',
                                   user_birthday='无',
                                   user_email='无',
                                   user_address='无',
                                   user_text='无',
                                   focus=focus,
                                   focus_groups=focus_groups
                                   )

    if choose == 'remake':  # 修改用户信息
        if request.method == 'GET':
            detail = sql_select("select * "
                                "from user_detail, userinfo "
                                "where userinfo.user_name = '%s' "
                                "and userinfo.user_id = user_detail.user_id "
                                % (user_name, ))

            if detail:
                return render_template('user_detail.html',
                                       user=session.get('user_name'),
                                       types=sql_article_type(),
                                       host=user_name,
                                       choose='remake',
                                       user_sex=detail[0][2],
                                       user_birthday=detail[0][3] if detail[0][3] else "",
                                       user_email=detail[0][4] if detail[0][4] else "",
                                       user_address=detail[0][5] if detail[0][5] else "",
                                       user_text=detail[0][6] if detail[0][6] else "")
            else:
                return render_template('user_detail.html',
                                       user=session.get('user_name'),
                                       types=sql_article_type(),
                                       host=user_name,
                                       choose='remake',
                                       user_sex='男')
        else:
            user_id = sql_first_one("user_id", "userinfo", "user_name='" + user_name + "'")
            user_sex = request.form['user_sex']
            user_birthday = request.form['user_birthday']
            user_email = request.form['user_email']
            user_address = request.form['user_address']
            user_text = request.form['user_text']
            sql_commit_userdetail(user_id, user_sex, user_birthday, user_email, user_address, user_text)
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose='detail',
                                   user_sex=user_sex,
                                   user_birthday=user_birthday,
                                   user_email=user_email,
                                   user_address=user_address,
                                   user_text=user_text)

    if choose == 'focus':
        if request.method == 'GET':
            focus_groups = sql_group_list(user_name)
            focus_lists = {}
            for focus_group in focus_groups:
                focus_list = sql_select("select userinfo.user_name "
                                        "from userinfo, focus "
                                        "where userinfo.user_id = focus.focused_id "
                                        "and focus.group_id = %s "
                                        % (focus_group[0], ))
                focus_lists[focus_group[2]] = focus_list
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose=choose,
                                   focus_groups=focus_groups,
                                   focus_lists=focus_lists)
        else:
            group_name = request.form['group_name']
            user_id = sql_first_one("user_id", "userinfo", "user_name='" + user_name + "'")
            sql_commit("insert group_user(user_id, group_name) "
                       "value(%d, '%s') "
                       % (user_id, group_name, ))
            focus_groups = sql_group_list(user_name)
            focus_lists = {}
            for focus_group in focus_groups:
                focus_list = sql_select("select userinfo.user_name "
                                        "from userinfo, focus "
                                        "where userinfo.user_id = focus.focused_id "
                                        "and focus.group_id = %s "
                                        % (focus_group[0], ))
                focus_lists[focus_group[2]] = focus_list
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose=choose,
                                   focus_groups=focus_groups,
                                   focus_lists=focus_lists)

    lists = []
    if choose == 'web':
        lists = sql_select("select * "
                           "from article_detail "
                           "where user_name='%s' "
                           "order by message_time desc "
                           % (user_name, ))
    if choose == 'praise':
        lists = sql_select("select * "
                           "from article_detail "
                           "where message_id in "
                           "(select message_id "
                           "from praise, userinfo "
                           "where userinfo.user_name = '%s' "
                           "and praise.user_id = userinfo.user_id ) "
                           % (user_name, ))
    if choose == 'comment':
        lists = sql_select("select * "
                           "from article_detail "
                           "where article_id in "
                           "(select comment_the.article_id "
                           "from comment_the, out_message, userinfo "
                           "where userinfo.user_name = '%s' "
                           "and comment_the.message_id = out_message.message_id "
                           "and out_message.user_id = userinfo.user_id) "
                           % (user_name, ))
    if choose == 'reply':
        lists = sql_article()
    return render_template('user_detail.html',
                           user=session.get('user_name'),
                           lists=lists,
                           praise=sql_praise_with_message_id(lists, 8),
                           choose=choose,
                           types=sql_article_type(),
                           host=user_name)


# 微博详细内容
@app.route('/article_detail/<article_id>', methods=['POST', 'GET'])
@login_required
def article_detail(article_id):
    user = session.get('user_name')
    list_a = sql_select("select * "
                        "from article_detail "
                        "where article_id=" + article_id)
    comments = sql_select("select * "
                          "from comment_detail "
                          "where article_id=" + article_id)
    focused_id = sql_first_one("user_id",
                               "userinfo",
                               "user_name='"+list_a[0][4]+"'")
    focus = sql_focus(focused_id)
    focus_group_list = sql_group_list(user)

    if request.method == 'POST':
        group_id = request.form['focus_group']
        time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        focus_id = sql_first_one("user_id", "userinfo", "user_name='"+session.get('user_name')+"'")
        sql_commit("insert focus(focus_id, focused_id, group_id, focus_time) "
                   "values(%s, %s, %s, '%s') "
                   % (focus_id, focused_id, group_id, time, ))
        focus = sql_focus(focused_id)

    return render_template('detail.html',
                           user=user,
                           types=sql_article_type(),
                           list=list_a[0],
                           praise=sql_praise_with_message_id(list_a, 8),
                           praise_comment=sql_praise_with_message_id(comments, 1),
                           focus=focus,
                           focus_groups=focus_group_list,
                           comments=comments)


# 主页点赞
@app.route('/praise_index/<message_id>/<choose>', methods=['POST', 'GET'])
@login_required
def praise_index(message_id, choose):
    praise_action(message_id)
    return index(choose)


# 文章页面内点赞
@app.route('/praise_detail/<message_id>/<article_id>', methods=['POST', 'GET'])
@login_required
def praise_detail(message_id, article_id):
    praise_action(message_id)
    return article_detail(article_id)


# 评论页面内点赞
@app.route('/praise_comment/<message_id>/<choose>/<article_id>', methods=['POST', 'GET'])
@login_required
def praise_comment(message_id, choose, article_id):
    praise_action(message_id)
    return comment_action(article_id, choose)


# 点赞
def praise_action(message_id):
    user_id = sql_first_one("user_id", "userinfo", "user_name='"+session.get('user_name')+"'")
    sql_commit("insert praise(message_id, user_id) "
               "values(%s, %s) "
               % (message_id, user_id, ))
    return


# 评论
@app.route('/comment_action/<article_id>/<choose>', methods=['POST', 'GET'])
@login_required
def comment_action(article_id, choose):
    if request.method == 'GET':
        list_a = sql_select("select * "
                            "from article_detail "
                            "where article_id=" + article_id)
        return render_template('makeComment.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               list=list_a[0],
                               praise=sql_praise_with_message_id(list_a, 8),
                               choose=choose,
                               action="评论")
    else:
        comment_text = request.form['content']
        username = session.get('user_name')
        user_id = sql_first_one("user_id", "userinfo", "user_name='" + username + "'")
        time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        sql_commit("insert into out_message(user_id, message_time, message_newtime, message_flag) "
                   "values (%d, '%s', '%s', %d) "
                   % (user_id, time, time, 1,))
        message_id = sql_identity_id()
        sql_commit("insert into comment_the(message_id, article_id, comment_text) "
                   "values(%s, %s, '%s') "
                   % (message_id, article_id, comment_text, ))
        if choose == "article_detail":
            user = session.get('user_name')
            list_a = sql_select("select * "
                                "from article_detail "
                                "where article_id=" + article_id)
            comments = sql_select("select * "
                                  "from comment_detail "
                                  "where article_id=" + article_id)
            focused_id = sql_first_one("user_id",
                                       "userinfo",
                                       "user_name='" + list_a[0][4] + "'")
            focus = sql_focus(focused_id)
            focus_group_list = sql_group_list(user)
            return render_template('detail.html',
                                   user=user,
                                   types=sql_article_type(),
                                   list=list_a[0],
                                   praise=sql_praise_with_message_id(list_a, 8),
                                   praise_comment=sql_praise_with_message_id(comments, 1),
                                   focus=focus,
                                   focus_groups=focus_group_list,
                                   comments=comments)
        else:
            return index(choose)


# 11111111111111111
# 回复
@app.route('/reply_action/<comment_id>/<article_id>', methods=['POST', 'GET'])
@login_required
def reply_action(comment_id, article_id):
    # if request.method == 'GET':
    #     list_a = sql_select("select * "
    #                         "from article_detail "
    #                         "where article_id=" + article_id)
    #     return render_template('makeComment.html',
    #                            user=session.get('user_name'),
    #                            list=list_a[0],
    #                            praise=sql_praise_with_lists(list_a, 8),
    #                            choose=choose,
    #                            action="评论")
    # else:
    #     comment_text = request.form['content']
    #     username = session.get('user_name')
    #     user_id = sql_first_one("user_id", "userinfo", "user_name='" + username + "'")
    #     time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    #     sql_commit("insert into out_message(user_id, message_time, message_newtime, message_flag) "
    #                "values (%d, '%s', '%s', %d) "
    #                % (user_id, time, time, 1,))
    #     message_id = sql_identity_id()
    #     sql_commit("insert into comment_the(message_id, article_id, comment_text) "
    #                "values(%s, %s, '%s') "
    #                % (message_id, article_id, comment_text, ))
    return


def article_type(message):      # 文章类型初始化定义
    sql_commit("insert into article_type(type_name) "
               "values ('%s') "
               "ON DUPLICATE KEY UPDATE type_name='%s'"
               % (message, message, ))
    return


def article_init():     # 文章类型初始化定义
    article_type("新闻")
    article_type("明星")
    article_type("生活")
    article_type("其他")
    return


# select获取数据
def sql_select(sql):
    cursor.execute(sql)
    return cursor.fetchall()


# 上传数据
def sql_commit(sql):
    cursor.execute(sql)
    conn.commit()
    return


# 获得最新插入的id
def sql_identity_id():
    return sql_select("select @@identity")[0][0]


# 文章类型
def sql_article_type():
    return sql_select("select type_name from article_type")


# 文章
def sql_article():
    return sql_select("select * from article_detail order by message_time desc")


# 是否点赞
def sql_praise_with_message_id(lists, location):
    praise_dict = {}
    for list_a in lists:
        user_name = session.get('user_name')
        praise_do = sql_select("select * "
                               "from praise,userinfo "
                               "where userinfo.user_name = '%s' "
                               "and praise.message_id = %d "
                               "and praise.user_id = userinfo.user_id "
                               % (user_name, list_a[location], ))
        if praise_do:
            praise_dict[list_a[location]] = 1
        else:
            praise_dict[list_a[location]] = 0
    return praise_dict


# 是否关注
def sql_focus(focused_id):
    focus_id = sql_first_one("user_id", "userinfo", "user_name='" + session.get('user_name') + "'")
    result = sql_select("select * "
                        "from focus "
                        "where focus_id=%d "
                        "and focused_id=%d "
                        % (focus_id, focused_id,))
    if result:
        return result[0]
    else:
        return False


# 查找现有分组，无则创建默认分组
def sql_group_list(user_name):
    user_id = sql_first_one("user_id", "userinfo", "user_name='"+user_name+"'")
    group_list = sql_select("select * "
                            "from group_user "
                            "where user_id=%d "
                            % (user_id, ))
    if group_list:
        return group_list
    else:
        sql_commit("insert group_user(user_id, group_name) "
                   "values(%d, '默认') "
                   % (user_id, ))
        return sql_select("select * "
                          "from group_user "
                          "where user_id=%d "
                          % (user_id,))


# 获得第一个的第一个值
def sql_first_one(id_name, table, equal):
    cursor.execute("select %s "
                   "from %s "
                   "where %s "
                   % (id_name, table, equal))
    re = cursor.fetchall()
    if any(re):
        return re[0][0]
    return


# 上传用户信息
def sql_commit_userdetail(user_id, user_sex, user_birthday, user_email, user_address, user_text):
    if user_birthday:
        sql_commit_a_userdetail(user_id, user_sex, "user_birthday", user_birthday)
    if user_email:
        sql_commit_a_userdetail(user_id, user_sex, "user_email", user_email)
    if user_address:
        sql_commit_a_userdetail(user_id, user_sex, "user_address", user_address)
    if user_text:
        sql_commit_a_userdetail(user_id, user_sex, "user_text", user_text)


# 上传用户单个信息
def sql_commit_a_userdetail(user_id, user_sex, text, value):
    sql_commit(
        "insert into user_detail(user_id, user_sex, %s) "
        "values(%d, '%s', '%s') "
        "ON DUPLICATE KEY UPDATE "
        "user_sex='%s',%s='%s' "
        % (text, user_id, user_sex, value, user_sex, text, value,))


if __name__ == '__main__':
    article_init()
    app.run()
