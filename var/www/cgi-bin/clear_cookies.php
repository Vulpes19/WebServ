<?php
// PHP code to clear cookies on the server-side
foreach ($_COOKIE as $name => $value) {
    // Set the expiration time to the past (1 second ago)
    setcookie($name, '', time() - 1, '/');
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Clear Cookies</title>
</head>
<body>
    <h1>Cookies Cleared</h1>
    <p>Click the button below to clear cookies from your browser.</p>
    <button onclick="clearCookies()">Clear Cookies</button>

    <script>
        function clearCookies() {
            // JavaScript code to clear cookies on the client-side
            var cookies = document.cookie.split("; ");
            for (var i = 0; i < cookies.length; i++) {
                var cookie = cookies[i];
                var eqPos = cookie.indexOf("=");
                var name = eqPos > -1 ? cookie.substr(0, eqPos) : cookie;
                document.cookie = name + "=;expires=Thu, 01 Jan 1970 00:00:00 GMT;path=/";
            }

            // Redirect back to the main page or any desired location
            window.location.href = '';
        }
    </script>
</body>
</html>
