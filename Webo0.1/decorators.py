# 装饰器函数
from functools import wraps
from flask import request, redirect, url_for, session


# 登陆限制装饰器
def login_required(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        print(session.get("user_name"))
        if session.get('user_name'):
            return func(*args, **kwargs)
        else:
            return redirect(url_for('login'))
    return wrapper