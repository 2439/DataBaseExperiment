/*==============================================================*/
/* DBMS name:      MySQL 5.0                                    */
/* Created on:     2021/4/17 15:40:38                           */
/*==============================================================*/


drop  
table  if   exists   article_detail;


/*==============================================================*/
/* Table: article                                               */
/*==============================================================*/
create table article
(
   article_id           int not null auto_increment,
   message_id           int not null,
   type_id              int not null,
   repeat_message_id    int,
   article_title        char(50),
   article_text         text not null,
   primary key (article_id)
);

/*==============================================================*/
/* Index: index_article                                         */
/*==============================================================*/
create index index_article on article
(
   article_title
);

/*==============================================================*/
/* Table: article_type                                          */
/*==============================================================*/
create table article_type
(
   type_id              int not null auto_increment,
   type_name            char(4) not null unique,
   primary key (type_id)
);

/*==============================================================*/
/* Table: at_the                                                */
/*==============================================================*/
create table at_the
(
   user_id              int not null,
   message_id           int not null,
   primary key (user_id, message_id)
);

/*==============================================================*/
/* Table: comment_the                                           */
/*==============================================================*/
create table comment_the
(
   comment_id           int not null auto_increment,
   message_id           int not null,
   article_id           int not null,
   comment_text         text not null,
   primary key (comment_id)
);

/*==============================================================*/
/* Table: focus                                                 */
/*==============================================================*/
create table focus
(
   focus_id             int not null,
   focused_id           int not null,
   group_id             int not null,
   focus_time           datetime not null,
   primary key (focus_id, focused_id)
);

/*==============================================================*/
/* Table: group_user                                            */
/*==============================================================*/
create table group_user
(
   group_id             int not null auto_increment,
   user_id              int not null,
   group_name           char(20) not null,
   primary key (group_id)
);

/*==============================================================*/
/* Table: out_message                                           */
/*==============================================================*/
create table out_message
(
   message_id           int not null auto_increment,
   user_id              int not null,
   message_time         datetime not null,
   message_newtime      datetime not null,
   message_flag         bool not null,
   primary key (message_id)
);

/*==============================================================*/
/* Table: praise                                                */
/*==============================================================*/
create table praise
(
   message_id           int not null,
   user_id              int not null,
   primary key (message_id, user_id)
);

/*==============================================================*/
/* Table: reply                                                 */
/*==============================================================*/
create table reply
(
   reply_id             int not null auto_increment,
   message_id           int not null,
   comment_id           int not null,
   reply_text           text not null,
   primary key (reply_id)
);

/*==============================================================*/
/* Table: userInfo                                              */
/*==============================================================*/
create table userInfo
(
   user_id              int auto_increment not null unique,
   user_name            char(10) not null unique,
   user_password        varchar(20) not null,
   user_state           bool not null,
   user_register        datetime not null,
   primary key (user_id)
);

/*==============================================================*/
/* Table: user_detail                                           */
/*==============================================================*/
create table user_detail
(
   user_detail_id       int not null auto_increment,
   user_id              int not null unique,
   user_sex             enum('男', '女') NOT NULL default '男',
   user_birthday		date,
   user_email           char(20) check (user_email like '%@%.com'),
   user_address         char(30),
   user_text            text,
   primary key (user_detail_id)
);

/*==============================================================*/
/* View: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!comment and praise */
/*==============================================================*/
create VIEW message_praise
as
select 
	praise.message_id as message_id, 
	count(praise.user_id) as praise_num
 from praise
 group by praise.message_id;
 
/*==============================================================*/
/* View: article_detail                                         */
/*==============================================================*/
create  VIEW      article_detail
  as
select 
	b.article_id,
    b.type_name,
    b.article_title,
    b.article_text,
    b.user_name,
    b.message_time,
    b.praise_num,
    c.comment_num
from
	(select a.article_id,
		a.type_name,
		a.article_title,
		a.article_text,
		a.user_name,
		a.message_time,
		message_praise.praise_num
	from
		(select 
			article.article_id,
			article.message_id,
			article_type.type_name,
			article.article_title,
			article.article_text,
			userInfo.user_name,
			out_message.message_time
		from article,
			article_type,
			out_message,
			userInfo
		where
			userInfo.user_id = out_message.user_id
			and article_type.type_id = article.type_id
			and out_message.message_id = article.message_id) as a
	left outer join
		message_praise
	on  message_praise.message_id = a.message_id) as b
left outer join
	 (select
	comment_the.article_id as article_id,
	count(comment_the.comment_id) as comment_num
	from comment_the
	group by comment_the.article_id) as c
on c.article_id = b.article_id;
 
alter table article add constraint FK_article_type foreign key (type_id)
      references article_type (type_id) on delete restrict on update restrict;

alter table article add constraint FK_message_article foreign key (message_id)
      references out_message (message_id) on delete restrict on update restrict;

alter table article add constraint FK_repeat foreign key (repeat_message_id)
      references out_message (message_id) on delete restrict on update restrict;

alter table at_the add constraint FK_at foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table at_the add constraint FK_at2 foreign key (message_id)
      references out_message (message_id) on delete restrict on update restrict;

alter table comment_the add constraint FK_article_comment foreign key (article_id)
      references article (article_id) on delete restrict on update restrict;

alter table comment_the add constraint FK_message_comment foreign key (message_id)
      references out_message (message_id) on delete restrict on update restrict;

alter table focus add constraint FK_focus foreign key (focus_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table focus add constraint FK_focus_divide foreign key (group_id)
      references group_user (group_id) on delete restrict on update restrict;

alter table focus add constraint FK_focused foreign key (focused_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table group_user add constraint FK_user_focus foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table out_message add constraint FK_publish foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table praise add constraint FK_prise foreign key (message_id)
      references out_message (message_id) on delete restrict on update restrict;

alter table praise add constraint FK_prise2 foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

alter table reply add constraint FK_comment_reply foreign key (comment_id)
      references comment_the (comment_id) on delete restrict on update restrict;

alter table reply add constraint FK_message_reply foreign key (message_id)
      references out_message (message_id) on delete restrict on update restrict;

alter table user_detail add constraint FK_user_detailInfo foreign key (user_id)
      references userInfo (user_id) on delete restrict on update restrict;

