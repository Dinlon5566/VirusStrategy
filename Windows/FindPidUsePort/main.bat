@echo off
setlocal enabledelayedexpansion
pause

rem 使用 netstat -ano 過濾本地開放的 PORT
for /f "tokens=2,5" %%a in ('netstat -ano ^| find "[::]:"') do (
    set "full_address=%%a"
    set "pid=%%b"
    rem 提取PORT號
    for /f "tokens=1,2 delims=]:" %%c in ("!full_address!") do (
        set "port=%%d"
    )
    echo PORT: !port! PID: !pid!
    rem 查詢使用該PID的進程名稱
    for /f "tokens=*" %%e in ('tasklist /fi "PID eq !pid!" /fo table /nh') do (
        echo Process: %%e
    )
)

endlocal
pause
