#!/usr/bin/php-cgi
<?php
    header("Content-Type: text/html");
    echo "<h2>Query Parameters:</h2>";
    echo "<pre>";
    print_r($_GET);
    echo "</pre>";
?>
