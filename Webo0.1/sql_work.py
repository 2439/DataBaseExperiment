from sql_connect import cursor, conn


# select获取数据
def sql_select(sql):
    cursor.execute(sql)
    return cursor.fetchall()


# 上传数据
def sql_commit(sql):
    cursor.execute(sql)
    conn.commit()
    return


# 查找最新插入的id
def sql_identity_id():
    return sql_select("select @@identity")[0][0]


# 查找第一个的第一个值
def sql_first_one(id_name, table, equal):
    cursor.execute("select %s "
                   "from %s "
                   "where %s "
                   % (id_name, table, equal))
    re = cursor.fetchall()
    if any(re):
        return re[0][0]
    return


# 搜索微博内容
def sql_select_web_with_message(find_message):
    return sql_select("select * "
                      "from article_detail "
                      "where article_title like '%%%s%%' "
                      "or article_text like '%%%s%%' "
                      "order by message_time desc "
                      % (find_message, find_message, ))


# 查找用户详细信息
def sql_select_user_detail(user_name):
    return sql_select("select * "
                      "from user_detail, userinfo "
                      "where userinfo.user_name = '%s' "
                      "and userinfo.user_id = user_detail.user_id "
                      % (user_name, ))


# 查找用户id
def sql_select_user_id(user_name):
    return sql_first_one("user_id", "userinfo", "user_name='"+user_name+"'")


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


# 查找现有分组，无则创建默认分组
def sql_group_list(user_name):
    user_id = sql_first_one("user_id", "userinfo", "user_name='" + user_name + "'")
    group_list = sql_select("select * "
                            "from group_user "
                            "where user_id=%d "
                            % (user_id,))
    if group_list:
        return group_list
    else:
        sql_commit("insert group_user(user_id, group_name) "
                   "values(%d, '默认') "
                   % (user_id,))
        return sql_select("select * "
                          "from group_user "
                          "where user_id=%d "
                          % (user_id,))


# 查找关注列表
def sql_select_focus(focus_id):
    return sql_select("select article_detail.* "
                      "from article_detail "
                      "where article_detail.user_name in "
                      "(select userinfo.user_name "
                      "from userinfo, focus "
                      "where focus_id=%s "
                      "and userinfo.user_id=focus.focused_id) "
                      % (focus_id,))


# 查找关注分组内用户名称
def sql_select_focus_group_user_name(group_id):
    return sql_select("select userinfo.user_name "
                      "from userinfo, focus "
                      "where userinfo.user_id = focus.focused_id "
                      "and focus.group_id = %s "
                      % (group_id, ))


# 查找用户已发微博
def sql_select_user_web(user_name):
    return sql_select("select * "
                      "from article_detail "
                      "where user_name='%s' "
                      "order by message_time desc "
                      % (user_name, ))


# 查找用户已点赞微博
def sql_select_user_praise_web(user_name):
    return sql_select("select * "
                      "from article_detail "
                      "where message_id in "
                      "(select message_id "
                      "from praise, userinfo "
                      "where userinfo.user_name = '%s' "
                      "and praise.user_id = userinfo.user_id ) "
                      % (user_name, ))


# 查找用户评论微博
def sql_select_user_comment_web(user_name):
    return sql_select("select * "
                      "from article_detail "
                      "where article_id in "
                      "(select comment_the.article_id "
                      "from comment_the, out_message, userinfo "
                      "where userinfo.user_name = '%s' "
                      "and comment_the.message_id = out_message.message_id "
                      "and out_message.user_id = userinfo.user_id) "
                      % (user_name, ))


# 查找用户回复微博
def sql_select_user_reply_web(user_name):
    return sql_select("select * "
                      "from article_detail "
                      "where article_id in "
                      "(select comment_the.article_id "
                      "from comment_the, reply_detail "
                      "where reply_detail.user_name = '%s' "
                      "and comment_the.comment_id = reply_detail.comment_id) "
                      % (user_name, ))


# 查找微博详细信息
def sql_select_web_with_article_id(article_id):
    list_a = sql_select("select * "
                        "from article_detail "
                        "where article_id=" + article_id)
    return list_a


# 查找文章评论
def sql_select_comments_with_article_id(article_id):
    return sql_select("select * "
                      "from comment_detail "
                      "where article_id=" + article_id)


# 查找回复
def sql_select_replys_with_comment_id(comment_id):
    return sql_select("select * "
                      "from reply_detail "
                      "where comment_id=%d "
                      % (comment_id,))


# 查找关注
def sql_focus(focused_id, focus_id):
    return sql_select("select * "
                      "from focus "
                      "where focus_id=%d "
                      "and focused_id=%d "
                      % (focus_id, focused_id,))


# 查找点赞
def sql_praise(user_id, message_id):
    return sql_select("select * "
                      "from praise "
                      "where user_id=%d "
                      "and message_id=%d "
                      % (user_id, message_id, ))


# 文章类型
def sql_article_type():
    return sql_select("select type_name from article_type")


# 文章
def sql_article():
    return sql_select("select * from article_detail order by message_time desc")




