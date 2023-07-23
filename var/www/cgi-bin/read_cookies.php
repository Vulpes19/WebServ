#!/usr/bin/php-cgi
<?php
// Read cookies
header("status: 200");
header("Content-Type: text/html");
$username = isset($_COOKIE['username']) ? $_COOKIE['username'] : 'N/A';
$session_id = isset($_COOKIE['session_id']) ? $_COOKIE['session_id'] : 'N/A';
?>

<html>
<head><title>Read Cookies</title></head>
<body>
    <h1>Cookie Test</h1>
    <p>Username: <?php echo $username; ?></p>
    <p>Session ID: <?php echo $session_id; ?></p>
</body>
</html>
