1.如何编译
在shell中转到当前目录，利用make工具进行编译
$make
如果没有错误，说明编译成功，相关class文件都放在./class目录下
(当前工程已经编译好,可以直接运行)

2.如何运行
在shell中输入命令$make run
如果能够弹出窗体，说明能够启动程序。
如果是在Windows环境下，直接双击run.bat文件。

3.功能说明
Make PDF 按钮作用：在当前目录下产生一个test.pdf文件，文件内容部分
来自MySql数据库。
Send Mail 按钮作用：把生成的test.pdf文件发送出去，相关目标邮件地址
在./conf/mail.lst中。

4.配置说明
./conf/db.conf为连接mysql时的配置文件，包括：用户名，密码，数据库名字,密码可以为空.
./conf/mail.lst中包含需要发送到的目标邮件地址，每个地址一行

5.java支持中文
编译  javac -encoding gbk
运行 java -Dfile.encoding="GBK"


