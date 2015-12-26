<%@ Page Language="C#" AutoEventWireup="true" CodeFile="StuSys.aspx.cs" Inherits="StuSys" %>

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
        BorderColor="#CC9966" Width="100%" ondeletecommand="DataGrid1_DeleteCommand" 
        oneditcommand="DataGrid1_EditCommand"
        onupdatecommand="DataGrid1_UpdateCommand" 
        oncancelcommand="DataGrid1_CancelCommand">
    <SelectedItemStyle Font-Bold="True" ForeColor="#663399" BackColor="#FFCC66"></SelectedItemStyle>
    <ItemStyle ForeColor="#330099" BackColor="White"></ItemStyle>
    <HeaderStyle Font-Bold="True" ForeColor="#FFFFCC" BackColor="#990000"></HeaderStyle>
    <FooterStyle ForeColor="#330099" BackColor="#FFFFCC"></FooterStyle>
    <Columns>
        <asp:TemplateColumn HeaderText="Scores">
        <ItemTemplate>
        <asp:ImageButton ImageUrl="~/App_Themes/NetShop/Images/root.gif" CommandName="ViewScore" runat="server"/> </ItemTemplate>
        <FooterTemplate>
       <asp:LinkButton id="LinkButton1" runat="server" CommandName="Insert">Add Student</asp:LinkButton>
         </FooterTemplate>
        <EditItemTemplate> </EditItemTemplate>
            <ItemStyle Font-Bold="False" Font-Italic="False" Font-Overline="False" 
                Font-Strikeout="False" Font-Underline="False" HorizontalAlign="Center" 
                VerticalAlign="Middle" />
        </asp:TemplateColumn>
     <asp:TemplateColumn HeaderText="Student ID">
      <ItemTemplate>
       <asp:Label id=Label3 runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.userid") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
       <asp:TextBox id="TextBox6" Width=80px runat="server"></asp:TextBox>
      </FooterTemplate>
      <EditItemTemplate>
       <asp:Label id="Label5" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.userid") %>'>
       </asp:Label>
      </EditItemTemplate>
     </asp:TemplateColumn>
     <asp:TemplateColumn HeaderText="Student Name">
      <ItemTemplate>
       <asp:Label id=Label1 runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.username") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
       <asp:TextBox id="TextBox2" Width=80px runat="server"></asp:TextBox>
      </FooterTemplate>
      <EditItemTemplate>
       <asp:TextBox id="TextBox1" runat="server"  Text='<%# DataBinder.Eval(Container, "DataItem.username")%>'></asp:TextBox>
       </EditItemTemplate>
     </asp:TemplateColumn>
     <asp:TemplateColumn HeaderText="Login Password">
      <ItemTemplate>
       <asp:Label id=Label2 runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.password") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
       <asp:TextBox id="TextBox4" Width=80px runat="server" ></asp:TextBox>
      </FooterTemplate>
      <EditItemTemplate>
       <asp:TextBox id="TextBox3" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.password") %>'></asp:TextBox>
      </EditItemTemplate>
     </asp:TemplateColumn>

        <asp:TemplateColumn HeaderText="Email Addr">
            <ItemTemplate>
       <asp:Label id="Label4" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.useremail") %>'>
       </asp:Label>
      </ItemTemplate>
      <FooterTemplate>
       <asp:TextBox id="TextBox8" runat="server"></asp:TextBox>
      </FooterTemplate>
      <EditItemTemplate>
       <asp:TextBox id="TextBox7" runat="server"  Text='<%# DataBinder.Eval(Container, "DataItem.useremail") %>'></asp:TextBox>
      </EditItemTemplate>
     </asp:TemplateColumn>

     <asp:EditCommandColumn ButtonType="PushButton" UpdateText="更新" HeaderText="编辑" CancelText="取消" EditText="编辑"></asp:EditCommandColumn>
  <asp:ButtonColumn Text="删除" CommandName="Delete" HeaderText ="删除"></asp:ButtonColumn>

    </Columns>
    <PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
   </asp:datagrid>
    </form>
</body>
</html>
