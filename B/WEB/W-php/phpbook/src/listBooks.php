<html>
    <head>
        <title>ͼ����Ϣ</title>
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
                    <caption>ͼ����Ϣ��</caption>
                    <tr><td colspan=6 align=left><a href="listUsers.php">������Ϣ</a></td></tr>
                    <tr><td colspan=6 align=right><a href="logout.php">�˳�</a></td></tr>
                    <tr><td colspan=6 align=right>
                    ����ͼ���Ž��м�����
                    <input type=text name=sbid />
                    <input type=submit value=���� /></td></tr>
                        <th>���</th>
                        <th>����</th>
                        <th>����</th>
                        <th>����</th>
                        <th>�۸�</th>
                        <th>����</th>
<?php
session_start();
include("config.php");

$link=mysqli_connect("localhost", $username, $password, $dbname);
//mysql_select_db($dbname);
$exec="select * from my_book";
if ($ubid != "") {
    // ����������ģ��ƥ��
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
    echo "<a href=addBooks.php?tid=$no>�༭</a>&nbsp";
    echo "<a href=delBooks.php?tid=$no>ɾ��</a>&nbsp";
    echo "<a href=listRecord.php?bid=$no>������Ϣ</a>&nbsp";
    echo "<a href=addRecord.php?bid=$no>���</a>";
?> 
</td>
                </tr>
<?php
}
mysqli_close($link);
?>
<tr><td colspan=6 align=left><a href="addBooks.php">���ͼ��</a></td></tr>
                </table>
            </form>
        </center>
    </body>
</html>
