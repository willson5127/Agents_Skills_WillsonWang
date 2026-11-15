# 第 1 章　讓第一支 C 程式執行

目標是建立檔案、編譯、執行，並區分這三件事。沒有程式先備。你可以把原始碼想成食譜，編譯器把它翻譯成電腦能執行的形式；但這只是教學比喻，真正的編譯流程還包含前處理、產生物件檔與連結。

## 1.1 工具各做什麼

編輯器儲存文字，編譯器檢查語法與產生機器碼，連結器把需要的定義組合起來，終端機讓你下指令。安裝編輯器不會自動取得 C 編譯器。C 也不是 C++ 的另一個拼法：請用 `.c` 與 `gcc` 或 `clang`，本書不用 `g++`。

Windows x64 新手可以依 [MSYS2 Getting Started 的 Installation 段落](https://www.msys2.org/#installation) 取得並安裝工具，再開啟 **MSYS2 UCRT64** 終端。若是 ARM64 電腦，應依官方 ARM64 說明選工具鏈，不直接照抄 x64 套件。以下安裝命令供讀者自行選擇執行，本次沒有代為安裝。

```sh
pacman -S mingw-w64-ucrt-x86_64-gcc
gcc --version
```

如需更新現有 MSYS2，先依官方更新流程完成再安裝，避免只更新部分套件。不要從隨機教學下載舊版工具壓縮檔。UCRT64 是一組編譯器、函式庫與環境設定，不只是終端機的配色。[MSYS2 Environments](https://www.msys2.org/docs/environments/) 說明各環境差異。

## 1.2 第一個完整程式

先在 UCRT64 終端建立練習資料夾：`mkdir -p ~/c-learning`，再用 `cd ~/c-learning` 進入。`~` 是這個環境的使用者家目錄，`pwd` 顯示目前位置，`ls` 列出檔案；使用編輯器開啟這個資料夾。要開啟現有 Windows 工作區，本例環境可用 `cd /c/AISpace/Codex_Fig`，路徑有空白時加雙引號。後續命令都要在原始碼所在資料夾執行。

以純文字儲存為 `hello.c`，注意不是 `hello.c.txt`。

```c
#include <stdio.h>

int main(void)
{
    puts("Hello, C!");
    return 0;
}
```

`#include` 讓編譯器看見標準 I/O 宣告；`main` 是本書環境的入口。`int` 表示回傳整數，參數位置的 `void` 表示不接收參數。大括號界定函式內容；每個這裡的敘述以分號結束。`puts` 輸出字串並加換行；雙引號是文字界線，不會出現在輸出。`return 0` 表示成功結束。先能把每一行連回用途即可，函式與字串會在後文深入。

在檔案所在資料夾執行：

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -g hello.c -o hello.exe
./hello.exe
```

推演輸出是 `Hello, C!` 後接換行，範例未實測。`-o` 指定輸出檔，`-g` 保留除錯資訊，警告選項協助發現可疑寫法；沒有訊息通常表示編譯成功。明確指定 C17，避免工具預設方言變更影響練習。[GCC C Dialect Options](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html) 說明 `-std` 與 GNU 擴充的區別。

Linux／macOS 已有 GCC 或 Clang 時可用同一份原始碼，輸出名稱改為 `hello`，執行 `./hello`。PowerShell 若要執行已建立的 Windows 程式則用 `.\hello.exe`。本書後續 shell 命令以 MSYS2 的 Bash 格式呈現。

## 1.3 錯誤發生在哪裡

`gcc: command not found`：先確認開的是 UCRT64、套件安裝成功，再看 `gcc --version`。`hello.c: No such file`：檢查目前目錄與檔名。刪掉分號造成編譯錯誤時，從第一個診斷及其前一行查起，後面可能只是連帶錯誤。`undefined reference` 通常發生在連結階段，例如有宣告卻未提供函式定義。執行後結果不對則是另一類問題，編譯成功不保證邏輯正確。

不要把舊執行檔的成功誤認為新程式成功：每次修改後先確認編譯命令成功，再執行。

## 1.4 練習與解答

1. 基礎：改成顯示 `I can compile C.`。解答：只改 `puts` 雙引號內文字；成功判準是輸出完全相同且換行。
2. 應用：顯示兩行，第一行 `Name: Learner`，第二行 `Lesson: 1`。解答：依序呼叫兩次 `puts`；換行由各次呼叫提供。
3. 挑戰：刪去一個分號，記錄編譯器第一個診斷，再補回。解答：診斷位置可能在下一行；應能說明「發現問題的位置」不一定是「造成問題的位置」。修正後重新編譯才算完成。

你已建立編寫 → 編譯 → 執行的循環。下一步：[變數與型別](02-types.md)。

## 本章查閱與延伸

- [MSYS2 安裝步驟](https://www.msys2.org/#installation)：工具取得、UCRT64 GCC 套件與安裝後確認。
- [MSYS2 環境](https://www.msys2.org/docs/environments/)：讀 Overview，理解環境不可隨意混用。
- [GCC 方言選項](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html)：讀 `-std`、C17 與 hosted 條目。
