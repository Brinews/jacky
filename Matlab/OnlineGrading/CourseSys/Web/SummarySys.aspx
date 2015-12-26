<%@ Page Language="C#" AutoEventWireup="true" CodeFile="SummarySys.aspx.cs" Inherits="StuSys" %>

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

        <asp:TemplateColumn HeaderText="">
        <ItemTemplate>
        <asp:ImageButton ImageUrl="~/App_Themes/NetShop/Images/task.gif" CommandName="DoNothing" runat="server"/> </ItemTemplate>
        <FooterTemplate>
         </FooterTemplate>
        <EditItemTemplate> </EditItemTemplate>
            <ItemStyle Font-Bold="False" Font-Italic="False" Font-Overline="False" 
                Font-Strikeout="False" Font-Underline="False" HorizontalAlign="Center" 
                VerticalAlign="Middle" />
        </asp:TemplateColumn>

         <asp:TemplateColumn HeaderText="Student ID">
      <ItemTemplate>
       <asp:Label id=Label4 runat="server" width=40px Text='<%# DataBinder.Eval(Container, "DataItem.stu_id") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate> </FooterTemplate>
      <EditItemTemplate>
      </EditItemTemplate>
     </asp:TemplateColumn>

     <asp:TemplateColumn HeaderText="My GPA">
      <ItemTemplate>
       <asp:Label id=Label1 runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.gpa") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
      </FooterTemplate>
      <EditItemTemplate>
       </EditItemTemplate>
     </asp:TemplateColumn>

     <asp:TemplateColumn HeaderText="Level">
      <ItemTemplate>
       <asp:Label id=Label3 runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.level") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
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
