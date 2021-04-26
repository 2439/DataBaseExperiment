from sql_work import sql_commit, sql_select


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
