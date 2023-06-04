<?php
    $fileContent = '';

    // check if the request method is GET or POST
    if ($_SERVER['REQUEST_METHOD'] == 'GET' && isset($_GET['file'])) {
        $fileContent = $_GET['file'];
    } 
    elseif ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['file'])) {
        $fileContent = $_POST['file'];
    }

    if (!empty($fileContent)) {
        // check if the content is base64 encoded
        if (base64_encode(base64_decode($fileContent, true)) === $fileContent){
            // decode the content
            $decodedContent = base64_decode($fileContent);

            // save the content with timestamp to filename
            $timestamp = time();
            $filename = $timestamp . '.txt';
            file_put_contents($filename, $decodedContent);

            // DEBUG 用
            echo "File {$filename} created.";
        } else {
            echo "Invalid base64 content.";
        }
    } else {
        echo "No 'file' parameter in request.";
    }
?>
