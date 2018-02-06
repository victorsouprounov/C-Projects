<?php

session_start();

if (isset($_POST['submit']))
{
	include_once 'dbh.inc.php';

	$category = mysqli_real_escape_string($conn, $_POST['category']);

	$onoma = "";
	if(trim($_POST['onoma']) != "")
	{
		$onoma = mysqli_real_escape_string($conn, $_POST['onoma']);
	}
	else if(trim($_POST['onoma1']) != "")
	{
		$onoma = mysqli_real_escape_string($conn, $_POST['onoma1']);
	}

	$eponimo = "";
	if(trim($_POST['eponimo']) != "")
	{
		$eponimo = mysqli_real_escape_string($conn, $_POST['eponimo']);
	}
	else if(trim($_POST['eponimo1']) != "")
	{
		$eponimo = mysqli_real_escape_string($conn, $_POST['eponimo1']);
	}

	$taut = "";
	if(trim($_POST['taut']) != "")
	{
		$taut = mysqli_real_escape_string($conn, $_POST['taut']);
	}
	else if(trim($_POST['taut1']) != "")
	{
		$taut = mysqli_real_escape_string($conn, $_POST['taut1']);
	}

	$afm = "";
	if(trim($_POST['afm']) != "")
	{
		$afm = mysqli_real_escape_string($conn, $_POST['afm']);
	}
	else if(trim($_POST['afm1']) != "")
	{
		$afm = mysqli_real_escape_string($conn, $_POST['afm1']);
	}

	$am = "";
	if(trim($_POST['am']) != "")
	{
		$am = mysqli_real_escape_string($conn, $_POST['am']);
	}
	else if(trim($_POST['am1']) != "")
	{
		$am = mysqli_real_escape_string($conn, $_POST['am1']);
	}

	$doi = mysqli_real_escape_string($conn, $_POST['doi']);
	$ame = mysqli_real_escape_string($conn, $_POST['ame']);
	$odos = mysqli_real_escape_string($conn, $_POST['odos']);
	$arithmos = mysqli_real_escape_string($conn, $_POST['arithmos']);
	$poli = mysqli_real_escape_string($conn, $_POST['poli']);
	$til = mysqli_real_escape_string($conn, $_POST['til']);
	$amdias = mysqli_real_escape_string($conn, $_POST['amdias']);
	$email = mysqli_real_escape_string($conn, $_POST['email']);
	$uid = mysqli_real_escape_string($conn, $_POST['uid']);
	$pwd = mysqli_real_escape_string($conn, $_POST['pwd']);

	$sql = " SELECT * FROM users WHERE user_uid='$uid' AND user_pwd='$pwd' ";
	$result = mysqli_query($conn,$sql);
	$resultCheck = mysqli_num_rows($result);

	if($resultCheck  > 0)
	{
		header("Location: ../registration.php?signup=taken");
			exit();
	}
	else
	{
		$sql ="INSERT INTO users (user_type,user_first,user_last,user_email,user_uid,user_pwd,taut,afm,doi,ame,am,odos,arithmos,poli,til,amdias) 
		VALUES 
		('$category','$onoma','$eponimo','$email','$uid','$pwd','$taut','$afm','$doi','$ame','$am','$odos','$arithmos','$poli','$til','$amdias'); ";
		mysqli_query($conn, $sql);

		$sql = " SELECT * FROM users WHERE user_uid='$uid' AND user_pwd='$pwd' ";
		$result = mysqli_query($conn,$sql);
		$row = mysqli_fetch_assoc($result);

		$_SESSION['u_type'] = $row['user_type'];
		$_SESSION['u_id'] = $row['user_id'];
		$_SESSION['u_first'] = $row['user_first'];
		$_SESSION['u_last'] = $row['user_last'];
		$_SESSION['u_email'] = $row['user_email'];
		$_SESSION['u_uid'] = $row['user_uid'];
		$_SESSION['u_pwd'] = $row['user_pwd'];

		header("Location: ../index.php?signup=success");
		exit();
	}
}
else
{
	header("Location: ../registration.php");
	exit();
}