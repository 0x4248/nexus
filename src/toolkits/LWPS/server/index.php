<!-- make a var with Hello world and make a H1 Using var -->
<php? $greeting = "Hello, World!"; ?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0
">
    <title>LWPS Server Index</title>    
    <style>
        body {
            font-family: monospace, monospace;
            margin: 40px;
            background-color: #1e1e1e;
            color: #d4d4d4
        }
        h1{ color: #6fff7dff; }
        p {
            font-size: 18px;
        }
    </style>
</head>
<body>
    <h1>Congratulations!</h1>
    <p><?php echo "If you see this text then it means PHP is working correctly"; ?></p>
    <ul>
        <li>PHP Version:<code> <?php echo phpversion(); ?></code></li>
        <li>Server Software:<code> <?php echo $_SERVER['SERVER_SOFTWARE']; ?></code></li>
    </ul>   
    <h2>README File</h2>
    <pre>
<?php
    $readme_path = __DIR__ . '/../README';
    if (file_exists($readme_path)) {
        echo htmlspecialchars(file_get_contents($readme_path));
    } else {
        echo "README file not found.";
    }
?>
    </pre>
</body>
</html>
