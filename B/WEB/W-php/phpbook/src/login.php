<center>
 <table width=50% border=1>
 <form action=loginchk.php method=post>
  <tr>
   <td>
   <div align=center><strong>����Ա��½</strong> </div>
   <hr>
   <p align=center>��½�û�:
   <input type=text name=username tabindex=1></p>
   <p align=center>�û�����:
   <input type=password name=pwd></p>

   <p align=center><font color=red>
<?php 
if (isset($_GET["err"])) {
    echo  "*".$_GET["err"]; 
}?>
    </font></p>

   <div align=center>
    <input type=submit value=Login>
    <input type=reset value=Reset>
   </div>
  </td>
 </tr>
 </form>
 </table>
</center>
