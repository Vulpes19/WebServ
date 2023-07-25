#!/usr/bin/php-cgi
<?php
header("status: 200");
header("Content-Type: text/html");
// Read the current count from the cookie or set it to 0 if not present
$count = isset($_COOKIE['count']) ? (int)$_COOKIE['count'] : 0;

// Increment the count
$count++;

// Set the updated count in the cookie
setcookie('count', $count, time() + 3600); // Cookie expires in 1 hour
?>

<html>
<head><title>Incrementing Number</title></head>
<body>
    <h1>Incrementing Number</h1>
    <p>Current Count: <?php echo $count; ?></p>
    <p>Reload the page to increment the number.</p>
</body>
</html>
