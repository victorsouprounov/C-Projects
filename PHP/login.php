<?php

session_start();

if (isset($_POST['submit']))
{
	include 'dbh.inc.php';

	$uid = mysqli_real_escape_string($conn,$_POST['uid']);
	$pwd = mysqli_real_escape_string($conn,$_POST['pwd']);

	$sql = "SELECT * FROM users WHERE user_uid = '$uid' AND user_pwd = '$pwd' ";
	$result = mysqli_query($conn,$sql);
	$resultCheck = mysqli_num_rows($result);

	if($resultCheck < 1)
	{
		header("Location: ../index.php?login=error");
		exit();
	}
	else
	{
		
		$row = mysqli_fetch_assoc($result);
		$_SESSION['u_type'] = $row['user_type'];
		$_SESSION['u_id'] = $row['user_id'];
		$_SESSION['u_first'] = $row['user_first'];
		$_SESSION['u_last'] = $row['user_last'];
		$_SESSION['u_email'] = $row['user_email'];
		$_SESSION['u_uid'] = $row['user_uid'];
		$_SESSION['u_pwd'] = $row['user_pwd'];

		header("Location: ../index.php?login=success");
		exit();
	}

}
else
{
		header("Location: ../index.php?login=error");
		exit();
}