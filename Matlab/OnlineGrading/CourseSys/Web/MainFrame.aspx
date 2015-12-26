<%@ Page Language="C#" AutoEventWireup="true"  CodeFile="MainFrame.aspx.cs" Inherits="WestGarden.Web.Products" StylesheetTheme="NetShop" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <style type="text/css">
        #mainFrame
        {
            margin-top: 0px;
        }
    </style>
</head>
<body>
<form id="form1" runat="server">
<div class="wp">
    <div id="nv">
        <asp:Repeater ID="repCategories" runat="server">
            <HeaderTemplate>
                <ul>
            </HeaderTemplate>
            <ItemTemplate>
                <li>
                    <asp:HyperLink runat="server" ID="lnkCategory" NavigateUrl='<%# string.Format("~/{0}.aspx", Eval("id")) %>'
                    target="mainFrame"
                        Text='<%# Eval("Name") %>' />
                    <asp:HiddenField runat="server" ID="hidCategoryId" Value='<%# Eval("id") %>' />
                </li>
            </ItemTemplate>
            <FooterTemplate>
                </ul>
            </FooterTemplate>
        </asp:Repeater>
    </div>
    </div>
  <div class="wp">

    <div style="width: 10%; margin-top: 10px; margin-left: 0%; height: 700px; padding: 0px; float:left; border-right-width: medium; border-right-color: #0000FF;">
    <asp:Label id="labelW" runat="server" Text="Welcome."></asp:Label>
    <br />
    <asp:Label ID="dat" runat="server" Text=""></asp:Label>
    <br />
    <br />
    <asp:LinkButton ID="logout" runat="server" Text="Logout" onclick="logout_Click" 
            Font-Underline="True" ForeColor="#FF3300"></asp:LinkButton>
    </div>
    <div style="width: 90%; margin-top: 10px; margin-left: 0%; height: 700px; padding: 0px; float:right">
        <asp:Panel ID="Panel3" runat="server" Width="100%" Height="700px">
            <iframe id="mainFrame" width="100%" height="100%" scrolling="no" name="mainFrame"
                runat="server" frameborder="0"></iframe>
        </asp:Panel>
    </div>
   </div>

    </form>
</body>
</html>
