<?php

session_start();

include("config.php");

if (isset($_GET["tid"])) {

    $link=mysqli_connect("localhost", $username, $password, $dbname);
    //mysql_select_db($dbname);
    $mtid=$_GET["tid"];
    $exec="select * from my_book where bno='$mtid'";
    $result=mysqli_query($link, $exec);
    while($rs=mysqli_fetch_array($result)){
        $no=$rs['bno'];
        $name=$rs['bname'];
        $author=$rs['bauthor'];
        $print=$rs['bprint'];
        $price=$rs['bprice'];                    
    }
?>

<center>
 <table width=50% border=0>
 <form action=updateBooks.php method=post>
   <tr>
   <td colspan=2>
   <div align=center><strong>图书信息更新</strong> </div>
   <hr>
    </td>
    </tr>
    <tr>
    <td> 编号: </td>
    <td><input type=text name=no value=<?php echo $mtid ?> ></td>
    </tr>
    <tr>
    <td> 书名: </td>
    <td><input type=text name=name value=<?php echo $name ?> ></td>
    </tr>
    <tr>
   <td> 作者: </td>
    <td>
   <input type=text name=author value=<?php echo $author ?> ></td>
   </tr>
   <tr>
   <td> 出版: </td>
    <td>
   <input type=text name=print value=<?php echo $print ?> ></td>
   </tr>
    <tr>
    <td> 价格: </td>
    <td> <input type=text name=price value=<?php echo $price ?> ></td>
    </tr>
    <tr>
    <td colspan=2>
    <div align=left> <input type=submit value="更新图书信息"> </div>
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
 <form action=insertBooks.php method=post>
   <tr>
   <td colspan=2>
   <div align=center><strong>增加图书信息</strong> </div>
   <hr>
   </td>
   </tr>
    <tr>
    <td> 编号: </td> 
    <td><input type=text name=no></td>
    </tr>
    <tr>
    <td> 书名: </td>
    <td><input type=text name=name></td>
    </tr>
    <tr>
   <td> 作者: </td>
    <td>
   <input type=text name=author></td>
   </tr>
   <tr>
   <td> 出版: </td>
    <td>
   <input type=text name=print></td>
   </tr>
    <tr>
    <td> 价格: </td>
    <td> <input type=text name=price></td>
    </tr>
    <tr>
    <td colspan=2>
    <div align=left> <input type=submit value="增加图书信息"> </div>
    </td>
    </tr>
 </form>
 </table>
</center>

<?php
}
?>
