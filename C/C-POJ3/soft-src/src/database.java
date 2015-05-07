import java.sql.*;

public class database {
    public static Connection cn;
    public static Statement st;
    public static ResultSet rs;

    public static boolean joinDB() {
        boolean joinFlag;
        try {
            joinFlag = true;
            Class.forName("sun.jdbc.odbc.JdbcOdbcDriver");
            //cn = DriverManager.getConnection("jdbc:odbc:student","sa","");
            cn = DriverManager.getConnection("jdbc:odbc:student","sa","");
            
            cn.setCatalog("student");//加载数据库
            System.out.println("数据库连接成功");
            st = cn.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE,
                                    ResultSet.CONCUR_READ_ONLY);
            return joinFlag;

        } catch (SQLException sqlEx) {
            System.out.println(sqlEx.getMessage());

            joinFlag = false;
            return joinFlag;

        } catch (ClassNotFoundException notfoundEX) {
            System.out.println(notfoundEX.getMessage());

            joinFlag = false;
            return joinFlag;
        }
    }

    public static boolean executeSQL(String sqlString) {
        boolean executeFlag;
        try {
            st.execute(sqlString);
            executeFlag = true;
        } catch (Exception e) {
            executeFlag = false;
            System.out.println("sql exception:" + e.getMessage());
        }
        return executeFlag;
    }


    public static boolean query(String sqlString) {

        try {
            rs = null;
            //System.out.println(sqlString);
            rs = st.executeQuery(sqlString);
        } catch (Exception Ex) {
            System.out.println("sql exception:" + Ex);
            return false;
        }
        return true;
    }
}
