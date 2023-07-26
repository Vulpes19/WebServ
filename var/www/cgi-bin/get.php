<?php
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    header("Content-Type: text/plain");
    $name = $_GET['name'] ?? 'p';
    $email = $_GET['email'] ?? 'p';
    echo "Name: $name\n";
    echo "Email: $email\n";
}
?>