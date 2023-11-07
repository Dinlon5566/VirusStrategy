<?php
// 警告：以下代碼存在 XSS漏洞，絕對不要在生產環境中使用。

if (isset($_POST['submit'])) {
    $name = $_POST['name'];

    echo "Hello " . $name . "!";
}
?>

<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>XSS Vulnerable Page</title>
</head>
<body>
    <h2>Input your name:</h2>
    <form method="POST" action="">
        <input type="text" name="name" />
        <input type="submit" name="submit" value="提交" />
    </form>
</body>
</html>
