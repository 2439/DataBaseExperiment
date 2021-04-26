from flask import Flask
import mysql.connector
import os

conn = mysql.connector.connect(user="root", password="mysql", database="web")    # 数据库连接
cursor = conn.cursor()

app = Flask(__name__)     # 引入Flask
app.config['SECRET_KEY'] = os.urandom(24)