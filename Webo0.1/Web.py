from flask import render_template, redirect, url_for
from flask import request, session
import datetime
from decorators import login_required
from sql_connect import app
from sql_work import sql_first_one, sql_commit, sql_select, \
    sql_identity_id, sql_select_focus, sql_select_user_detail, \
    sql_select_user_id, sql_commit_userdetail, sql_group_list, \
    sql_select_focus_group_user_name, sql_select_web_with_message, \
    sql_select_user_web, sql_select_user_comment_web, \
    sql_select_user_praise_web, sql_select_web_with_article_id, \
    sql_select_comments_with_article_id, sql_focus, sql_praise, \
    sql_article, sql_article_type, sql_select_user_reply_web, \
    sql_select_replys_with_comment_id
from article_type_init import article_init


# 跳转至login.html，请求方式GET
@app.route('/', methods=['GET'])
def show():
    return redirect(url_for('login'))


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
            return redirect(url_for('login'))

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
    return redirect(url_for('login'))


# 首页
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
                               praise=if_praise_with_message_id(lists, 8),
                               choose=choose
                               )
    if choose == 'focus':
        focus_id = sql_first_one("user_id", "userinfo", "user_name='" + session.get('user_name') + "'")
        lists = sql_select_focus(focus_id)
        return render_template('index.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               lists=lists,
                               praise=if_praise_with_message_id(lists, 8),
                               choose=choose
                               )

    if choose == 'find_message':
        if request.method == 'GET':
            lists = sql_article()
            return render_template('index.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   lists=lists,
                                   praise=if_praise_with_message_id(lists, 8),
                                   choose=choose
                                   )
        else:
            find_message = request.form['find_article']
            lists = sql_select_web_with_message(find_message)
            return render_template('index.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   lists=lists,
                                   praise=if_praise_with_message_id(lists, 8),
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
                           praise=if_praise_with_message_id(lists, 8),
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
    return redirect(url_for('index', choose='all'))


# 用户详细信息
@app.route('/user_detail/<user_name>/<choose>', methods=['POST', 'GET'])
@login_required
def user_detail(user_name, choose):
    if choose is None:
        choose = 'detail'

    if choose == 'detail':  # 显示用户信息
        detail = sql_select_user_detail(user_name)
        focused_id = sql_select_user_id(user_name)
        focus_id = sql_select_user_id(session.get('user_name'))
        if request.method == 'POST':
            group_id = request.form['focus_group']
            time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
            sql_commit("insert focus(focus_id, focused_id, group_id, focus_time) "
                       "values(%s, %s, %s, '%s') "
                       % (focus_id, focused_id, group_id, time,))

        focus = if_focus(focused_id, focus_id)
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
            if user_name != session.get('user_name'):
                redirect(url_for('user_detail', user_name=user_name, choose='detail'))

            detail = sql_select_user_detail(user_name)
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
            focus_lists = focus_user_group_dict(focus_groups)
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose=choose,
                                   focus_groups=focus_groups,
                                   focus_lists=focus_lists)
        else:
            group_name = request.form['group_name']
            user_id = sql_select_user_id(user_name)
            sql_commit("insert group_user(user_id, group_name) "
                       "value(%d, '%s') "
                       % (user_id, group_name, ))

            focus_groups = sql_group_list(user_name)
            focus_lists = focus_user_group_dict(focus_groups)
            return render_template('user_detail.html',
                                   user=session.get('user_name'),
                                   types=sql_article_type(),
                                   host=user_name,
                                   choose=choose,
                                   focus_groups=focus_groups,
                                   focus_lists=focus_lists)

    lists = []
    if choose == 'web':
        lists = sql_select_user_web(user_name)
    if choose == 'praise':
        lists = sql_select_user_praise_web(user_name)
    if choose == 'comment':
        lists = sql_select_user_comment_web(user_name)
    if choose == 'reply':
        lists = sql_select_user_reply_web(user_name)
    return render_template('user_detail.html',
                           user=session.get('user_name'),
                           lists=lists,
                           praise=if_praise_with_message_id(lists, 8),
                           choose=choose,
                           types=sql_article_type(),
                           host=user_name)


# 微博详细内容
@app.route('/article_detail/<article_id>', methods=['POST', 'GET'])
@login_required
def article_detail(article_id):
    user = session.get('user_name')
    focus_id = sql_select_user_id(user)
    list_a = sql_select_web_with_article_id(article_id)
    comments = sql_select_comments_with_article_id(article_id)
    reply_lists, praise_replys = replys_comment_dic(comments)
    focused_id = sql_select_user_id(list_a[0][4])
    focus_group_list = sql_group_list(user)

    if request.method == 'POST':
        group_id = request.form['focus_group']
        time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        sql_commit("insert focus(focus_id, focused_id, group_id, focus_time) "
                   "values(%s, %s, %s, '%s') "
                   % (focus_id, focused_id, group_id, time, ))

    focus = if_focus(focused_id, focus_id)
    return render_template('detail.html',
                           user=user,
                           types=sql_article_type(),
                           list=list_a[0],
                           praise=if_praise_with_message_id(list_a, 8),
                           praise_comment=if_praise_with_message_id(comments, 1),
                           focus=focus,
                           focus_groups=focus_group_list,
                           comments=comments,
                           reply_lists=reply_lists,
                           praise_replys=praise_replys)


# 主页点赞
@app.route('/praise_index/<message_id>/<choose>', methods=['POST', 'GET'])
@login_required
def praise_index(message_id, choose):
    praise_action(message_id)
    return redirect(url_for('index', choose=choose))


# 文章页面内点赞
@app.route('/praise_detail/<message_id>/<article_id>', methods=['POST', 'GET'])
@login_required
def praise_detail(message_id, article_id):
    praise_action(message_id)
    return redirect(url_for('article_detail', article_id=article_id))


# 评论页面内点赞
@app.route('/praise_comment/<message_id>/<choose>/<article_id>', methods=['POST', 'GET'])
@login_required
def praise_comment(message_id, choose, article_id):
    praise_action(message_id)
    return redirect(url_for('comment_action', article_id=article_id, choose=choose))


# 回复页面内点赞
@app.route('/praise_comment/<message_id>/<article_id>/<comment_id>', methods=['POST', 'GET'])
@login_required
def praise_reply(message_id, comment_id, article_id):
    praise_action(message_id)
    return redirect(url_for('reply_action', comment_id=comment_id, article_id=article_id))


# 评论
@app.route('/comment_action/<article_id>/<choose>', methods=['POST', 'GET'])
@login_required
def comment_action(article_id, choose):
    user = session.get('user_name')
    if request.method == 'GET':
        list_a = sql_select_web_with_article_id(article_id)
        return render_template('makeComment.html',
                               user=user,
                               types=sql_article_type(),
                               list=list_a[0],
                               praise=if_praise_with_message_id(list_a, 8),
                               choose=choose)
    else:
        comment_text = request.form['content']
        user_id = sql_select_user_id(user)
        time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        sql_commit("insert into out_message(user_id, message_time, message_newtime, message_flag) "
                   "values (%d, '%s', '%s', %d) "
                   % (user_id, time, time, 1,))
        message_id = sql_identity_id()
        sql_commit("insert into comment_the(message_id, article_id, comment_text) "
                   "values(%s, %s, '%s') "
                   % (message_id, article_id, comment_text, ))
        if choose == "article_detail":
            return redirect(url_for('article_detail', article_id=article_id))
        else:
            return index(choose)


# 回复
@app.route('/reply_action/<comment_id>/<article_id>', methods=['POST', 'GET'])
@login_required
def reply_action(comment_id, article_id):
    user = session.get('user_name')
    if request.method == 'GET':
        list_a = sql_select_web_with_article_id(article_id)
        return render_template('makeReply.html',
                               user=user,
                               types=sql_article_type(),
                               list=list_a[0],
                               praise=if_praise_with_message_id(list_a, 8),
                               comment_id=comment_id)
    else:
        reply_text = request.form['content']
        if len(reply_text) == 0:
            return redirect(url_for('reply_action', comment_id=comment_id, article_id=article_id))

        user_id = sql_select_user_id(user)
        time = str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
        sql_commit("insert into out_message(user_id, message_time, message_newtime, message_flag) "
                   "values (%d, '%s', '%s', %d) "
                   % (user_id, time, time, 1,))
        message_id = sql_identity_id()
        sql_commit("insert into reply(message_id, comment_id, reply_text) "
                   "values(%d, %s, '%s') "
                   % (message_id, comment_id, reply_text, ))
        return redirect(url_for('article_detail', article_id=article_id))


# 点赞
def praise_action(message_id):
    user_id = sql_select_user_id(session.get('user_name'))
    sql_commit("insert praise(message_id, user_id) "
               "values(%s, %s) "
               % (message_id, user_id, ))
    return


# 是否关注
def if_focus(focused_id, focus_id):
    result = sql_focus(focused_id, focus_id)
    if result:
        return result[0]
    else:
        return False


# 是否点赞
def if_praise_with_message_id(lists, location):
    praise_dict = {}
    for list_a in lists:
        praise_do = sql_praise(sql_select_user_id(session.get('user_name')), list_a[location])
        if praise_do:
            praise_dict[list_a[location]] = 1
        else:
            praise_dict[list_a[location]] = 0
    return praise_dict


# 关注用户分组名单
def focus_user_group_dict(focus_groups):
    focus_lists = {}
    for focus_group in focus_groups:
        focus_lists[focus_group[0]] = sql_select_focus_group_user_name(focus_group[0])
    return focus_lists


# 评论回复字典
def replys_comment_dic(comments):
    reply_lists = {}
    praise_replys = {}
    for comment in comments:
        reply_lists[comment[0]] = sql_select_replys_with_comment_id(comment[0])
        praise_replys.update(if_praise_with_message_id(reply_lists[comment[0]], 1))
    return reply_lists, praise_replys


if __name__ == '__main__':
    article_init()
    app.run()
