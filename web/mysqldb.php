<?php

class Ferment
{
	private $server = "localhost";
	private $username = "fermenter";
	private $password = "Scrub12";
	private $db = "Ferment";
	private $mysql;
	private $isOpen = false;
	
	function __construct()
	{
		$this->isOpen = false;
		
		date_default_timezone_set('America/Chicago');	
		$this->mysql = new MySQLi($this->server, $this->username, $this->password, $this->db);
		if ($this->mysql->connect_errno) {
			die ("Error opening mysql connection".$this->mysql->connect_error);
			$this->isOpen = false;
		}
		else 
			$this->isOpen = true;
	}
	
	public function init()
	{
		return $this->isOpen;
	}
	
	public function getLastError()
	{
		if ($this->mysql->connect_error)
			return $this->mysql->connect_error;
		
		return "No Error";
	}
	
	public function getResultSet($count)
	{
		$results = array();
		$query = "SELECT date,value FROM FermentTemps ORDER BY date DESC LIMIT " . $count;
		$result = $this->mysql->query($query);
		if ($result->num_rows > 0) {
			while ($row = $result->fetch_assoc()) {
				if ($row['value'] > 85 || $row['value'] < 60)
					continue;
				$assoc = $row['date'];
				$results[$assoc] = $row['value'];
			}
		}
		return array_reverse($results);
//		return $results;
	}
}
?>
