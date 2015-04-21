<form name="iform" action="show.jsp" target="show">
	<table align="center">
		<tr>
			<td>
				<%=session.getValue("uid") %> Says:
			</td>
			<td><input type="textarea" name="chat" size="30"></input>
			</td>
			<td><input type="submit" value="Send" onClick="return ch()"/>
			</td>
		</tr>
	</table>
</form>

<form name="jform" action="list.jsp" target="list">
	<table align="center">
		<tr> <td>
		  <input type="submit" value="View Online Users" />
		</td></tr>
	</table>
</form>

<script type="text/javascript">
function ch() {
	if(document.iform.chat.value=="") {
		alert("please input your words");
		return false;
	}
	else 
		return true;
}
</script> 

