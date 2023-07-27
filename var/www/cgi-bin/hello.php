#!/usr/bin/php-cgi
<?php
    header("status: 200");
    header("Content-Type: text/html");
    header("set-cookie: foo=bar");
    echo "<h1>Hello, CGI World!</h1>";
?>
