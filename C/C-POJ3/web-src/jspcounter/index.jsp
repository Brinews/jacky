<%@ page import="beans.WebCounter" %>

<%@ page contentType="text/html;charset=gb2312"%>    
<HTML>    
	<HEAD>    
		<meta http-equiv="Content-Type" content="text/html; charset=gb2312">    
		<META NAME="GENERATOR" CONTENT="Oracle JDeveloper">    
		<TITLE>计数器演示程序</TITLE>    
	</HEAD>    
	<BODY>    
		<!--创建并调用bean(counter)-->    
		<jsp:useBean id="counter" class="beans.WebCounter" scope="request">    
		</jsp:useBean>    
		<%    
		//调用counter对象的ReadFile方法来读取文件jsqcount.txt中的计数    
		
		//WebCounter counter = new WebCounter();

		String cont=counter.ReadFile("./jsqcount.txt");    
		//调用counter对象的ReadFile方法来将计数器加一后写入到文件jsqcount.txt中    
		counter.WriteFile("./jsqcount.txt",cont);%>    

		您是第<font color="red"><%=cont%></font>位访问者    
	</BODY>
</HTML>

