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

        cursor.execute('select user_name from userinfo')     # 查询test表查询用户名
        usernames = cursor.fetchall()
        for user in usernames:
            if username == user[0]:
                cursor.execute('select user_password from userinfo where user_name=%s', (username,))
                pw = cursor.fetchall()    # 从test表中获取密码
                if password == pw[0][0]:  # 如果页面输入的password匹配test表返回的密码
                    session['user_name'] = username
                    return render_template('main.html', user=username)
                return render_template('login.html', text="密码错误")
        return render_template('login.html', text="用户名不存在")


@app.route('/regist', methods=['POST', 'GET'])   # 注册
def regist():
    if request.method == 'GET':
        return render_template('regist.html')
    else:
        user = request.form.get('username')
        pw = request.form.get('password1')
        if pw != request.form.get('password2'):
            return render_template('regist.html', text="密码不一致", username=user)

        cursor.execute('select user_name '
                       'from userinfo '
                       'where user_name=%s', (user,))
        re = cursor.fetchall()
        if any(re):
            return render_template('regist.html', text="用户名已存在")
        else:
            # 把注册信息加入test表
            cursor.execute("insert into userinfo(user_name, user_password, user_state, user_register)\
                           values ('%s', '%s', %s, '%s')"
                           % (user, pw, 1, str(datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")),))
            conn.commit()
            return render_template('login.html', text="注册成功，请重新登陆")


@app.route('/main', methods=['POST', 'GET'])    # 首页
@login_required
def main():
    return render_template('main.html')


@app.route('/make_article', methods=['POST', 'GET'])     # 发布文章
@login_required
def make_article():
    return render_template('makeArticle.html')


if __name__ == '__main__':
    app.run()
