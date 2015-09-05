<%@ page import = "beans.DblpBean" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
	<form action="result.jsp">
	<table>
		<tr height=40px><td colspan=2 align=right>
				<b>Welcome To Paper Shop</b><hr/>
				<a href="shopcart.jsp">My Cart</a>
		</td></tr>
		<tr>
			<td><b>Publication:</b></td>
			<td><input type="textarea" name="title" size=30></input></td>
		</tr>
		<tr>
			<td><b>Author:</b></td>
			<td><input type="textarea" name="author" size=30></input></td>
		</tr>
		<tr>
			<td><b>Year:</b></td>
			<td><input type="textarea" name="year" size=30></input></td>
		</tr>
		<tr>
			<td colspan=2><input type="submit" value="Search"/> </td>
		</tr>
	</table>

</body>
