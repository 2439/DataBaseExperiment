from flask import Flask, render_template
from flask import request, session
import datetime
import mysql.connector
import os
from decorators import login_required

conn = mysql.connector.connect(user="root", password="mysql", database="web")    # 数据库连接
cursor = conn.cursor()

app = Flask(__name__)     # 引入Flask
app.config['SECRET_KEY'] = os.urandom(24)


@app.route('/', methods=['GET'])     # 跳转至login.html，请求方式GET
def show():
    return render_template('login.html')


@app.route('/login', methods=['POST', 'GET'])   # 登录
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
                return render_template('index.html', user=username, types=sql_article_type(), lists=sql_article())
            return render_template('login.html', text="密码错误")
        return render_template('login.html', text="用户名不存在")


@app.route('/regist', methods=['POST', 'GET'])   # 注册
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


@app.route('/logout', methods=['POST', 'GET'])    # 退出登录
def logout():
    session.pop('user_name')
    return render_template('login.html')


@app.route('/index/<choose>', methods=['POST', 'GET'])    # 首页
@login_required
def index(choose):
    if len(choose) == 0:
        choose = 'all'

    if choose == 'all':
        return render_template('index.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               lists=sql_article()
                               )
    if choose == 'focus':
        return render_template('index.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               lists=sql_article()
                               )
    lists = sql_select("select * "
                       "from article_detail "
                       "where type_name='%s' "
                       % (choose, ))
    return render_template('index.html',
                           user=session.get('user_name'),
                           types=sql_article_type(),
                           lists=lists
                           )


@app.route('/make_article', methods=['POST', 'GET'])     # 发布文章
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
                                   text="内容为空或")
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
    return render_template('index.html', user=username, types=sql_article_type(), lists=sql_article())


@app.route('/user_detail/<user_name>', methods=['POST', 'GET'])
@login_required
def user_detail(user_name):
    print(11111111111)
    choose = request.args.get('choose')

    if choose is None:
        choose = 'detail'

    if choose == 'detail':  # 显示用户信息
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
                                   choose='detail',
                                   user_sex=detail[0][2],
                                   user_birthday=detail[0][3],
                                   user_email=detail[0][4],
                                   user_address=detail[0][5],
                                   user_text=detail[0][6])
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
                                   user_text='无')

    if choose == 'remake':  # 修改用户信息
        print(222222222222222)
        detail = sql_select("select * "
                            "from user_detail, userinfo "
                            "where userinfo.user_name = '%s' "
                            "and userinfo.user_id = user_detail.user_id "
                            % (user_name, ))

        if detail:
            print(33333333333)
            user_sex = detail[2]
            user_birthday = detail[3]
            user_email = detail[4]
            user_address = detail[5]
            user_text = detail[6]
        else:
            print(444444444444)
            user_sex = '男'
            user_birthday = '无'
            user_email = '无'
            user_address = '无'
            user_text = '无'

        return render_template('user_detail.html',
                               user=session.get('user_name'),
                               types=sql_article_type(),
                               host=user_name,
                               choose='remake',
                               detail=detail)

    return render_template('user_detail.html',
                           user=session.get('user_name'),
                           types=sql_article_type(),
                           host=user_name)


@app.route('/article_detail/<article_id>', methods=['POST', 'GET'])     # 微博详细内容
@login_required
def article_detail(article_id):
    print(article_id)
    return render_template('detail.html')


def article_type(message):
    sql_commit("insert into article_type(type_name) "
               "values ('%s') "
               "ON DUPLICATE KEY UPDATE type_name='%s'"
               % (message, message, ))
    return


def article_init():
    article_type("新闻")
    article_type("明星")
    article_type("生活")
    article_type("其他")
    return


def sql_select(sql):
    cursor.execute(sql)
    return cursor.fetchall()


def sql_commit(sql):
    cursor.execute(sql)
    conn.commit()
    return


def sql_identity_id():
    return sql_select("select @@identity")[0][0]


def sql_article_type():
    return sql_select("select type_name from article_type")


def sql_article():
    return sql_select("select * from article_detail order by message_time desc")


def sql_first_one(id_name, table, equal):
    cursor.execute("select %s "
                   "from %s "
                   "where %s "
                   % (id_name, table, equal))
    re = cursor.fetchall()
    if any(re):
        return re[0][0]
    return


if __name__ == '__main__':
    article_init()
    app.run()
