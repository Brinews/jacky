<%@ page import ="beans.Employee" %>
<%@ page import ="beans.EmpInfo" %>

<head>
<link href="./css/table.css" rel="stylesheet" type="text/css"></link>
</head>

<body>
  <form action="empUpdate">
    <%
		//variables must be init

		String id = "";
		id = request.getParameter("empid");
		//out.write(id);

		String name = "", status="", title="", site="";

		Employee emp = new Employee();

		if (null != id && !"".equals(id)) {
			EmpInfo empInf = emp.getEmployeeInfoById(id);

			name = empInf.empName;
			status = empInf.empStatus;
			title = empInf.empTitle;
			site = empInf.empSite;
		}

		//out.write(name);
    %>
	<table align="center" width="60%" class="table">
	<tr>
	<td> EmpID:</td>
	<td><input type="text" name="empid" value="<%=id%>" readonly class="input"/></td>
	</tr>

	<tr><td>EmpName:</td>
	<td><input type="text" name="empname" value="<%=name%>" readonly class="input"/></td>
	</tr>

	<tr><td> EmpStatus:</td>
	<td><input type="text" name="empstatus" value="<%=status%>"/></td>
	</tr>

	<tr><td>EmpTitle:</td>
	<td><input type="text" name="emptitle" value="<%=title%>"/></td>
	</tr>

	<tr><td>EmpSite:</td>
	<td><input type="text" name="empsite" value="<%=site%>"/></td>
	</tr>

	<tr> <td colspan=2> <input type="submit" value="Update Employee Infomation"/>
	<a href="Javascript:history.go(-1);">back up</a>
	</td></tr>

	</table>

  </form>
</body>
