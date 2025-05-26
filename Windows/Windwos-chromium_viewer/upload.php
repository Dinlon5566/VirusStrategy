<?php

declare(strict_types=1);

date_default_timezone_set('Asia/Taipei');

$uploadDir = __DIR__ . '/uploads';
if (!is_dir($uploadDir)) {
    mkdir($uploadDir, 0755, true);
}

/**
 * Send JSON response and exit.
 *
 * @param int    $statusCode HTTP status code
 * @param string $message    Response message
 */
function respond(int $statusCode, string $message): never
{
    http_response_code($statusCode);
    header('Content-Type: application/json; charset=utf-8');
    echo json_encode(
        ['status' => $statusCode, 'message' => $message],
        JSON_UNESCAPED_UNICODE
    );
    exit;
}

// Only allow POST requests
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    respond(405, 'Method Not Allowed: only POST is accepted');
}

// 1. Handle multipart/form-data upload (field name “file”)
if (isset($_FILES['file']) && $_FILES['file']['error'] === UPLOAD_ERR_OK) {
    $originalName = $_FILES['file']['name'];
    $baseName = pathinfo($originalName, PATHINFO_FILENAME);
    $timestamp = date('Ymd_His');
    $newName = sprintf('%s_%s.json', $baseName, $timestamp);
    $destination = "$uploadDir/$newName";

    if (move_uploaded_file($_FILES['file']['tmp_name'], $destination)) {
        respond(200, "File saved as {$newName}");
    }

    respond(500, 'Failed to move uploaded file');
}

// 2. Handle raw JSON payload
$rawInput = file_get_contents('php://input');
if (empty($rawInput)) {
    respond(400, 'No data received');
}

if (json_decode($rawInput, true) === null && json_last_error() !== JSON_ERROR_NONE) {
    respond(400, 'Invalid JSON payload');
}

$timestamp = date('Ymd_His');
$newName = "payload_{$timestamp}.json";
$destination = "$uploadDir/$newName";

if (file_put_contents($destination, $rawInput) !== false) {
    respond(200, "JSON payload saved as {$newName}");
}

respond(500, 'Failed to write JSON file');
