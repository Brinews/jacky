<%@ Page Language="C#" AutoEventWireup="true" CodeFile="PassSys.aspx.cs" Inherits="login" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <link href="App_Themes/NetShop/reset.css" rel="stylesheet" type="text/css" />
    <link href="App_Themes/NetShop/StyleSheet.css" rel="stylesheet" type="text/css" />
</head>
<body>
<form runat="server">

<div style="margin-left:30px;margin-top:30px">
<table>
<tr>
<td>
<asp:Label ID="Label1" runat="server" Font-Names="Courier New" 
    Font-Size="Medium" Text="Orginal Password:"></asp:Label>
</td>
<td>
<asp:TextBox ID="TextBox1" runat="server" Height="24px" TextMode="Password"></asp:TextBox>
</td>
</tr>
<tr>
<td>
<asp:Label ID="Label2" runat="server" Font-Names="Courier New" 
    Font-Size="Medium" Text="New Password:"></asp:Label>
</td>
<td>
<asp:TextBox ID="TextBox2" TextMode=Password runat="server" Height="24px"></asp:TextBox>
</td>
</tr>
<tr>
<td>
<asp:Label ID="Label3" runat="server" Font-Names="Courier New" 
    Font-Size="Medium" Text="Confirm New Password:"></asp:Label>
</td>
<td>
<asp:TextBox ID="TextBox3" TextMode=Password runat="server" Height="24px"></asp:TextBox>
</td>
</tr>
</table>

<asp:Button ID="Button1" runat="server" Font-Names="Aharoni" Height="34px" 
    onclick="Button1_Click" Text="Confirm Change" Width="162px" />
<asp:Button ID="Button2" runat="server" Font-Names="Aharoni" Height="34px" 
    onclick="Button2_Click" Text="Reset" Width="70px" />
</div>

</form>
    </body>
</html>
