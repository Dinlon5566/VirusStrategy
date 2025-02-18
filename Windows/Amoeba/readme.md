# Amoeba.exe
這個玩具是為了使程式在每次執行後不會有固定的hash值而製作。使用了 Self-mutation 、Self-deletion 來規避hash掃描 。

這程式有一個 32byte 的變異空間，每次執行時會將本空間進行一次 SHA256轉換  

變異區的初始值是:

```
static const char embedded_mutation_data[] =
MUTATION_START_MARKER
"\x01\x02\x03\x04\x05\x06\x07\x08"
"\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10"
"\x11\x12\x13\x14\x15\x16\x17\x18"
"\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20"
MUTATION_END_MARKER; (使用時需要改掉前後綴與特徵)
```
隨後建立一個系統隱藏資料夾，並將延遲刪除腳本放在這邊，時間到了將會把舊版的 Amoeba.exe 替換成新的。
```bash
timeout /t 3 /nobreak >nul
del /f /q "%~dp0\..\Amoeba.exe"
move /y "%~dp0\Amoeba.exe" "%~dp0\..\Amoeba.exe"
del "%~f0"
```

