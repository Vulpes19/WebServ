#!/usr/bin/php-cgi
<?php
    header("Content-Type: text/html");
    echo "<h2>POST Data:</h2>";
    echo "<pre>";
    print_r($_POST);
    echo "</pre>";
?>
