#!/usr/bin/php-cgi
<?php
// Set cookies
header("status: 200");
header("Content-Type: text/html");
setcookie('username', 'Ayman', time() + 3600); // Cookie expires in 1 hour
setcookie('session_id', '123456789', time() + 3600);
?>

<html>
<head><title>Set Cookies</title></head>
<body>
    <h1>Cookie Set</h1>
    <p>Cookies have been set. Reload the page to see the cookies in action.</p>
</body>
</html>
