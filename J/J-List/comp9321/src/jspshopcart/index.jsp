<%@ page import = "beans.DblpBean" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
	<table>
		<tr height=40px><td colspan=2 align=right>
				<b>Welcome To Paper Shop</b><hr/>
				<a href="search.jsp">Do Search</a></td></tr>
	<%
	DblpBean myDblp = DblpBean.getInstance();

	String content = myDblp.getDblpRandomly(10);

	out.write(content);
	%>
	</table>

</body>
