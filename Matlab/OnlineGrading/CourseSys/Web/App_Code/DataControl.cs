using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

using System.Data;
using System.Data.SqlClient;
using System.Data.OleDb;

/// <summary>
///DataControl 的摘要说明
/// </summary>
public class DataControl
{
    protected string connStr;

    protected SqlConnection myConnection;

    protected SqlCommand myCommand;

    protected SqlDataReader myReader;

    protected DataTable myTable;

    protected SqlDataAdapter myDataAdapter;

    protected DataSet myDataSet;

    protected SqlTransaction myTransaction;
    /// <summary>
    /// 构造函数,每次实例化这个类的时候必定会用到myConnection和myCommand，所以在这里将它们实化，而其他对象未必用到，所以不在构造函数中实例化
    /// </summary>
    /// <param name="connectionString"></param>
    public DataControl(string connectionString)
    {
        connStr = connectionString;// TODO: 在此处添加构造函数逻//
        myConnection = new SqlConnection(connStr);
        myCommand = new SqlCommand();
        myCommand.Connection = myConnection;
        myTable = null;
    }
    /// <summary>
    /// 一定要调用这个方法将数据库关闭
    /// </summary>
    public void CloseReader()
    {
        myReader.Close();
    }
    public void CloseConnection()
    {
        myConnection.Close();
    }
    /// <summary>
    /// 这个方法把数据库打开
    /// </summary>
    public void OpenConnection()
    {
        myConnection.Open();
    }
    /// <summary>
    /// 开始一个事务
    /// </summary>
    public void BeginTransaction()
    {
        myTransaction = myConnection.BeginTransaction();
        myCommand.Transaction = myTransaction;
    }
    /// <summary>
    /// 事务回滚
    /// </summary>
    public void TransactionRollback()
    {
        myTransaction.Rollback();
    }
    /// <summary>
    /// 事务成功
    /// </summary>
    public void TransactionCommit()
    {
        myTransaction.Commit();
    }
    /// <summary>
    /// 执行insert,update,delete数据库操作
    /// </summary>
    /// <param name="sql"></param>
    public void UpdateDataBase(string sql)
    {
        myCommand.CommandText = sql;
        myCommand.ExecuteNonQuery();
    }
    /// <summary>
    /// 通过select语句读取数据，将数据放在myReader中，并返回myReader，执行后应调用CloseConnection()关闭数据库
    /// </summary>
    /// <param name="sql">select语句</param>
    /// <returns></returns>
    public SqlDataReader GetReader(string sql)
    {
        myCommand.CommandText = sql;
        myReader = myCommand.ExecuteReader();
        return myReader;
    }
    /// <summary>
    /// 通过select语句读取数据，将数据放在myTable中，并返回myTable
    /// </summary>
    /// <param name="sql">select语句</param>
    /// <returns></returns>
    public DataTable GetTable(string sql)
    {
        myCommand.CommandText = sql;
        myDataAdapter = new SqlDataAdapter();
        myDataAdapter.SelectCommand = myCommand;
        myTable = new DataTable();
        myDataAdapter.Fill(myTable);
        return myTable;
    }
    /// <summary>
    /// 通过select语句读取数据，并将从第startRecord条数据开始的maxRecord条数据放在myTable中，并返回myTable
    /// </summary>
    /// <param name="sql">select语句</param>
    /// <param name="startRecord">开始记录</param>
    /// <param name="maxRecord">存放的记录数</param>
    /// <returns></returns>
    public DataTable GetTable(string sql, int startRecord, int maxRecord)
    {
        myCommand.CommandText = sql;
        myDataAdapter = new SqlDataAdapter();
        myDataAdapter.SelectCommand = myCommand;
        myDataSet = new DataSet();
        myDataAdapter.Fill(myDataSet, startRecord, maxRecord, "tableName");
        return myDataSet.Tables["tableName"];
    }
    /// <summary>
    /// 计算数据库中的一张表有多少条记录
    /// </summary>
    /// <param name="tableName">存放在数据库中的表</param>
    /// <returns></returns>
    public int CalculateRecord(string tableName, string condition)
    {
        int recordCount = 0;
        string sql = "select count(*) as recordNum from" + tableName;
        if (null != condition && !"".Equals(condition))
        {
            sql += " where " + condition;
        }

        myCommand.CommandText = sql;
        myReader = myCommand.ExecuteReader();
        if (myReader.Read())
        {
            recordCount = Convert.ToInt32(myReader["recordNum"]);
        }
        return recordCount;
    }
    /// <summary>
    /// 在数据库里创建一张只有id的表
    /// </summary>
    /// <param name="tableName">表名</param>
    public void CreateEmptyTable(string tableName)
    {
        myCommand.CommandText = "create table [" + tableName + "](id int identity(1,1))";
        myCommand.ExecuteNonQuery();
    }
    public void DropTable(string tableName)
    {
        myCommand.CommandText = "drop table " + "[" + tableName + "]";
        myCommand.ExecuteNonQuery();
    }
    /// <summary>
    /// 向特定的表里增加一个字段，并表明其类型
    /// </summary>
    /// <param name="fieldName">字段名</param>
    /// <param name="tableName">表名</param>
    /// <param name="type">类型</param>
    public void CreateField(string fieldName, string tableName, string type)
    {
        myCommand.CommandText = "alter table [" + tableName + "] add [" + fieldName + "] " + type;
        myCommand.ExecuteNonQuery();
    }
    /// <summary>
    /// 从特定表里删除一个字段
    /// </summary>
    /// <param name="fieldName">字段名</param>
    /// <param name="tableName">表名</param>
    public void DropField(string fieldName, string tableName)
    {
        myCommand.CommandText = "alter table [" + tableName + "] drop column " + "[" + fieldName + "]";
        myCommand.ExecuteNonQuery();
    }
    /// <summary>
    /// 获取所有的表名
    /// </summary>
    /// <returns></returns>
    public DataTable GetTableNames()
    {
        string sql = "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_TYPE = 'BASE TABLE'";
        myDataAdapter = new SqlDataAdapter(sql, myConnection);
        myTable = new DataTable();
        myDataAdapter.Fill(myTable);
        return myTable;
    }
    /// <summary>
    /// 获取名为tableName的表的表名，用来查询是否存在这张表
    /// </summary>
    /// <param name="tableName"></param>
    /// <returns></returns>
    public SqlDataReader GetSingleTableName(string tableName)
    {
        string sql = "Select TABLE_NAME from information_schema.tables where TABLE_NAME='" + tableName + "'";
        myCommand.CommandText = sql;
        myReader = myCommand.ExecuteReader();
        return myReader;
    }
    /// <summary>
    /// 获得一个dataadapter
    /// </summary>
    /// <param name="sql"></param>
    /// <returns></returns>
    public SqlDataAdapter GetDataAdapter(ref string sql)
    {
        myDataAdapter = new SqlDataAdapter();
        myCommand.CommandText = sql;
        myDataAdapter.SelectCommand = myCommand;
        return myDataAdapter;
    }
    /// <summary>
    /// 通过一个dataadapter获得一个dataset
    /// </summary>
    /// <param name="dataAdapter"></param>
    /// <returns></returns>
    public DataSet GetDataSet(ref SqlDataAdapter dataAdapter)
    {
        myDataSet = new DataSet();
        dataAdapter.Fill(myDataSet);
        return myDataSet;
    }
    /// <summary>
    /// 通过dataset更新数据库
    /// </summary>
    /// <param name="dataAdapter"></param>
    /// <param name="dataSet"></param>
    public void UpdateDataBase(ref SqlDataAdapter dataAdapter, ref DataSet dataSet)
    {
        SqlCommandBuilder myCommandBuilder = new SqlCommandBuilder(dataAdapter);
        myDataAdapter.InsertCommand = myCommandBuilder.GetInsertCommand();
        myDataAdapter.Update(dataSet);
    }
    /// <summary>
    /// 完成从excel到sqlserver的数据转输
    /// </summary>
    /// <param name="excelTable">excel表</param>
    /// <param name="tableName">数据库中的表名</param>
    public void DataTransfers(DataTable excelTable, string tableName)
    {
        CreateEmptyTable(tableName);

        DataRow myRow = excelTable.Rows[0];
        int i = 0;
        for (int j = 0; j < myRow.ItemArray.GetLength(0); j++)
        {
            if (myRow.ItemArray[j].ToString() != "")
            {
                i++;
            }
        }

        for (int k = 0; k < i; k++)
        {
            CreateField(excelTable.Columns[k].ColumnName, tableName, "nvarchar(512)");
        }
        DropField("id", tableName);

        myDataSet = new DataSet();
        myDataAdapter = new SqlDataAdapter();
        myCommand.CommandText = "select * from " + "[" + tableName + "]";
        myDataAdapter.SelectCommand = myCommand;
        myDataAdapter.Fill(myDataSet, "ExcelTable");

        myTable = myDataSet.Tables["ExcelTable"];
        DataRow excelRow;

        foreach (DataRow row in excelTable.Rows)
        {
            excelRow = myTable.NewRow();
            for (int j = 0; j < i; j++)
            {
                excelRow[j] = row[j];
            }
            myTable.Rows.Add(excelRow);

        }
        SqlCommandBuilder myCommandBuilder = new SqlCommandBuilder(myDataAdapter);
        myDataAdapter.InsertCommand = myCommandBuilder.GetInsertCommand();
        myDataAdapter.Update(myDataSet, "ExcelTable");

    }
    /// <summary>
    /// 获得指定表中包含指定字符的字段名的个数
    /// </summary>
    public int CountSimField(string com_str, string tablename)
    {
        int n = 0;
        string sql = "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME ='" + tablename + "'";
        myCommand.CommandText = sql;
        myReader = myCommand.ExecuteReader();
        while (myReader.Read())
        {
            string str = myReader["COLUMN_NAME"].ToString();
            int i = str.Length;
            if (i > 4)
            {
                string str2 = str.Substring(i - 4);
                if (str2 == com_str)
                {
                    n++;
                }
            }
        }
        return n;
    }
    public string[] GetSimField(string com_str, string tablename)
    {
        string[] com_name = new string[10];
        int n = 0;
        string sql = "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME ='" + tablename + "'";
        myCommand.CommandText = sql;
        myReader = myCommand.ExecuteReader();
        while (myReader.Read())
        {
            string str = myReader["COLUMN_NAME"].ToString();
            int i = str.Length;
            if (i > 4)
            {
                string str2 = str.Substring(i - 4);
                if (str2 == com_str)
                {
                    com_name[n] = str;
                    n++;
                }
            }
        }
        return com_name;

    }

    ~DataControl()
    { }

}