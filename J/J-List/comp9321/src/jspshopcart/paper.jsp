<%@ page import = "beans.DblpBean" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
	<table>
		<tr height=40px><td colspan=2 align=right>
				<b>Welcome To Paper Shop</b><hr/>
				<a href="Javascript:history.go(-1);">Back</a></td></tr>
	<%
	DblpBean myDblp = DblpBean.getInstance();

	String key = request.getParameter("key");
	String content = myDblp.getCachedPaper(key);

	out.write(content);
	%>
	</table>

</body>
