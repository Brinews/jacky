<html>
    <head>
        <title>������Ϣ</title>
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
                    <caption>������Ϣ��</caption>
                    <tr><td colspan=6 align=left><a href="listBooks.php">�鼮��Ϣ</a></td></tr>
                    <tr><td colspan=4 align=right><a href="logout.php">�˳�</a></td></tr>
                    <tr><td colspan=4 align=right>
                    ���ս��Ŀ���Ž��м�����
                    <input type=text name=sbid />
                    <input type=submit value=���� /></td></tr>
                        <th>���߿���</th>
                        <th>��������</th>
                        <th>�����Ա�</th>
                        <th>����</th>
<?php
session_start();
include("config.php");

$link=mysqli_connect("localhost", $username, $password, $dbname);
$exec="select * from my_student";
if ($ubid != "") {
    // ����������ģ��ƥ��
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
    echo "<a href=addUsers.php?sid=$no>�༭</a>&nbsp";
    echo "<a href=delUsers.php?sid=$no>ɾ��</a>&nbsp";
    echo "<a href=listRecord.php?sid=$no>���ļ�¼</a>&nbsp";
?> 
</td>
                </tr>
<?php
}
mysqli_close($link);
?>
<tr><td colspan=4 align=left><a href="addUsers.php">��Ӷ���</a></td></tr>
                </table>
            </form>
        </center>
    </body>
</html>
