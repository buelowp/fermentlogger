<html>
<head>
<title>Ferment graph</title>
</head>
<body>

<?php
	require_once "mysqldb.php";
	require_once "ezc/Base/ezc_bootstrap.php";

	$db = new Ferment();
	$chart = new ezcGraphLineChart();
	$chart->title = 'Last ~100 Fermentation Temp Values';

	$results = $db->getResultSet(100);

	$chart->data['Temperatures'] = new ezcGraphArrayDataSet($results);
	$chart->render(1500, 500, 'line_chart.svg' );
?>
</body>
<img src="line_chart.svg">
</html>

