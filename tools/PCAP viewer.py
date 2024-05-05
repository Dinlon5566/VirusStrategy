import tkinter as tk
from tkinter import filedialog, messagebox, Toplevel, Scrollbar, StringVar, Text, END
from tkinter.ttk import Button, Label, Entry, Treeview, Scrollbar, Style
import threading
import pyshark
import asyncio
import os
import io
import contextlib

pcap = None  # 全局變量，用於存儲當前的 PCAP 封包對象
filtered_packets = []


def load_pcap():
    file_path = filedialog.askopenfilename(initialdir=os.getcwd(), title="選擇 PCAP 檔案",
                                           filetypes=[("PCAP 檔案", "*.pcap"), ("所有檔案", "*.*")])
    if file_path:
        loading_label.config(text="檔案載入中...")
        loading_thread = threading.Thread(
            target=load_pcap_in_background, args=(file_path,))
        loading_thread.start()


def load_pcap_in_background(file_path):
    global pcap
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    try:
        pcap = list(pyshark.FileCapture(file_path, keep_packets=False))
        file_label.config(
            text=f"檔案名稱: {os.path.basename(file_path)}，封包數量: {len(pcap)}")
        packet_tree.delete(*packet_tree.get_children())  # 清除舊的封包列表
        loading_label.config(text="")  # 清除載入中提示
    except Exception as e:
        messagebox.showerror("Error", str(e))
        loading_label.config(text="")  # 清除載入中提示


def apply_filter():
    global filtered_packets
    if pcap is None:
        messagebox.showwarning("No PCAP", "請先選擇 PCAP 檔案！")
        return

    filter_str = filter_var.get()
    try:
        # 過濾符合規則的封包
        filtered_packets = [
            pkt for pkt in pcap if filter_str.lower() in pkt.highest_layer.lower()]
        packet_tree.delete(*packet_tree.get_children())  # 清除舊的封包列表
        for i, packet in enumerate(filtered_packets):
            try:
                packet_num = i + 1
                length = packet.length
                src_ip = packet.ip.src if hasattr(packet, 'ip') else "N/A"
                dst_ip = packet.ip.dst if hasattr(packet, 'ip') else "N/A"
                protocol = packet.highest_layer
                packet_tree.insert("", "end", values=(
                    packet_num, length, src_ip, dst_ip, protocol))
            except AttributeError as e:
                packet_tree.insert("", "end", values=(
                    i + 1, "N/A", "N/A", "N/A", f"Error: {str(e)}"))
    except Exception as e:
        messagebox.showerror("Error", str(e))


def show_packet_details(event):
    selected_item = packet_tree.selection()
    if selected_item:
        index = int(packet_tree.item(selected_item, "values")[0]) - 1
        packet = filtered_packets[index]

        details_window = Toplevel(app)
        details_window.title(f"封包詳情 - Packet {index + 1}")
        details_window.geometry("800x500")  # 設置長條形視窗大小

        # 創建滾動條和文本框
        scrollbar = Scrollbar(details_window)
        details_text = Text(details_window, wrap='word',
                            yscrollcommand=scrollbar.set)
        scrollbar.config(command=details_text.yview)
        scrollbar.pack(side="right", fill="y")
        details_text.pack(side="left", fill="both", expand=True)

        # 使用 io.StringIO 和 contextlib.redirect_stdout 來捕獲 packet.show() 的輸出
        packet_output = io.StringIO()
        with contextlib.redirect_stdout(packet_output):
            packet.show()

        # 顯示封包資訊
        details_text.insert(END, packet_output.getvalue())
        details_text.config(state='disabled')  # 防止用戶編輯內容


# 創建主窗口
app = tk.Tk()
app.title("PCAP Viewer")
app.geometry("800x600")
app.configure(bg="#4b3621")  # 咖啡色背景

# 使用 ttk.Style 進行現代化風格設計
style = Style()
style.theme_use('clam')
style.configure('TLabel', background="#4b3621",
                foreground="white", font=("Arial", 12))
style.configure('TButton', background="#6b4226",
                foreground="white", font=("Arial", 12), padding=5)
style.configure('TEntry', foreground="black", font=("Arial", 12), padding=5)
style.configure('Treeview', background="#6b4226",
                fieldbackground="#6b4226", foreground="white", font=("Arial", 10))
style.configure('Treeview.Heading', background="#4b3621",
                foreground="white", font=("Arial", 12))

# 檔案選擇按鈕
file_label = Label(app, text="尚未選擇 PCAP 檔案", style='TLabel')
file_label.pack(pady=10)
load_button = Button(app, text="選擇 PCAP 檔案",
                     command=load_pcap, style='TButton')
load_button.pack(pady=5)

# 載入狀態標籤
loading_label = Label(app, text="", style='TLabel')
loading_label.pack()

# 過濾規則
filter_var = StringVar()
filter_label = Label(
    app, text="輸入過濾規則（協議名稱，例如: http, tcp, dns）:", style='TLabel')
filter_label.pack(pady=5)
filter_entry = Entry(app, textvariable=filter_var, style='TEntry')
filter_entry.pack(fill="x", padx=20)
filter_button = Button(app, text="應用過濾", command=apply_filter, style='TButton')
filter_button.pack(pady=10)

# 封包列表表格
columns = ("編號", "長度", "來源 IP", "目的 IP", "協議")
packet_tree = Treeview(app, columns=columns, show="headings")
packet_tree.heading("編號", text="封包編號")
packet_tree.heading("長度", text="長度")
packet_tree.heading("來源 IP", text="來源 IP")
packet_tree.heading("目的 IP", text="目的 IP")
packet_tree.heading("協議", text="協議")
packet_tree.bind("<Double-1>", show_packet_details)

# 加入滾動條
scrollbar = Scrollbar(app, orient="vertical", command=packet_tree.yview)
packet_tree.configure(yscrollcommand=scrollbar.set)
packet_tree.pack(side="left", fill="both", expand=True, padx=20, pady=10)
scrollbar.pack(side="right", fill="y")

app.mainloop()
