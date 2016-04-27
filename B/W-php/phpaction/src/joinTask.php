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
    echo "Connected successfully <br/>";

    // get FORM contents below
    $uid = mysqli_real_escape_string($db, $_GET["uid"]);
    $tid = mysqli_real_escape_string($db, $_GET["tid"]);
	$now = date("Y/m/d");

    $query = "INSERT INTO mytask.my_attend (userid, taskid, registerdate) VALUES ($uid, $tid, '".$now."')";

    echo "Your query statement: " . $query . "<br />";

    if (!mysqli_query($db, $query)){
      die('Error: ' . mysqli_error($db));
    }
    else{
      echo "Register successful <br/>";
    }

    mysqli_close($db);  

	header("Location: viewTask.php");
?>
