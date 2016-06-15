<html>
    <head>
        <title>图书信息</title>
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
            <form name="myForm" action="listBooks.php">
                <table  border="1" cellspacing="0" cellpadding="0">
                    <caption>图书信息表</caption>
                    <tr><td colspan=6 align=left><a href="listUsers.php">读者信息</a></td></tr>
                    <tr><td colspan=6 align=right><a href="logout.php">退出</a></td></tr>
                    <tr><td colspan=6 align=right>
                    按照图书编号进行检索：
                    <input type=text name=sbid />
                    <input type=submit value=检索 /></td></tr>
                        <th>编号</th>
                        <th>书名</th>
                        <th>作者</th>
                        <th>出版</th>
                        <th>价格</th>
                        <th>操作</th>
<?php
session_start();
include("config.php");

$link=mysqli_connect("localhost", $username, $password, $dbname);
//mysql_select_db($dbname);
$exec="select * from my_book";
if ($ubid != "") {
    // 条件检索，模糊匹配
    $exec = $exec." where bno like '$ubid%'";
}

$result=mysqli_query($link, $exec);

while($rs=mysqli_fetch_array($result, MYSQL_ASSOC)){
    $no=$rs['bno'];
    $name=$rs['bname'];
    $author=$rs['bauthor'];
    $print=$rs['bprint'];
    $price=$rs['bprice'];                    
?>
                <tr align="center">
                <td><?php echo $no ?></td>
                <td><?php echo $name ?></td>
                <td><?php echo $author ?></td>
                <td><?php echo $print ?></td>
                <td><?php echo $price ?></td>
                <td>
<?php 
    echo "<a href=addBooks.php?tid=$no>编辑</a>&nbsp";
    echo "<a href=delBooks.php?tid=$no>删除</a>&nbsp";
    echo "<a href=listRecord.php?bid=$no>借阅信息</a>&nbsp";
    echo "<a href=addRecord.php?bid=$no>借出</a>";
?> 
</td>
                </tr>
<?php
}
mysqli_close($link);
?>
<tr><td colspan=6 align=left><a href="addBooks.php">添加图书</a></td></tr>
                </table>
            </form>
        </center>
    </body>
</html>
