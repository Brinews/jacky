<?php
	session_start();

	include("config.php");

    $servername = "localhost";
    $username = "root";

    // Create connection
    $db = mysql_connect($servername, $username, $password);
	mysql_select_db("mytask");

    // Check connection
    if (!$db) {
      die("Connection failed: " . mysqli_connect_error()) . " <br/>";
    }
    echo "Connected successfully <br/>";

	$email = $_POST["email"];
    $pwd1 = $_POST["pwd1"];
    $pwd2 = $_POST["pwd2"];
    $uname = $_POST["uname"];
    $uphone = $_POST["uphone"];

	if ($pwd1 == $pwd2) {

		$exec = "SELECT * FROM mytask.my_user WHERE uemail='$email'";

		$result = mysql_query($exec);
		$rows = mysql_num_rows($result);

		if ($rows == 0) {

			$query = "INSERT INTO mytask.my_user (uemail, upwd, uname, uphone, utype) VALUES ('$email', '$pwd1', '$uname', '$uphone', '00')";

			echo "Your query statement: " . $query . "<br />";

			if (!mysql_query($query)){
				die('Error: ' . mysql_error($db));
			}
			else{
				echo "Insert successful <br/>";

				$exec = "SELECT * FROM mytask.my_user WHERE uemail='$email'";
				$result = mysql_query($exec);
				$rows = mysql_num_rows($result);

				$row = mysql_fetch_assoc($result);
				$_SESSION['USERID']=$row['uid'];

				header("Location: viewTask.php");
			}
		} else {
			header("Location: register.php?err=AlreadyExist");
		}
	} else {
		header("Location: register.php?err=PasswordNotEqual");
	}

    mysql_close($db);  

?>
