<html>
	<body>
		<font color="blue">
			<%
			String nid = (String) session.getValue("uid");
			String users = (String)application.getAttribute("usersList");
			boolean flag = false;

			if (users != null) {
				String[] ulist = users.split("<br/>");
				for (int i = 0; i < ulist.length; i++)
					if (ulist[i].equals(nid))
						flag = true;
				if (flag != true)
					users += nid + "<br/>";
			}
			else {
				users = nid + "<br/>";
			}

			application.setAttribute("usersList", users);

			out.println(users);
			%>
		</font>
	</body>
</html>
