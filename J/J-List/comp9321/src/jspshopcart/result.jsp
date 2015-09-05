<%@ page import = "beans.DblpBean" %>
<%@ page import ="java.util.*" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
	<table>
		<tr height=40px><td colspan=2 align=right>
				<b>Welcome To Paper Shop</b><hr/>
				<a href="shopcart.jsp">My Cart</a></td></tr>
	<%

	String title = request.getParameter("title");
	String author = request.getParameter("author");
	String year = request.getParameter("year");
	String pageno = request.getParameter("pageno");

	Map<String, String> hashKey = new HashMap<String, String>();

	String href = "";

	if (null != title && !"".equals(title)) {
		hashKey.put("title", title);
		href += "title=";
		href += title;
	}

	if (null != author && !"".equals(author)) {
		hashKey.put("author", author);
		if (!"".equals(href)) href += "&";
		href += "author=";
		href += author;
	}

	if (null != year && !"".equals(year)) {
		hashKey.put("year", year);
		if (!"".equals(href)) href += "&";
		href += "year=";
		href += year;
	}

	int pageNo = 0; // every page 10 items

	if (null != pageno) {
		pageNo = Integer.parseInt(pageno);
	}

	int[] isEnd = new int[1]; // is reach the end
	isEnd[0] = 0;

	DblpBean myDblp = DblpBean.getInstance();
	String content = myDblp.getSearchDblp(hashKey, pageNo, isEnd);

	if (!"".equals(content)) {
		out.write(content); // output searched papers
	} else {
		out.write("<tr><td colspan=2>Sorry, no matching datasets found"
			+ "</td></tr>");
	}

	String links = "<tr><td colspan=2>";

	if (pageNo != 0) {
		pageNo -= 1;
		String prev = "pageno=" + pageNo;
		if (!"".equals(href)) prev += "&" + href;
		links += "<a href=result.jsp?"+prev+"> Prev </a>";
	}

	if (isEnd[0] == 0) {
		pageNo += 1;
		String next = "pageno=" + pageNo;
		if (!"".equals(href)) next += "&" + href;
		links += "&nbsp&nbsp&nbsp<a href=result.jsp?"+next+"> Next </a>";
	}

	links += "</td></tr>";

	out.write(links); //output links

	%>
	</table>

</body>
