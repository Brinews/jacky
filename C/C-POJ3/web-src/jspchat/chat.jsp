<% 
String uid=new String();
uid=request.getParameter("uid");//
session.putValue("uid",uid);
application.setAttribute("show","");
%>

<frameset cols=%80,%20>
	<frameset rows=80%,20%>
		<frame name="show" src="show.jsp" />
		<frame name="input" src="input.jsp" />
	</frameset>
	<frame name="list" src="list.jsp" />
</frameset>

