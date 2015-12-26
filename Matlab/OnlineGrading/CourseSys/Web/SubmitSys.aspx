<%@ Page Language="C#" AutoEventWireup="true" CodeFile="SubmitSys.aspx.cs" Inherits="StuSys" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <asp:DataGrid ID="DataGrid1" runat="server" AutoGenerateColumns="False" ShowFooter="True"
        OnItemCommand="ItemsGrid_Command" CellPadding="4" BackColor="White" BorderWidth="1px"
        >
        <SelectedItemStyle Font-Bold="True" ForeColor="#663399" BackColor="#FFCC66"></SelectedItemStyle>
        <ItemStyle ForeColor="#330099" BackColor="White"></ItemStyle>
        <HeaderStyle Font-Bold="True" ForeColor="#FFFFCC" BackColor="#990000"></HeaderStyle>
        <FooterStyle ForeColor="#330099" BackColor="#FFFFCC"></FooterStyle>
        <Columns>
            <asp:TemplateColumn HeaderText="">
                <ItemTemplate>
                    <asp:ImageButton ImageUrl="~/App_Themes/NetShop/Images/task.gif" CommandName="View"
                        runat="server" />
                </ItemTemplate>
                <FooterTemplate>
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
                </EditItemTemplate>
            </asp:TemplateColumn>

            <asp:TemplateColumn HeaderText="Name">
                <ItemTemplate>
                    <asp:Label ID="Label3" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.name") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                    </asp:TextBox>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Description">
                <ItemTemplate>
                    <asp:Label ID="Label1" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.desc") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Objective">
                <ItemTemplate>
                    <asp:Label ID="Label2" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.objective") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Critical">
                <ItemTemplate>
                    <asp:Label ID="Label4" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.critical") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="ScoreLevel">
                <ItemTemplate>
                    <asp:Label ID="Label6" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.score") %>'>
                    </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>
            <asp:TemplateColumn HeaderText="Weight">
                <ItemTemplate>
                    <asp:Label ID="Label7" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.weight") %>'> </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>

            <asp:TemplateColumn HeaderText="Is Submit?">
                <ItemTemplate>
                    <asp:Label ID="Label8" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.is_submit") %>'> </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>

            <asp:TemplateColumn HeaderText="My Score">
                <ItemTemplate>
                    <asp:Label ID="Label9" runat="server" Text='<%# DataBinder.Eval(Container, "DataItem.myscore") %>'> </asp:Label>
                </ItemTemplate>
                <FooterTemplate>
                </FooterTemplate>
                <EditItemTemplate>
                </EditItemTemplate>
            </asp:TemplateColumn>
            
            <asp:ButtonColumn Text="Submit" CommandName="Submit" HeaderText="Action"></asp:ButtonColumn>
        </Columns>
        <PagerStyle HorizontalAlign="Center" ForeColor="#330099" BackColor="#FFFFCC"></PagerStyle>
    </asp:DataGrid>
    </form>
</body>
</html>
