<%@ page import = "beans.DblpBean" %>
<%@ page import = "java.util.*" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<%
	Map<String, String> cart = (Map<String, String>)
								session.getAttribute("cart");

	String key = request.getParameter("key");
	if (null != key && !"".equals(key)) {
		if (cart.containsKey(key))
			cart.remove(key);
	}

	response.sendRedirect("shopcart.jsp");

%>
