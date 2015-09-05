<%@ page import = "beans.DblpBean" %>
<%@ page import = "java.util.*" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
	<table>
		<tr height=40px><td colspan=2 align=right>
		    <b>Shop Cart List</b><hr/></td>
		</tr>
	<%

	//1. get cart list
	Map<String, String> cart = (Map<String, String>)
								session.getAttribute("cart");

    //2. output
	int hasPaper = 0;
	String ids = "";

	if(cart != null) {

		Set<String> paperKeys = cart.keySet();

		Integer idx = 0;

    	for (String paperKey : paperKeys) {
			out.write(cart.get(paperKey));
			hasPaper = 1;

			ids += "+++"+ paperKey; 

			out.write("<tr><td colspan=2><input type=checkbox id="
					+paperKey+" name="+paperKey
					+"/>Mark to remove</td></tr>");

			idx = idx + 1;
        }
    }

	if (hasPaper == 0) {
	out.write("<tr><td colspan=2>Shopping Cart is Empty!</td></tr>");
	}

	if (!"".equals(ids))
		ids = ids.substring(3);

	%>
	<tr>
		<td>
			<input type="button" value="Remove from Cart" onclick="doDelete()"/>
		</td>
		<td>
			<a href="index.jsp"> Back to Search</a>
		</td>
	</tr>
	</table>

	<script type="text/javascript">

		var ids = '<%=ids %>';

		function doDelete() {
			if (ids == '') return;

			var keyList = ids.split("+++");
			var len = keyList.length;

			for (var i = 0; i < len; i++) {
				if ($(keyList[i]).checked) {
					location.href = "delete.jsp?key=" + keyList[i];
				}
			}

			//location.href = "shopcart.jsp";
		}

		function $(id) {
			return document.getElementById(id);
		}
	</script>

</body>
