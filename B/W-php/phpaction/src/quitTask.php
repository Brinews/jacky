<?php
	include("config.php");

    $servername = "localhost";
    $username = "root";

    // Create connection
    $db = mysqli_connect($servername, $username, $password);

    // Check connection
    if (!$db) {
      die("Connection failed: " . mysqli_connect_error()) . " <br/>";
    }

    $uid = mysqli_real_escape_string($db, $_GET["uid"]);
    $tid = mysqli_real_escape_string($db, $_GET["tid"]);

    $query = "delete from mytask.my_attend where userid=$uid and taskid=$tid";

    echo "Your query statement: " . $query . "<br />";

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "Unregister successful <br/>";
    }

    mysqli_close($db);  

	header("Location: viewTask.php");
?>
