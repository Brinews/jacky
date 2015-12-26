<%@ Page Language="C#" AutoEventWireup="true" CodeFile="DisSys.aspx.cs" Inherits="StuSys" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
<asp:datagrid id="DataGrid1" runat="server" AutoGenerateColumns="False" 
        ShowFooter="True" OnItemCommand="ItemsGrid_Command"
    CellPadding="4" BackColor="White" BorderWidth="1px" BorderStyle="None" 
        BorderColor="#CC9966" Width="100%" 
        >
    <SelectedItemStyle Font-Bold="True" ForeColor="#663399" BackColor="#FFCC66"></SelectedItemStyle>
    <ItemStyle ForeColor="#330099" BackColor="White"></ItemStyle>
    <HeaderStyle Font-Bold="True" ForeColor="#FFFFCC" BackColor="#990000"></HeaderStyle>
    <FooterStyle ForeColor="#330099" BackColor="#FFFFCC"></FooterStyle>
    <Columns>
        <asp:TemplateColumn HeaderText="Check">
            <ItemTemplate>
            <asp:CheckBox Checked=false ID="ck_asgn" runat=server />
             </ItemTemplate>
            <FooterTemplate> 
            <asp:LinkButton ID="lb_select" runat="server" CommandName="SelectAll">Select All</asp:LinkButton>
            </FooterTemplate>
            <EditItemTemplate> 
            </EditItemTemplate>
        </asp:TemplateColumn>

     <asp:TemplateColumn HeaderText="Student ID">
      <ItemTemplate>
       <asp:Label id=Label3 runat="server" width=40px Text='<%# DataBinder.Eval(Container, "DataItem.userid") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate> </FooterTemplate>
      <EditItemTemplate> </EditItemTemplate>
     </asp:TemplateColumn>

     <asp:TemplateColumn HeaderText="Student Name">
      <ItemTemplate>
       <asp:Label id=Label1 runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.username") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
          <asp:LinkButton ID="lb_add" runat="server" CommandName="DisAssign">Do Assign</asp:LinkButton>
      </FooterTemplate>
      <EditItemTemplate>
       </EditItemTemplate>
     </asp:TemplateColumn>

    </Columns>
    <PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
   </asp:datagrid>
    </form>
</body>
</html>
