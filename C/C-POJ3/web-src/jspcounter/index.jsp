<%@ page import="beans.WebCounter" %>

<%@ page contentType="text/html;charset=gb2312"%>    
<HTML>    
	<HEAD>    
		<meta http-equiv="Content-Type" content="text/html; charset=gb2312">    
		<META NAME="GENERATOR" CONTENT="Oracle JDeveloper">    
		<TITLE>��������ʾ����</TITLE>    
	</HEAD>    
	<BODY>    
		<!--����������bean(counter)-->    
		<jsp:useBean id="counter" class="beans.WebCounter" scope="request">    
		</jsp:useBean>    
		<%    
		//����counter�����ReadFile��������ȡ�ļ�jsqcount.txt�еļ���    
		
		//WebCounter counter = new WebCounter();

		String cont=counter.ReadFile("./jsqcount.txt");    
		//����counter�����ReadFile����������������һ��д�뵽�ļ�jsqcount.txt��    
		counter.WriteFile("./jsqcount.txt",cont);%>    

		���ǵ�<font color="red"><%=cont%></font>λ������    
	</BODY>
</HTML>

