using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Collections;
using System.Data;

public partial class StuSys : System.Web.UI.Page
{
    static string constr = "Server=127.0.0.1;Database=Grading;uid=sa;pwd=123;";
    DataControl myControl = new DataControl(constr);
    string stuId = null;

    protected void Page_Load(object sender, EventArgs e)
    {
        stuId = (string) Session["uid"];

        if (!IsPostBack)
            BindGrid();
    }

    void BindGrid()
    {
        if (stuId == null) return;

        myControl.OpenConnection();

        string objComm = "SELECT * From [Gpa] where [stu_id]='"+stuId+"'";

        DataTable myTable = myControl.GetTable(objComm);
        DataGrid1.DataSource = myTable;
        DataGrid1.DataBind();

        myControl.CloseConnection();
    }

    protected void ItemsGrid_Command(object source, DataGridCommandEventArgs e)
    {
    }
}