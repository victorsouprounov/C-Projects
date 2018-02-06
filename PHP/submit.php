<?php

session_start();

if (isset($_POST['submit']))
{
	include_once 'dbh.inc.php';

	$tupos = mysqli_real_escape_string($conn, $_POST['tupos']);
	$ame = mysqli_real_escape_string($conn, $_POST['ame']);
	$afm = mysqli_real_escape_string($conn, $_POST['afm']);
	$imer_apo = mysqli_real_escape_string($conn, $_POST['imer_apo']);
	$imer_eos = mysqli_real_escape_string($conn, $_POST['imer_eos']);
	$onoma = mysqli_real_escape_string($conn, $_POST['onoma']);
	$eponimo = mysqli_real_escape_string($conn, $_POST['eponimo']);
	$onoma_pat = mysqli_real_escape_string($conn, $_POST['onoma_pat']);
	$onoma_mit = mysqli_real_escape_string($conn, $_POST['onoma_mit']);
	$am_asf = mysqli_real_escape_string($conn, $_POST['am_asf']);
	$imer_gen = mysqli_real_escape_string($conn, $_POST['imer_gen']);
	$apasx_apo = mysqli_real_escape_string($conn, $_POST['apasx_apo']);
	$apasx_eos = mysqli_real_escape_string($conn, $_POST['apasx_eos']);
	$imer_asf = mysqli_real_escape_string($conn, $_POST['imer_asf']);
	$eisfores_asf = mysqli_real_escape_string($conn, $_POST['eisfores_asf']);
	$imeromisthio = mysqli_real_escape_string($conn, $_POST['imeromisthio']);
	$apodoxes = mysqli_real_escape_string($conn, $_POST['apodoxes']);
	$user_id = $_SESSION['u_id'];

	$sql ="INSERT INTO statements 
		(user_id,tupos,ame,afm,imer_apo,imer_eos,onoma,eponimo,onoma_pat,onoma_mit,am_asf,imer_gen,apasx_apo,apasx_eos,imer_asf,eisfores_asf,imeromisthio,apodoxes) 
		VALUES 
		('$user_id','$tupos','$ame','$afm','$imer_apo','$imer_eos','$onoma','$eponimo','$onoma_pat','$onoma_mit','$am_asf','$imer_gen','$apasx_apo','$apasx_eos','$imer_asf','$eisfores_asf','$imeromisthio','$apodoxes'); ";
		mysqli_query($conn, $sql);

		header("Location: ../submit.php?signup=success");
		exit();
}
else
{
	header("Location: ../submit.php");
	exit();
}