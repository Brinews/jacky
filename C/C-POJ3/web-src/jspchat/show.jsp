<html>
	<body>
		<font size="14" color="red"><%=session.getValue("uid") %>, welcome to chat room!</font>
		<hr>
		<%
		int i,k;

		String in=(String)application.getAttribute("i");
		String  show=""; 

		if(in != null) {
			i=Integer.parseInt(in)+1; 
		}
		else {
			i=0;
		}

		application.setAttribute("i", ""+i);
		String uid=(String)session.getValue("uid");

		//�����ʾ50�������¼
		if(i<=50) {
			for(k=0;k<i;k++) {
				show=show+(String)application.getAttribute("show"+k);  
			}
		}
		else {
			for(k=0;k<i-1;k++) {
				//��ǰ����, ȥ��������ʷ��¼
				application.setAttribute("show"+k,
								application.getAttribute("show"+(k+1)));
				show=show+(String)application.getAttribute("show"+k);  
			}
			i--;
			application.setAttribute("i",""+i)  ;
		}

		show+= "<br>"+uid+">:"+request.getParameter("chat"); 
		application.setAttribute("show"+i,"<br>"+uid+">:"+request.getParameter("chat")) ;
		out.println(show);
		%>
	</body>
</html>

