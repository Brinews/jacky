﻿using System;
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
        string objComm = "SELECT * From [Distribution] left join [Assignment] on [asgn_id] = [id] where [stu_id]='"+stuId+"'  ";
        DataTable myTable = myControl.GetTable(objComm);
        DataGrid1.DataSource = myTable;
        DataGrid1.DataBind();
        myControl.CloseConnection();

        for (int i = 0; i < DataGrid1.Items.Count; i++)
        {
            LinkButton cb = (LinkButton)(DataGrid1.Items[i].Cells[10].Controls[0]);

            if (((Label)DataGrid1.Items[i].FindControl("Label8")).Text == "Y")
            {
                cb.CommandName = "View";
                cb.Text = "View";
            }
        }
    }

    protected void ItemsGrid_Command(object source, DataGridCommandEventArgs e)
    {
        if (e.CommandName == "Submit")
        {
            string id = ((Label)e.Item.Cells[1].Controls[1]).Text;
            Response.Redirect("AnswerSys.aspx?aid="+ id +"&sid="+stuId);
        }

        if (e.CommandName == "View")
        {
            string id = ((Label)e.Item.Cells[1].Controls[1]).Text;
            Response.Redirect("ReviewSys.aspx?aid="+ id +"&sid="+stuId);
        }
    }
}