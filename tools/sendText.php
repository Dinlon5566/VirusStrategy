<!DOCTYPE html>
<!-- 這東西就只是虛擬機共用資料夾模組壞掉，暫時用的 -->
<html>
<head>
    <title>傳資料</title>
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css">
    <style>
        #outputArea {
            white-space: pre-wrap;
        }
    </style>
</head>
<body>

<div class="container">
    <h1 class="mt-4 mb-4">虛擬機資料傳輸</h1>

    <form action="" method="POST" class="mb-4">
        <div class="form-group">
            <label for="dataInput">輸入區:</label><br>
            <textarea class="form-control" name="dataInput" id="dataInput" rows="4"></textarea><br>
        </div>
        <button type="submit" class="btn btn-primary">傳送</button>
    </form>

    <h2 class="mb-2">輸出區:</h2>
    <div class="alert alert-info" id="outputArea">...</div>

    <h2 class="mt-4 mb-2">歷史區</h2>
    <table class="table table-striped">
        <thead>
            <tr>
                <th>名稱</th>
                <th>簡述</th>
                <th>建立時間</th>
                <th>刪除</th>
            </tr>
        </thead>
        <tbody>
        <?php
        $files = glob("*.txt");
        foreach($files as $file) {
            $time = date("Y-m-d H:i:s", filemtime($file));
            $data = file_get_contents($file);
            $brief = mb_substr($data, 0, 20);
            echo "<tr>
                    <td><a href=\"?file=$file\">{$file}</a></td>
                    <td>{$brief}</td>
                    <td>{$time}</td>
                    <td><a href=\"?delete=$file\" class=\"btn btn-danger btn-sm\">刪除</a></td>
                  </tr>";
        }
        ?>
        </tbody>
    </table>
</div>

<?php
if(isset($_POST['dataInput'])) {
    $data = $_POST['dataInput'];
    $id = uniqid();
    file_put_contents("{$id}.txt", $data);
    echo "<script>document.getElementById('outputArea').innerText = `{$data}`;</script>";
}
if(isset($_GET['file'])) {
    $data = file_get_contents($_GET['file']);
    echo "<script>document.getElementById('outputArea').innerText = `{$data}`;</script>";
}
if(isset($_GET['delete'])) {
    unlink($_GET['delete']);
    header("Location: ".$_SERVER['PHP_SELF']);
    exit;
}
?>

<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js"></script>
</body>
</html>
