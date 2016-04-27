<?php
	include("../config.php");
    $servername = "localhost";
    $username = "root";

    // Create connection
    $db = mysqli_connect($servername, $username, $password);

    // Check connection
    if (!$db) {
      die("Connection failed: " . mysqli_connect_error()) . " <br/>";
    }
    echo "Connected successfully <br/>";

    // get FORM contents below
    $tid = mysqli_real_escape_string($db, $_POST["tid"]);
    $title = mysqli_real_escape_string($db, $_POST["title"]);
    $tdate = mysqli_real_escape_string($db, $_POST["tdate"]);
    $tcap = mysqli_real_escape_string($db, $_POST["tcap"]);
    $tloc = mysqli_real_escape_string($db, $_POST["tloc"]);
    $tdesc = mysqli_real_escape_string($db, $_POST["tdesc"]);

    $query = "update mytask.my_task set utaskname='$title', utaskdesc='$tdesc', uspace='$tcap', ulocation='$tloc', ubegin='$tdate' where uid=$tid";

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "update successful <br/>";
    }

    mysqli_close($db);  

	header("Location: listTask.php");
?>
