<html>
    <head>
        <title>������Ϣ</title>
    </head>
    <body>
        <center>
<?php

if (isset($_GET["bid"])) {
    $ubid = $_GET["bid"];
} else {
    $ubid = "";
}

if (isset($_GET["sid"])) {
    $usid = $_GET["sid"];
} else {
    $usid = "";
}

?>
            <form name="myForm" action="listRecord.php">
                <table  border="1" cellspacing="0" cellpadding="0">
                    <caption>ͼ�������Ϣ</caption>
                    <tr><td colspan=4 align=right><a href="logout.php">�˳�</a></td></tr>
                        <th>���Ŀ���</th>
                        <th>�鼮���</th>
                        <th>���ʱ��</th>
                        <th>����ʱ��</th>
<?php
session_start();
include("config.php");

$link=mysqli_connect("localhost", $username, $password, $dbname);

$exec="select * from my_record where ";
if ($ubid != "") {
    $exec = $exec." rbno ='$ubid'";
}
if ($usid != "") {
    $exec = $exec." rsno ='$usid'";
}

$result=mysqli_query($link, $exec);

while($rs=mysqli_fetch_array($result, MYSQL_ASSOC)){
    $bno=$rs['rbno'];
    $sno=$rs['rsno'];
    $from=$rs['fromdate'];
    $to=$rs['todate'];
?>
                <tr align="center">
                <td><?php echo $sno ?></td>
                <td><?php echo $bno ?></td>
                <td><?php echo $from ?></td>
                <td><?php echo $to ?></td>
                </tr>
<?php
}
mysqli_close($link);
?>
<tr><td colspan=4 align=left>
<?php
if ($ubid != "") {
 echo "<a href=listBooks.php>����</a>";
} else {
 echo "<a href=listUsers.php>����</a>";
}
?>
</td></tr>
                </table>
            </form>
        </center>
    </body>
</html>
