<?php

session_start();

include("config.php");

if (isset($_GET["sid"])) {
    $link=mysqli_connect("localhost", $username, $password, $dbname);
    $mtid=$_GET["sid"];
    $exec="select * from my_student where sno='$mtid'";
    $result=mysqli_query($link, $exec);
    while($rs=mysqli_fetch_array($result)){
        $no=$rs['sno'];
        $name=$rs['sname'];
        $sex=$rs['ssex'];
    }
?>

<center>
 <table width=50% border=0>
 <form action=updateUsers.php method=post>
   <tr> <td colspan=2>
   <div align=center><strong>������Ϣ����</strong> </div> <hr>
    </td> </tr>

    <tr> <td> ����: </td>
    <td><input type=text name=no value=<?php echo $mtid ?> ></td>
    </tr>

    <tr> <td> ����: </td>
    <td><input type=text name=name value=<?php echo $name ?> ></td>
    </tr>

    <tr> <td> �Ա�: </td>
    <td> <input type=text name=sex value=<?php echo $sex ?> ></td>
   </tr>

    <tr> <td colspan=2>
    <div align=left> <input type=submit value="���¶�����Ϣ"> </div>
    </td>
    </tr>
 </form>
 </table>
</center>

<?php
} else {
?>

<center>
 <table width=50% border=0>
 <form action=insertUsers.php method=post>
   <tr> <td colspan=2>
   <div align=center><strong>���Ӷ�����Ϣ</strong> </div> <hr>
   </td> </tr>

    <tr> <td> ����: </td> 
    <td><input type=text name=no></td>
    </tr>

    <tr> <td> ����: </td>
    <td><input type=text name=name></td>
    </tr>

    <tr> <td> �Ա�: </td>
    <td> <input type=text name=sex></td>
    </tr>

    <tr> <td colspan=2>
    <div align=left> <input type=submit value="����ͼ����Ϣ"> </div>
    </td> </tr>

 </form>
 </table>
</center>

<?php
}
?>

