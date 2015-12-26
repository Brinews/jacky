<%@ Page Language="C#" AutoEventWireup="true" CodeFile="login.aspx.cs" Inherits="login" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <link href="App_Themes/NetShop/reset.css" rel="stylesheet" type="text/css" />
    <link href="App_Themes/NetShop/StyleSheet.css" rel="stylesheet" type="text/css" />
</head>
<body>
<form runat="server">

<div style="margin-top:30%;margin-left:100px">
<asp:Label ID="Label1" runat="server" Font-Names="Courier New" 
    Font-Size="Medium" Text="UserName:"></asp:Label>
<asp:TextBox ID="TextBox1" runat="server" Height="24px"></asp:TextBox>
<br />
<asp:Label ID="Label2" runat="server" Font-Names="Courier New" 
    Font-Size="Medium" Text="Password:"></asp:Label>
<asp:TextBox ID="TextBox2" TextMode=Password runat="server" Height="24px"></asp:TextBox>
<br />
<asp:Label ID="Label3" runat="server" Font-Names="Courier New" 
    Font-Size="Medium" Text="LoginType:"></asp:Label>
<asp:DropDownList ID="DropDownList1" runat="server" Height=24px Width="163px">
 <asp:ListItem Text="Teacher" Value="teacher"></asp:ListItem>
 <asp:ListItem Text="Student" Value="student"></asp:ListItem>
</asp:DropDownList>
<br />
<br />
<asp:Button ID="Button1" runat="server" Font-Names="Aharoni" Height="34px" 
    onclick="Button1_Click" Text="Login" Width="70px" />
<asp:Button ID="Button2" runat="server" Font-Names="Aharoni" Height="34px" 
    onclick="Button2_Click" Text="Reset" Width="70px" />
    </div>

</form>
    </body>
</html>
