<%@ Page Language="C#" AutoEventWireup="true" CodeFile="AssignSys.aspx.cs" Inherits="StuSys" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <asp:DataGrid ID="DataGrid1" runat="server" AutoGenerateColumns="False" ShowFooter="True"
        OnItemCommand="ItemsGrid_Command" CellPadding="4" BackColor="White" BorderWidth="1px"
        BorderStyle="None" BorderColor="#CC9966" Width="100%" OnDeleteCommand="DataGrid1_DeleteCommand"
        OnEditCommand="DataGrid1_EditCommand" OnUpdateCommand="DataGrid1_UpdateCommand"
        OnCancelCommand="DataGrid1_CancelCommand">
        <SelectedItemStyle Font-Bold="True" ForeColor="#663399" BackColor="#FFCC66"></SelectedItemStyle>
        <ItemStyle ForeColor="#330099" BackColor="White"></ItemStyle>
        <HeaderStyle Font-Bold="True" ForeColor="#FFFFCC" BackColor="#990000"></HeaderStyle>
        <FooterStyle ForeColor="#330099" BackColor="#FFFFCC"></FooterStyle>
        <Columns>
            <asp:TemplateColumn HeaderText="Questions">
                <ItemTemplate>
                    <asp:ImageButton ImageUrl="~/App_Themes/NetShop/Images/tree.gif" CommandName="AddQuestion"
                        runat="server" />
                </ItemTemplate>
                <FooterTemplate>
                    <asp:LinkButton ID="LinkButton1" runat="server" CommandName="Insert">Add Assignment</asp:LinkButton>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
                <ItemStyle Font-Bold="False" Font-Italic="False" Font-Overline="False" Font-Strikeout="False"
                    Font-Underline="False" HorizontalAlign="Center" VerticalAlign="Middle" />
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="ID" Visible="false">
                <ItemTemplate>
                    <asp:Label ID="Label10" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.id") %>'></asp:Label>
                </ItemTemplate>
                <EditItemTemplate>
                    <asp:Label ID="Label11" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.id") %>'></asp:Label>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Name">
                <ItemTemplate>
                    <asp:Label ID="Label3" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.name") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                    <asp:TextBox ID="TextBox6" runat="server" Width="80px"></asp:TextBox>
                </FooterTemplate>
                <EditItemTemplate>
                    <asp:TextBox ID="TextBox5" runat="server" Width="80px" Text='<%# DataBinder.Eval(Container, "DataItem.name") %>'>
                    </asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Description">
                <ItemTemplate>
                    <asp:Label ID="Label1" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.desc") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                    <asp:TextBox ID="TextBox2" runat="server"></asp:TextBox>
                </FooterTemplate>
                <EditItemTemplate>
                    <asp:TextBox ID="TextBox1" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.desc")%>'></asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Objective">
                <ItemTemplate>
                    <asp:Label ID="Label2" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.objective") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                    <asp:TextBox ID="TextBox4" Width="80px" runat="server"></asp:TextBox>
                </FooterTemplate>
                <EditItemTemplate>
                    <asp:TextBox ID="TextBox3" Width="80px" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.objective") %>'></asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Critical">
                <ItemTemplate>
                    <asp:Label ID="Label4" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.critical") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                    <asp:TextBox ID="TextBox8" runat="server"></asp:TextBox>
                </FooterTemplate>
                <EditItemTemplate>
                    <asp:TextBox ID="TextBox7" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.critical") %>'></asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Score">
                <ItemTemplate>
                    <asp:Label ID="Label6" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.score") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                    <asp:TextBox ID="TextBox10" runat="server" Width="40px"></asp:TextBox>
                </FooterTemplate>
                <EditItemTemplate>
                    <asp:TextBox ID="TextBox9" runat="server" Width="40px" Text='<%# DataBinder.Eval(Container, "DataItem.score") %>'></asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Weight">
                <ItemTemplate>
                    <asp:Label ID="Label7" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.weight") %>'> </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                    <asp:TextBox ID="TextBox12" runat="server" Width="40px"></asp:TextBox>
                </FooterTemplate>
                <EditItemTemplate>
                    <asp:TextBox ID="TextBox11" runat="server" Width="40px" Text='<%# DataBinder.Eval(Container, "DataItem.weight") %>'></asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:EditCommandColumn ButtonType="PushButton" UpdateText="更新" HeaderText="编辑" CancelText="取消"
                EditText="编辑"></asp:EditCommandColumn>
            <asp:ButtonColumn Text="删除" CommandName="Delete" HeaderText="删除"></asp:ButtonColumn>
            <asp:ButtonColumn Text="指派" CommandName="Assign" HeaderText="指派"></asp:ButtonColumn>
        </Columns>
        <PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
    </asp:DataGrid>
    </form>
</body>
</html>
