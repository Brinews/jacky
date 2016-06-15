<html>
    <head>
        <title>读者信息</title>
    </head>
    <body>
        <center>
<?php
if (isset($_GET["sbid"])) {
    $ubid = $_GET["sbid"];
} else {
    $ubid = "";
}
?>
            <form name="myForm" action="listUsers.php">
                <table  border="1" cellspacing="0" cellpadding="0">
                    <caption>读者信息表</caption>
                    <tr><td colspan=6 align=left><a href="listBooks.php">书籍信息</a></td></tr>
                    <tr><td colspan=4 align=right><a href="logout.php">退出</a></td></tr>
                    <tr><td colspan=4 align=right>
                    按照借阅卡编号进行检索：
                    <input type=text name=sbid />
                    <input type=submit value=检索 /></td></tr>
                        <th>读者卡号</th>
                        <th>读者姓名</th>
                        <th>读者性别</th>
                        <th>操作</th>
<?php
session_start();
include("config.php");

$link=mysqli_connect("localhost", $username, $password, $dbname);
$exec="select * from my_student";
if ($ubid != "") {
    // 条件检索，模糊匹配
    $exec = $exec." where sno like '$ubid%'";
}

$result=mysqli_query($link, $exec);

while($rs=mysqli_fetch_array($result, MYSQL_ASSOC)){
    $no=$rs['sno'];
    $name=$rs['sname'];
    $sex=$rs['ssex'];
?>
                <tr align="center">
                <td><?php echo $no ?></td>
                <td><?php echo $name ?></td>
                <td><?php echo $sex ?></td>
                <td>
<?php 
    echo "<a href=addUsers.php?sid=$no>编辑</a>&nbsp";
    echo "<a href=delUsers.php?sid=$no>删除</a>&nbsp";
    echo "<a href=listRecord.php?sid=$no>借阅记录</a>&nbsp";
?> 
</td>
                </tr>
<?php
}
mysqli_close($link);
?>
<tr><td colspan=4 align=left><a href="addUsers.php">添加读者</a></td></tr>
                </table>
            </form>
        </center>
    </body>
</html>
