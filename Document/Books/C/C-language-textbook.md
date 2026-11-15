# 從零寫出可靠程式：C 語言教科書

原創繁體中文書稿 · 2026-11-15 · C17 主體／C23 延伸

[閱讀入口、範圍與限制](README.md) · [分章目錄](README.md#目錄)

本合訂版收錄學習地圖、14 章正文、術語、速查與來源。程式未在此環境編譯執行，輸出為人工推演；完整適用範圍與未涵蓋分支見閱讀入口。


---

# 學習地圖

先備關係主要是 1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 9 → 10 → 11 → 12 → 13 → 14。第一次可先把第 14 章當成探索章，熟悉實作後再回讀。

| 階段 | 完成後能做什麼 | 先備章節 | 章節 | 支持來源（閱讀範圍見來源表） | 驗收任務 |
| --- | --- | --- | --- | --- | --- |
| 零基礎 | 由原始碼產生程式、解釋輸出 | 無 | 1–2 | MSYS2 安裝、GCC 方言、Microsoft 型別轉換 | 顯示平均值並解釋整數除法 |
| 基礎邏輯 | 用條件與迴圈處理資料、切成函式 | 1–2 | 3–4 | Microsoft if／for、Aspnes Functions | 自寫統計與有終止條件的遞迴 |
| 核心資料 | 分辨陣列、字串、位址與記錄 | 1–4 | 5–7 | Microsoft 陣列／指標／結構、fgets | 安全遍歷、交換變數、表示多種資料 |
| 資源管理 | 配置、擴充、釋放記憶體及分檔 | 5–7 | 8–9 | realloc、CERT MEM、include 文件 | 擴充陣列失敗仍保留原資料、三檔建置 |
| 實務整合 | 解析輸入、判讀錯誤、定位缺陷 | 1–9 | 10–11 | strtol、fopen／fread／fclose、ASan／UBSan | 拒絕無效成績，說明測試覆蓋的限制 |
| 進階設計 | 選資料結構、分析時間與空間 | 5–11 | 12 | qsort、Aspnes 回呼段落 | 寫比較器、推導成長策略成本 |
| 獨立作品 | 從檔案載入、排序、統計與清理 | 1–12 | 13 | fgets、strtol、realloc、qsort | 正常／空檔／壞資料均有明確結果 |
| 專家議題起點 | 區分行為種類，推理別名與同步 | 1–13 | 14 | N1570 §6.5、§5.1.2.4；N3096 §7.20 | 解釋型別轉換不能修正 UB，檢查發布協定 |

各章提供基礎、應用及挑戰題與參考解答。成功判準包括結果正確、邊界合理、能用自己的話解釋；單看「程式沒有當掉」不能算通過。

## 主要分支與後續路線

| 分支 | 本書程度 | 後續先備與探索任務 |
| --- | --- | --- |
| 可攜式應用與標準函式庫 | 核心實作 | 以標準索引查 math、time、locale，先核對前置條件 |
| 資料結構 | 動態陣列、單向鏈結、排序、搜尋 | 熟悉不變量後實作樹、雜湊表及失敗注入 |
| 系統與嵌入式 | hosted／freestanding 差異、位元觀念 | 學計算機組織、OS、平台 ABI；再讀 POSIX 或晶片手冊 |
| 並行 | 原子與 happens-before 的基本推理 | 先學執行緒／互斥鎖，再閱讀記憶體模型；本書不教無鎖容器 |
| 編譯器與語意 | UB、別名、最佳化前提 | 學中介表示、資料流分析，對照標準與編譯器輸出 |
| 數值計算 | 整數邊界、平均值與浮點近似 | 學誤差分析、穩定性，再研究浮點環境與數值函式庫 |

遇到卡點先記錄「最小輸入、預期、實際、工具版本」，再回到對應章節。不要同時更換編譯器、修改輸入、改動演算法，否則難以判斷哪一步產生影響。


---

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

你已建立編寫 → 編譯 → 執行的循環。下一步：[變數與型別](chapters/02-types.md)。

## 本章查閱與延伸

- [MSYS2 安裝步驟](https://www.msys2.org/#installation)：工具取得、UCRT64 GCC 套件與安裝後確認。
- [MSYS2 環境](https://www.msys2.org/docs/environments/)：讀 Overview，理解環境不可隨意混用。
- [GCC 方言選項](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html)：讀 `-std`、C17 與 hosted 條目。


---

# 第 2 章　變數、型別與運算

先備：[第 1 章](chapters/01-start.md)。目標是表示資料、預測運算的型別，寫出正確平均值。假設兩次成績是 80 與 95，若電腦顯示平均 87，你應先問「它用了什麼型別算除法」，而不是改顯示的小數位數。

## 2.1 名稱、物件與值

`int score = 80;` 定義一個具名物件，型別是 `int`，初始值是 80。變數可在後續被賦值：`score = 90;`。數學等號是關係，C 的單等號是把右邊結果存進左邊；比較是否相等用 `==`。區域變數要先初始化再讀，不要假定自動變數會是零。

`int` 表示一般整數，`unsigned int` 表示非負範圍，`double` 表示浮點近似值，`char` 是字元與位元組操作的基本型別。`size_t` 用於物件大小與陣列數量。C 沒有保證 `int` 固定 32 位元；用 `<limits.h>` 查範圍，用 `sizeof` 查佔用的 C 位元組數。`sizeof(char)` 為 1，但一個 C 位元組的位元數由 `CHAR_BIT` 表示，不要把兩者概念混成同一件事。

## 2.2 完整示範與推導

```c
#include <stdio.h>

int main(void)
{
    int first = 80;
    int second = 95;
    int total = first + second;
    double wrong = total / 2;
    double mean = total / 2.0;
    printf("total=%d wrong=%.1f mean=%.1f\n", total, wrong, mean);
    printf("int uses %zu C bytes\n", sizeof(int));
    return 0;
}
```

使用第 1 章命令，把檔名換成 `types.c`。未實測；第一行推演為 `total=175 wrong=87.0 mean=87.5`，第二行依平台不同。`total / 2` 先做整數除法，175 除 2 的整數結果是 87，再轉成 `double`；事後存進浮點變數救不回已丟掉的部分。`2.0` 讓這次除法採浮點運算。

`printf` 的格式必須配合引數：`%d` 對應 `int`，`%u` 對應 `unsigned int`，`%zu` 對應 `size_t`，`%f` 對應傳入的 `double`，`.1` 表示小數一位。格式不符可能導致未定義行為，不是可靠的型別轉換方式。

## 2.3 運算前會發生轉換

乘除先於加減，括號可明示順序。`7 % 3` 是餘數 1；整數除以零不合法。C17 的整數除法向零截斷，例如 `-7 / 3` 為 -2。邏輯運算會在下一章使用。

混合型別時，語言會先做提升與一般算術轉換。小整數型別常先提升為 `int`；而 signed／unsigned 混用可能把負值變成很大的非負值。因此不要靠「看起來比較大」推斷型別。[Microsoft Type Conversions (C)](https://learn.microsoft.com/en-us/cpp/c-language/type-conversions-c?view=msvc-170) 列出轉換發生的情況及整數提升。

顯式轉型 `(double)total / count` 是轉換值的要求，不是取消風險的符咒。轉成較小的型別前仍應先檢查範圍。無號運算有模數回繞規則；有號整數溢位則不能當作可靠的回繞。後續第 8、11 章會在加法／乘法**之前**檢查界線。

浮點數適合平均值，但許多十進位小數無法精確表示。金額若以最小貨幣單位的整數表示，要先定義幣別與四捨五入規則；不是把所有資料一律換成 `double`。

## 2.4 練習與解答

1. 基礎：`5 / 2`、`5 / 2.0`、`5 % 2` 分別為何？解答：2、2.5、1。必須說明前兩者運算時型別不同。
2. 應用：三筆 70、80、95 的平均。解答：`(70 + 80 + 95) / 3.0` 約為 81.6667；顯示兩位小數為 81.67。成功判準是除法前已進入浮點運算。
3. 挑戰：為何 `(double)(5 / 2)` 仍是 2.0？解答：括號內先算出整數 2，再轉型。可改成 `(double)5 / 2`。請用運算順序解釋，不只背結果。

下一步：[用條件與迴圈控制運算](chapters/03-control.md)。

## 本章查閱與延伸

- [Microsoft 型別轉換](https://learn.microsoft.com/en-us/cpp/c-language/type-conversions-c?view=msvc-170)：整數提升與轉換時機。
- [WG14 N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：查 §6.3 轉換、§6.5.5 除法與餘數；這是 C11 草案，閱讀範圍見來源總表。


---

# 第 3 章　條件、迴圈與不變量

先備：[第 2 章](chapters/02-types.md)。目標是讓程式依資料選路、重複工作，並用不變量解釋為什麼結果可信。統計成績時，你不會為每位學生手寫一套程式；你需要一個可重複的步驟。

## 3.1 把規則寫成條件

```c
if (score < 0 || score > 100) {
    puts("invalid");
} else if (score >= 60) {
    puts("pass");
} else {
    puts("retry");
}
```

這是放在函式內的片段，`score` 須已宣告。`||` 是「或」，`&&` 是「且」，`!` 是否定。零是假、非零是真；關係運算結果為 0 或 1。`&&`、`||` 會短路，例如 `count != 0 && total / count > 60`，左邊為假就不算右邊。別把 `&` 當作 `&&`，前者是位元操作。

[Microsoft if 文件](https://learn.microsoft.com/en-us/cpp/c-language/if-statement-c?view=msvc-170) 說明分支與 `else` 的配對。固定寫大括號能讓巢狀條件的意圖明確。`if (score = 60)` 是賦值，結果為非零，通常不是你想要的比較。`0 <= score <= 100` 也不是數學連鎖比較；應寫 `score >= 0 && score <= 100`。

## 3.2 完整的重複工作

```c
#include <stdio.h>

int main(void)
{
    int total = 0;
    int passes = 0;
    for (int i = 0; i < 5; ++i) {
        int score = 50 + i * 10;
        total += score;
        if (score >= 60) {
            ++passes;
        }
    }
    printf("total=%d passes=%d mean=%.1f\n", total, passes, total / 5.0);
    return 0;
}
```

未實測，推演輸出：`total=350 passes=4 mean=70.0`。`for` 先初始化 `i=0`，每次檢查 `i<5`，執行內容，再做 `++i`。`+=` 把右邊加進原值。這裡依序產生 50、60、70、80、90 五筆資料。

不要只追蹤最後結果。建立表格：進入第 0 次時 total=0；處理 50 後為 50；處理 60 後為 110。一般化後可得：**每次迴圈開始時，total 等於前 i 筆總和，passes 等於前 i 筆及格數。** 這就是不變量。

初始時前 0 筆總和為 0，成立。每次加進第 i 筆後，變成前 i+1 筆統計，仍成立。結束時 i=5，因此結果包含全部五筆。這三步是初始化、保持、終止推理；比「看起來跑五次」更能幫你抓出少一筆的錯誤。[Microsoft for 文件](https://learn.microsoft.com/en-us/cpp/c-language/for-statement-c?view=msvc-170) 可核對執行順序。

## 3.3 其他控制方式與界線

`while (condition)` 在每輪開始判斷；`do { ... } while (condition);` 至少執行一次。`break` 離開最近一層迴圈或 switch；`continue` 跳過本輪剩餘內容。`for` 的 continue 仍會走更新式，`while` 則要自己確保進度，否則可能無窮迴圈。

`switch` 適合整數／列舉的離散選項。每個 `case` 預設會接著執行下一個 case，直到遇到 break 等跳轉；刻意串接要註明理由。不適合拿來直接比較字串。

迴圈是否終止，要找一個受限制且朝界線移動的量。本例 i 每次加 1，從 0 移向 5。若資料量未受限，還需要考慮計數與總和溢位，不能沿用本例的小範圍假設。

## 3.4 練習與解答

1. 基礎：把及格門檻改成 70。解答：比較改成 `score >= 70`，及格數應為 3；其他統計不變。
2. 應用：總計 1 到 10。解答：total 從 0 開始，`for (int i=1; i<=10; ++i) total += i;`，答案 55。不變量是每輪開始前總計 1 到 i-1。
3. 挑戰：列出原範例改成 `i <= 5` 的影響。解答：多處理 100，總和 450、及格 5；若仍除以 5，平均錯成 90。成功判準是不只指出「多一次」，還指出衍生的分母錯誤。

下一步：[把可重複邏輯抽成函式](chapters/04-functions.md)。

## 本章查閱與延伸

- [Microsoft if](https://learn.microsoft.com/en-us/cpp/c-language/if-statement-c?view=msvc-170)：條件、分支、懸空 else。
- [Microsoft for](https://learn.microsoft.com/en-us/cpp/c-language/for-statement-c?view=msvc-170)：執行順序、break、continue；不變量證明是本書的教學推導。


---

# 第 4 章　函式、契約與遞迴

先備：[第 3 章](chapters/03-control.md)。目標是把規則封裝、區分參數與引數，並寫出會終止的遞迴。若及格判斷出現在十個地方，改門檻時很容易漏改；函式讓規則有單一入口。

## 4.1 函式是有介面的工作單位

```c
#include <stdio.h>

int is_pass(int score);

int main(void)
{
    int score = 75;
    printf("%d %d\n", is_pass(score), score);
    return 0;
}

int is_pass(int score)
{
    return score >= 60 && score <= 100;
}
```

未實測，推演為 `1 75`。最上面的 `int is_pass(int score);` 是宣告，告訴呼叫端型別；最後帶函式本體的是定義。呼叫中的 `score` 是引數，被呼叫函式中的名稱是參數。C 採**傳值**：參數取得值的副本，所以函式修改自己的整數參數不會改掉呼叫端變數。指標參數也同樣是值的副本，只是它能指向同一份資料，第 6 章再解釋。

[Aspnes 的 Functions 筆記](https://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Functions.html) 說明封裝與傳值。本書採 C17 原型寫法；沒有參數寫 `f(void)`，不要把舊式 `f()` 宣告當作相同意思。這份筆記是 2003–2012 年靜態教材，歷史寫法須留意版本。

## 4.2 先寫契約再寫本體

契約是呼叫者與函式的責任分工。`is_pass` 的本書契約是：輸入任何 int 都可，只有 60 到 100 回傳 1，其他回傳 0，沒有外部副作用。若無效值需要和不及格分開，單一布林結果就不足，應換成列舉狀態或額外結果；不能讓呼叫者猜。

作用域是名稱可見範圍，生命期是物件存在多久，兩者不同。函式內區塊的普通區域變數通常只活到該次區塊執行結束。檔案範圍的名稱也不代表應任意共享全域狀態；明確參數更容易測試。`static` 區域變數會保留狀態，呼叫結果因此可能依賴先前歷史，並行時更要小心。

## 4.3 遞迴：把問題縮小

遞迴是函式直接或間接呼叫自己。下面完整程式只接受 0 到 10，刻意限制範圍以排除溢位與過深呼叫。

```c
#include <stdio.h>

static unsigned int sum_to(unsigned int n)
{
    if (n == 0) {
        return 0;
    }
    return n + sum_to(n - 1);
}

int main(void)
{
    printf("%u\n", sum_to(10));
    return 0;
}
```

未實測，推演 55。`static` 放在此函式定義前表示此名稱只供本翻譯單元使用，和前述 static 區域變數的用途不同。遞迴基底是 n=0；每次 n 減少，最後必抵達基底。展開 n=3：3 + (2 + (1 + 0))，所以得 6。

每次呼叫通常需要保存尚未完成的工作，可能耗盡執行資源；標準不保證尾呼叫最佳化。這個問題用迴圈更直接，遞迴的價值會在樹狀結構等自然遞迴問題出現。不要只因寫法短就選遞迴。

## 4.4 常見問題與練習

忘記回傳值、宣告與定義型別不一致、回傳區域物件位址，都可能造成錯誤。先核對原型、每一條控制路徑及物件生命期。`void` 函式不產生可用的回傳值，不能拿來相加。

練習中的 `condition ? a : b` 是條件運算子：先判斷 condition，為真只求值 a，為假只求值 b，整個運算式取得被選中的值。它適合簡短的二選一；複雜分支仍用 if 以保持可讀性。

1. 基礎：寫 `max2(int a, int b)`。解答：`return a > b ? a : b;`，或用 if；相等時任選相同值。測 -3／-8、5／5、1／9。
2. 應用：函式中做 `score = 0;`，呼叫端的 score 為何仍是 75？解答：修改的是參數副本；能畫出兩個不同的物件即達標。
3. 挑戰：把 sum_to 改為迴圈，保持相同 0–10 契約。解答：累加器設 0，i 從 1 到 n 累加，回傳累加器。n=0 時不執行迴圈，結果仍為 0；這是驗收必查的邊界。

下一步：[保存多筆資料](chapters/05-arrays.md)。

## 本章查閱與延伸

- [Aspnes C/Functions](https://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Functions.html)：已讀定義、呼叫與傳值部分；本書契約與小範圍遞迴範例為原創。


---

# 第 5 章　陣列、字串與文字輸入

先備：[第 4 章](chapters/04-functions.md)。目標是保存多筆同型別資料、理解字串終止符與容量。上一章資料只活在單一變數；現在我們要讓每筆成績能被再次查看。

## 5.1 陣列是連續的元素

`int scores[3] = {80, 95, 70};` 定義三個連續的 int，索引為 0、1、2。`scores[1]` 是第二筆。長度為 n 的陣列只可存取 0 到 n-1；`scores[3]` 不是「自動新增第四筆」。下標通常沒有自動範圍檢查。

```c
#include <stdio.h>

int main(void)
{
    int scores[] = {80, 95, 70};
    size_t n = sizeof scores / sizeof scores[0];
    int total = 0;
    for (size_t i = 0; i < n; ++i) {
        total += scores[i];
    }
    printf("n=%zu mean=%.2f\n", n, (double)total / n);
    return 0;
}
```

未實測，推演 `n=3 mean=81.67`。這個 sizeof 除法只因這裡的 scores 確實是陣列才成立；把陣列交給函式後，通常無法用同一公式推得元素數。下一章解釋轉換。[N1570 §6.5.3.4](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) 有 sizeof 與參數調整的規則。

`int grid[2][3]` 是兩個元素的陣列，每個元素又是三個 int 的陣列。`grid[1][2]` 是第二列第三欄。依列儲存，同一列的欄位先相鄰。[Microsoft Array Declarations](https://learn.microsoft.com/en-us/cpp/c-language/array-declarations?view=msvc-170) 支持宣告與列儲存；其把陣列稱為指標的簡化措辭不應當作型別相同，請接著讀第 6 章。

## 5.2 字串需要結尾，也需要空間

`char name[] = "Ada";` 有四個 char：`'A'`、`'d'`、`'a'`、`'\0'`。單引號表示字元常數，雙引號表示字串字面值。`'\0'` 是數值零的終止字元，不是 `'0'`，也不是空指標 NULL。

陣列容量是可使用的總格數；字串長度是第一個終止字元前的位元組數。`char name[16] = "Ada";` 容量 16、長度 3。`strlen` 需要有效的零結尾字串；對沒有終止符的緩衝區呼叫它，並不能替你找出安全邊界。UTF-8 中文常佔多個位元組，strlen 不等於人眼看到的字數。

## 5.3 讀取一行的完整範例

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    char name[32];
    if (fgets(name, sizeof name, stdin) == NULL) {
        fputs("no input or read error\n", stderr);
        return 1;
    }
    size_t end = strcspn(name, "\n");
    if (name[end] != '\n') {
        fputs("line needs a newline within 31 bytes\n", stderr);
        return 1;
    }
    name[end] = '\0';
    printf("Hello, %s!\n", name);
    return 0;
}
```

未實測。輸入 `Ada` 再按 Enter，推演為 `Hello, Ada!`。stdin 是標準輸入串流，stderr 是錯誤訊息串流。`fgets` 最多讀取容量減一並保留換行、補上終止字元；必須先確認成功才能使用內容。[fgets 文件](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-170) 說明此行為。

`strcspn` 回傳第一個屬於指定集合的字元前的長度；這裡找換行。若讀到換行，就把它替換成零。這個教學程式採保守契約：**沒有在緩衝區讀到換行就拒絕**，包含沒有換行的最後一行。它遇到過長行後直接結束，不會把殘餘輸入誤認為下一筆。第 13 章會提供接受 EOF 最後一行、並排空過長行的讀取器。本章輸入假設是無內嵌零的文字。

不要用 `gets`，也不要對不受信任的文字直接做 `printf(name)`；使用 `printf("%s", name)`，讓格式字串由程式控制。`scanf("%s", ...)` 不會讀完整含空白的一行，還需要寬度限制；初學先用行讀取再解析。

## 5.4 練習與解答

1. 基礎：`char s[]="C";` 的 sizeof 與 strlen？解答：2 與 1；必須解釋終止字元佔一格。
2. 應用：找出 `{80,95,70}` 最大值。解答：用第一筆初始化 max，從索引 1 走到 n-1，遇到較大值才替換，結果 95；空陣列情境需另訂契約，不能先讀第 0 筆。
3. 挑戰：為何 `char s[3]={'A','d','a'}; puts(s);` 不可靠？解答：沒有零結尾，puts 可能越過陣列讀取。改為四格含終止字元，或依明確長度逐格輸出。成功判準是能區分「字元陣列」與「字串」。

下一步：[指標與陣列的真正關係](chapters/06-pointers.md)。

## 本章查閱與延伸

- [Microsoft 陣列宣告](https://learn.microsoft.com/en-us/cpp/c-language/array-declarations?view=msvc-170)：宣告、二維陣列與 row-major；簡化措辭的限制如正文。
- [fgets](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-170)：讀取上限、換行、終止字元與回傳值。
- [N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：§6.5.3.4 的陣列大小與指標參數差別。


---

# 第 6 章　指標、陣列轉換與生命期

先備：[第 5 章](chapters/05-arrays.md)。目標是透過指標改動呼叫端資料，並判斷可存取範圍與生命期。單靠「指標存位址」仍不足以保證安全：你還必須知道它指向哪個物件、能讀寫幾個元素、物件是否仍存在。

## 6.1 取址與間接存取

`int x=10; int *p=&x;` 中，`&x` 取得 x 的位址，p 的型別是「指向 int 的指標」。`*p=20;` 透過它修改 x。宣告中的星號描述型別，運算式中的星號做間接存取，不能只看符號而忽略位置。[Microsoft Pointer Declarations](https://learn.microsoft.com/en-us/cpp/c-language/pointer-declarations?view=msvc-170) 可核對宣告形式。

```c
#include <stdio.h>

static void swap(int *a, int *b)
{
    int saved = *a;
    *a = *b;
    *b = saved;
}

int main(void)
{
    int low = 70;
    int high = 95;
    swap(&low, &high);
    printf("%d %d\n", low, high);
    return 0;
}
```

未實測，推演 `95 70`。swap 的契約：a、b 都指向活著且可寫的 int，可指向同一物件。參數仍是傳值，但副本保留指向原物件的關係。若省去 & 傳整數進來，就是型別不符，不是另一種等價寫法。

## 6.2 陣列不是指標

陣列本身包含元素；指標物件保存指標值。多數運算式中的陣列會轉成指向首元素的指標，sizeof、取址等情境則有例外。下面是函式片段：

```c
/* a must point to at least n readable ints; n must be positive. */
static double mean(const int *a, size_t n)
{
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        total += a[i];
    }
    return total / n;
}
```

需要先包含 `<stddef.h>`。`const int *a` 表示不能**透過 a** 修改 int，並不保證別人完全不能改同一資料。函式必須另收 n；`sizeof a` 得到指標大小，無法推出陣列長度。此處契約要求 n>0；整合專題會顯式處理空資料。

`a[i]` 等價於 `*(a+i)`。a 指向陣列第一格時，a+1 指向下一個元素，跨過一個 int 的儲存空間，而非固定一個位元組。可以形成同陣列尾後一格指標作為界線，不能解參照它；也不能先算出遠遠越界的指標，再說稍後會移回來。[N1570 §6.5.6](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) 是此範圍規則的依據。

二維陣列的列大小也是型別的一部分：傳 `int a[2][3]` 給函式，可用參數 `int (*a)[3]`，不是 `int **`。後者是指向指標的指標，資料布局與解讀方式不同。

## 6.3 有位址不代表能用

`int *p=NULL;` 表示沒有指向物件，不能做 `*p`。未初始化指標連「是否為 NULL」都不能隨意假定。函式回傳區域變數的位址，會留下指向生命期已結束物件的懸空關係。不是因為下一次呼叫「一定覆蓋」才錯，而是生命期規則已不允許再用。[N1570 §6.2.4](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) 說明此界線。

`int *p, q;` 只有 p 是指標。`int *const p` 是不能重新指定的指標；`const int *p` 是透過它只讀目標；`const int *const p` 兩者皆有。`void *` 可承載物件指標，使用前要有正確型別；ISO C 不提供對 void 指標的算術。函式指標另有規則，不假定它與 void 指標可攜互換。

## 6.4 練習與解答

1. 基礎：x=3、p=&x、*p=8，x 為何？解答：8，改的是同一物件。
2. 應用：替 mean 寫最小 main，以 `{60,80,100}` 呼叫並印一位小數。解答：計算 n 後傳入 `mean(scores,n)`，結果 80.0；不得在 mean 內用 sizeof 猜 n。
3. 挑戰：`int a[3]; int *end=a+3;` 哪些合法？解答：形成 end 合法、`end-a` 為 3；讀 `*end` 不合法。若再形成 `a+4` 已超出允許範圍。成功判準是把「形成指標」與「存取元素」分開判斷。

下一步：[組合不同型別資料](chapters/07-records.md)。

## 本章查閱與延伸

- [Microsoft 指標宣告](https://learn.microsoft.com/en-us/cpp/c-language/pointer-declarations?view=msvc-170)：指向物件、陣列與其他指標的宣告。
- [N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：已讀 §6.2.4 生命期、§6.5.3.4 sizeof、§6.5.6 指標算術相關段落。


---

# 第 7 章　結構、列舉、聯集與位元

先備：[第 6 章](chapters/06-pointers.md)。目標是把姓名與成績合成記錄，表示有種類的值，並用位元旗標記錄多個開關。不同欄位如果分散在兩個陣列，一次排序可能只移動成績而忘記姓名；結構讓相關資料成為一個整體。

## 7.1 struct 與 typedef

```c
#include <stdio.h>

typedef struct {
    char name[16];
    int score;
} Student;

static void print_student(const Student *s)
{
    printf("%s: %d\n", s->name, s->score);
}

int main(void)
{
    Student first = {.name = "Ada", .score = 90};
    Student copy = first;
    copy.score = 85;
    print_student(&first);
    print_student(&copy);
    return 0;
}
```

未實測，推演兩行 `Ada: 90`、`Ada: 85`。`typedef` 建立型別別名，不是配置記憶體。點號存取結構物件的成員，箭頭 `s->score` 等價於 `(*s).score`。初始化器中的 `.name` 指定成員，不靠欄位順序猜意圖。[Microsoft Structure Declarations](https://learn.microsoft.com/en-us/cpp/c-language/structure-declarations?view=msvc-170) 解釋結構的成員與型別定義。

結構賦值可複製整體，包括成員陣列；但若成員是指標，只複製指標值，沒有自動複製它指向的記憶體。這是淺複製。兩個副本若都以為自己應 free 同一資源，就會重複釋放，下一章會用所有權避免。

編譯器可為對齊加入 padding（填補空間），sizeof 結構不一定等於各欄位大小總和。因此不要用 memcmp 當作一般結構相等，也不要把整個結構直接寫成跨平台檔案格式。

## 7.2 enum 與 tagged union

列舉替整數狀態命名，例如 `enum Status { STATUS_OK, STATUS_INVALID, STATUS_IO };`。使用名稱比回傳神祕的 0、1、2 易讀；仍需文件說明每個值的意義。

聯集 union 的成員共用儲存區。若某欄有時存成績、有時存文字，就搭配 tag 記錄目前使用哪一種：

```c
enum Kind { VALUE_SCORE, VALUE_TEXT };
struct Value {
    enum Kind kind;
    union {
        int score;
        char text[16];
    } data;
};
/* Example inside a function: */
/* struct Value v = {.kind=VALUE_SCORE, .data.score=90}; */
```

這是型別片段，未實測。先檢查 kind 再讀相符成員，並在更新時一起改 tag。不要把讀取非目前成員當作一般數值轉換；表示、陷阱值與可攜性牽涉更多規則。第 14 章會用 memcpy 說明較合適的表示觀察方法。

## 7.3 用位元表示多個選項

```c
unsigned int flags = 0;
const unsigned int PRESENT = 1u << 0;
const unsigned int REVIEW = 1u << 1;
flags |= PRESENT;              /* set */
flags |= REVIEW;
flags &= ~REVIEW;              /* clear */
/* (flags & PRESENT) != 0 tests the bit */
```

這是函式內片段，最後 flags 為 1。`|` 合併位元、`&` 篩選、`^` 切換、`~` 反轉。位移用無號型別並限制位數小於提升後型別寬度，且不能為負。旗標是獨立開關；列舉 tag 是單一分類，不要混淆兩種模型。[Microsoft Bitwise Operators](https://learn.microsoft.com/en-us/cpp/c-language/c-bitwise-operators?view=msvc-170) 說明基本位元運算；Microsoft 特定的 signed 行為不作為本書可攜保證。

## 7.4 練習與解答

1. 基礎：把結構增加 `int absences`，初始化為 2 並列印。解答：指定 `.absences=2`，透過 `s->absences` 配合 `%d`。舊欄位不應被錯置。
2. 應用：如何判斷兩個 Student 內容相等？解答：分別比較 score 與 `strcmp(name1,name2)==0`，假設姓名有效零結尾；不能直接以 `==` 比較結構，也不以 memcmp 判值相等。
3. 挑戰：兩個結構副本共享同一 char 指標，有什麼成本？解答：複製便宜但生命期相依；必須指定唯一所有者、借用者或做深複製。能指出「陣列成員」與「指標成員」複製後的不同關係才算通過。

下一步：[自己管理記憶體](chapters/08-memory.md)。

## 本章查閱與延伸

- [結構宣告](https://learn.microsoft.com/en-us/cpp/c-language/structure-declarations?view=msvc-170)：成員、結構型別與不完整型別。
- [位元運算](https://learn.microsoft.com/en-us/cpp/c-language/c-bitwise-operators?view=msvc-170)：AND／OR／XOR。本章 tagged union 與記錄案例為教學設計。


---

# 第 8 章　動態記憶體與所有權

先備：[第 7 章](chapters/07-records.md)。目標是處理執行時才知道的資料量，並在成功與失敗路徑都交代清楚資源。固定大小陣列很簡單，但不能無限制接收檔案；動態配置能調整容量，也把清理責任交給你。

## 8.1 配置不是初始化

`malloc(bytes)` 取得儲存區，失敗回傳 NULL；成功後的內容尚未初始化。`free(p)` 歸還配置，不會自動清除其他指向它的指標。`calloc(n,size)` 取得以全零位元初始化的儲存區；不要泛化成所有型別的值都等於零，例如全零位元不保證是所有平台的空指標表示。

所有權是本書採用的程式設計約定：某個物件負責最後一次清理，其他指標只借用。這不是 C 型別系統自動強制的規則。把 `p=NULL` 只能更新 p，不能修好其他已懸空的別名。生命期結束後就不要再讀取或比較那些舊指標值。[CERT MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/) 以釋放後存取說明風險。

## 8.2 安全擴充的完整示範

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int grow(int **owner, size_t count)
{
    if (count == 0 || count > SIZE_MAX / sizeof **owner) {
        return 0;
    }
    int *next = realloc(*owner, count * sizeof **owner);
    if (next == NULL) {
        return 0;
    }
    *owner = next;
    return 1;
}

int main(void)
{
    int *scores = NULL;
    if (!grow(&scores, 3)) {
        fputs("allocation failed\n", stderr);
        return 1;
    }
    scores[0] = 80;
    scores[1] = 95;
    scores[2] = 70;
    if (!grow(&scores, 6)) {
        free(scores);
        return 1;
    }
    scores[3] = 100;
    printf("%d %d\n", scores[0], scores[3]);
    free(scores);
    scores = NULL;
    return 0;
}
```

未實測，若配置成功推演 `80 100`。owner 必須指向有效且已初始化的 int 指標，該指標為 NULL 或由相容配置函式傳回。二層指標讓 grow 能改呼叫端的 owner。新容量六格只有前四格完成初始化，其餘不可當作已有成績讀取。

乘法前先確認 `count <= SIZE_MAX / sizeof **owner`，否則乘法可能回繞成很小的數，結果配置不足卻大量寫入。[CERT MEM35-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem35-c/) 支持這類容量檢查。

`realloc` 成功可能搬家，原本借用的元素指標都應視為失效；失敗且要求的大小非零時，原配置仍由呼叫者持有。因此先放在 next，成功才更新 owner。直接 `p=realloc(p,...)` 會在失敗時遺失原位址。[realloc 文件](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170) 說明非零配置失敗與搬移。本書刻意不呼叫 `realloc(p,0)`，各版本／實作的細節不同，不以 Microsoft 的零大小行為推論 ISO C。

## 8.3 長度、容量與失敗原子性

長度 len 是已初始化且有意義的元素數，容量 cap 是配置能裝的元素數。維持 `0 <= len <= cap`；只有擴充成功後才更新 cap，只有元素寫入成功後才增加 len。這稱為讓失敗不破壞原狀態的設計。本書專題會把三者放入同一結構。

固定容量成本最低但有上限；每新增一筆就擴充一格可能反覆搬移；倍增容量可減少搬移次數，代價是預留空間。釋放函式要只接收配置起點，不能 free 陣列中間、區域變數或字串字面值。free(NULL) 可用於統一清理。

## 8.4 練習與解答

1. 基礎：成功 malloc 後要做哪三件事？解答：確認非 NULL、在讀前初始化、在最後使用後恰好釋放一次。順序與責任都要清楚。
2. 應用：grow 失敗時誰清理？解答：原所有者。函式沒有更新 owner，呼叫端仍能 free 原配置；若原本為 NULL，free(NULL) 也可。
3. 挑戰：保存 `int *best=&scores[1]` 後成功 grow，如何再使用第二筆？解答：保存索引 1，擴充成功後以新的 scores 重新取得 `&scores[1]`；不可沿用 best。能解釋搬移與生命期而非只說「指標變了」才達標。

下一步：[把作品拆成模組](chapters/09-modules.md)。

## 本章查閱與延伸

- [realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170)：非零失敗、內容保留、新增範圍未初始化；Microsoft 零大小行為不採用。
- [MEM35-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem35-c/)：配置大小計算。
- [MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/)：懸空指標與釋放後存取。


---

# 第 9 章　標頭、分檔與前處理器

先備：[第 8 章](chapters/08-memory.md)。目標是分開公開介面與實作，從多個原始檔建置程式。當所有程式塞進 main，修改統計規則時容易碰壞輸入；分檔讓責任邊界可見，也讓編譯器幫忙核對介面。

## 9.1 一份介面、一份定義

以下三個檔案需放在同一資料夾，也提供於 examples/modules。全部未實測。

`stats.h`：

```c
#ifndef CBOOK_STATS_H
#define CBOOK_STATS_H
#include <stddef.h>
/* a points to n readable ints; n > 0; result is their mean. */
double stats_mean(const int *a, size_t n);
#endif
```

`stats.c`：

```c
#include "stats.h"

double stats_mean(const int *a, size_t n)
{
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        total += a[i];
    }
    return total / n;
}
```

`main.c`：

```c
#include <stdio.h>
#include "stats.h"

int main(void)
{
    int scores[] = {70, 80, 90};
    printf("%.1f\n", stats_mean(scores, 3));
    return 0;
}
```

編譯 `gcc -std=c17 -Wall -Wextra -Wpedantic main.c stats.c -o stats.exe`，執行 `./stats.exe`，推演 80.0。每個 `.c` 經前處理後成為各自的翻譯單元，連結時才組合。不要 `#include "stats.c"`；那會把定義文字放進另一單元，若又另外編譯 stats.c 就可能重複定義。

標頭放宣告與需要共享的型別；實作也 include 自己標頭，使宣告與定義不一致時可被發現。標頭防護避免同一單元重複引入；它不會防止你在多個單元各定義同一個外部物件。[Microsoft #include](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp?view=msvc-170) 解釋文字引入與搜尋方式；本書不依賴其特定搜尋路徑順序。

## 9.2 宣告、定義與連結

`extern int app_count;` 宣告另一處定義的物件；真正的 `int app_count=0;` 只放一份在 `.c`。把具有外部連結的定義放進一般標頭，容易形成多份定義。較好的設計通常是以函式封裝狀態，避免呼叫端直接改動全域物件。

檔案內專用的輔助函式加 static，限制名稱在本單元的可見連結範圍。公開名稱加模組前綴如 stats_mean，減少碰撞。若用 `static inline` 放小函式於標頭，仍需了解它產生每單元內部定義；先不要憑 inline 關鍵字承諾一定不產生函式呼叫。

## 9.3 巨集是文字替換，不是普通函式

```c
#define BAD_SQUARE(x) x * x
#define SQUARE(x) ((x) * (x))
```

`BAD_SQUARE(1+2)` 展開成 `1+2*1+2`，得 5。第二個雖修正括號，`SQUARE(i++)` 仍會重複求值且造成未定義行為；應使用一般函式，並限定乘法不溢位的輸入。巨集適合條件編譯、常數與某些型別泛用場合，不宜隱藏帶副作用的工作。

你也可以分開執行 `gcc -c main.c`、`gcc -c stats.c`、`gcc main.o stats.o -o stats.exe`。`.o` 是尚待連結的物件檔。建置工具的價值是記錄相依與選項，不只是縮短命令。初學先能手動建置，再學 Make／CMake，比一開始複製龐大設定更容易判讀失敗。

## 9.4 練習與解答

1. 基礎：只編譯 main.c，為何可能連結失敗？解答：標頭只提供宣告，缺 stats_mean 定義，須一起連結 stats.o 或 stats.c。
2. 應用：新增 `int stats_max(const int *a,size_t n);`。解答：標頭加宣告，stats.c 以 a[0] 初始化後遍歷；契約 n>0。由 main 呼叫 `{70,80,90}` 應得 90。
3. 挑戰：為何有標頭防護仍發生重複定義？解答：防護的巨集狀態每個翻譯單元各自處理；不能跨單元防止外部定義重複。能畫出兩次獨立前處理過程即達標。

下一步：[讓資料進出程式](chapters/10-io.md)。

## 本章查閱與延伸

- [Microsoft #include](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp?view=msvc-170)：引入標頭的機制；平台搜尋順序須查實際編譯器。
- [Aspnes Functions](https://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Functions.html)：宣告與定義的教學背景；本章模組與巨集反例為本書原創。


---

# 第 10 章　檔案、解析與錯誤處理

先備：[第 9 章](chapters/09-modules.md)。目標是區分讀取、解析與業務驗證，處理正常結尾和真正錯誤。使用者打入 `90oops` 時，只取前面的 90 可能不是你想要的行為；資料進入程式之前應先有明確格式。

## 10.1 三層驗證

第一層是傳輸：有沒有成功讀到完整資料？第二層是語法：文字是不是完整整數？第三層是領域：整數是否落在成績 0–100？把它們混成一次 scanf，常會丟失「錯在哪裡」的資訊。

以下解析器接受十進位整數與前後空白，拒絕沒有數字、尾端垃圾、超出 long 範圍與不合法成績。需要 `<ctype.h>`、`<errno.h>`、`<stdlib.h>`。這是函式片段，未實測。

```c
static int parse_score(const char *text, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text || errno == ERANGE) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || value < 0 || value > 100) {
        return 0;
    }
    *out = (int)value;
    return 1;
}
```

契約是 text 指向有效零結尾字串、out 指向可寫 int。失敗時不改 out。`strtol` 的 end 指向第一個未轉換字元；`end==text` 表示沒轉成數字。errno 要先歸零，再查 ERANGE，否則可能把舊錯誤當新錯誤。先確認 0–100 後，轉成 int 才有範圍依據。[strtol 文件](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170) 支持停止位置與溢位判斷。

`isspace` 的參數必須是 EOF 或可表示為 unsigned char 的值；一般 char 可能有負值，因此先轉 unsigned char。`atoi` 缺少同等的錯誤與停止位置資訊，不適合作為本書的嚴格驗證器。

## 10.2 開檔、讀取、關閉

```c
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: lines.exe FILE\n", stderr);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("open");
        return 1;
    }
    char block[128];
    int failed = 0;
    while (fgets(block, sizeof block, in) != NULL) {
        if (fputs(block, stdout) == EOF) {
            failed = 1;
            break;
        }
    }
    if (ferror(in)) {
        failed = 1;
    }
    if (fclose(in) == EOF) {
        failed = 1;
    }
    if (fflush(stdout) == EOF) {
        failed = 1;
    }
    return failed ? 1 : 0;
}
```

未實測。存成 `lines.c` 後依第 1 章編譯，執行 `./lines.exe input.txt`；對一般無內嵌零的文字，推演結果為把檔案內容顯示到終端。argc 是引數數量，argv[0] 是程式名稱，argv[1] 是第一個額外引數。這個程式依區塊輸出，不把每次 fgets 都叫做一筆完整紀錄；長行可分多次讀取。

讀取失敗可能是 EOF，也可能是 I/O 錯誤，迴圈結束後要查 ferror。不要寫 `while (!feof(in))`：EOF 指示通常要在一次讀取遇到結尾後才設定，預先問它容易多處理舊資料。fclose 也可能失敗；輸出有緩衝，寫入當下成功不保證最後 flush 成功。[fclose 文件](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170) 說明關閉與緩衝處理。

## 10.3 格式與可攜性

`"r"` 讀取既有文字，`"w"` 會建立或截斷檔案，`"a"` 追加；二進位用途用 `"rb"`、`"wb"`。切勿對要保留的輸入誤用 w。[fopen 文件](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170) 的模式段落說明差異。Windows 文字模式可能轉換換行，二進位模式才適合逐位元組協定。

`fread(buffer, size, count, file)` 回傳完整元素數，不一定是位元組數。以 `size=1` 讀取時，回傳值才直接是位元組數；短讀要分辨結尾與錯誤。[fread 文件](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-170) 支持這項區別。

把 struct 整塊 fwrite 到檔案，可能包含 padding、不同大小與不同位元組順序；應定義明確欄位編碼、版本與範圍。文字格式容易檢查，二進位格式通常更緊湊，但你要自己處理相容性。這是設計取捨，不是某種模式自動比較安全。

## 10.4 練習與解答

1. 基礎：解析 `90`、` 90 `、`90x`、空字串。解答：前二成功，後二失敗；out 在失敗時保持原值。
2. 應用：為何 `fread(buf,4,10,in)` 回傳 3 不能說讀到 3 bytes？解答：是三個完整的四位元組元素，共 12 個完整元素位元組；還可能有不完整元素，不能使用未確認的部分。
3. 挑戰：把 parser 改成支援 -50 到 150。解答：調整 value 範圍檢查，其餘保持；驗收 -51、-50、150、151、巨大整數與尾端垃圾。不要先 cast 再查範圍。

下一步：[讓錯誤可以重現與定位](chapters/11-debug.md)。

## 本章查閱與延伸

- [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170)：停止位置、ERANGE；僅採標準函式部分。
- [fopen](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170)：開檔、回傳值、模式；不採 Windows Unicode 擴充作為 ISO 保證。
- [fread](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-170)、[fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170)：元素數、短讀、關閉錯誤。


---

# 第 11 章　除錯、測試與可靠性

先備：[第 10 章](chapters/10-io.md)。目標是把「有時怪怪的」變成可重現的問題，並理解工具能證明什麼。看到當機位置不一定就是根因：較早的越界寫入可能直到後面配置記憶體時才顯現。

## 11.1 從證據開始

依序記錄最小輸入、預期輸出、實際輸出、編譯選項與版本。先保留可重現版本，再一次改一件事。若陣列統計錯了，先用三筆可手算資料，檢查 len、cap、索引、累加器；不必一開始用一百萬筆。

常見缺陷可分為語法／連結錯誤、邏輯錯誤、資源錯誤與未定義行為（UB）。最後一類不能以「上次得到正確答案」排除。越界、釋放後使用、格式與引數型別不符、有號溢位，都可能在最佳化後表現不同。

## 11.2 有界整數加法的完整例子

```c
#include <assert.h>
#include <limits.h>
#include <stdio.h>

static int add_int(int a, int b, int *out)
{
    if ((b > 0 && a > INT_MAX - b) ||
        (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    *out = a + b;
    return 1;
}

int main(void)
{
    int result = 0;
    int ok = add_int(70, 30, &result);
    assert(ok && result == 100);
    ok = add_int(INT_MAX, 1, &result);
    assert(!ok && result == 100);
    printf("%d\n", result);
    return 0;
}
```

未實測，推演為 100。正 b 時先比較與 INT_MAX 的剩餘距離；負 b 時先比較與 INT_MIN 的距離。只有確認可表示才做 a+b。寫成「先算再看結果是否變小」對有號溢位並不可靠。out 契約是有效可寫 int，失敗時保留原值。

assert 用來檢查程式內部假設，可能因 NDEBUG 被移除，所以不要把必要工作藏在 `assert(add_int(...))` 裡。本例先呼叫再 assert；正式輸入驗證仍應有普通 if 分支。斷言通過也僅代表已執行路徑符合條件。

## 11.3 用動態工具擴大觀察

runtime（執行期支援函式庫）提供程式執行時所需的檢查與報告功能。在已具備對應 runtime 的 Clang 環境，可嘗試：

```sh
clang -std=c17 -Wall -Wextra -Wpedantic -g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined program.c -o program.exe
./program.exe
```

命令未實測，平台支援不同；Windows 的 UCRT64 GCC 不保證直接支援此組合，不要把缺 sanitizer runtime 當作程式本體錯誤。可用支援的 Linux／WSL／Clang 環境做同樣練習。ASan 擅長發現越界與 use-after-free；UBSan 可檢查某些溢位、位移與不合法存取。兩者都需要執行到出問題路徑，且不涵蓋全部語意錯誤。[ASan 官方文件](https://clang.llvm.org/docs/AddressSanitizer.html)、[UBSan 官方文件](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html) 說明各自能力。

讀報告時先看錯誤種類，再看第一個屬於自己程式的 stack frame，最後對照配置與釋放位置。不要只把報告行旁的指標改成 NULL；應回到誰擁有資源、哪個索引應受限的設計。

有 GDB 時可編譯 `-g -O0`，使用 `gdb program.exe`，依序 `break main`、`run`、`next`、`print variable`、`backtrace`。這些命令未實測。單步執行適合查控制流程，Sanitizer 適合捕捉非法操作，測試則負責描述可預期行為，三者相互補充。

## 11.4 從單點案例到性質

對成績 parser 測空字串、0、100、-1、101、超長數字、尾端文字；對排序測空、一筆、重複、已排序、逆序。除了例子，還可檢查性質：排序後相鄰元素非遞減，元素數不變，每個原值出現次數不變。只有檢查「有序」可能漏掉把全部元素改成零的錯誤。

配置失敗、檔案讀取錯誤不容易自然重現，可在測試版本替配置或讀取函式加可控失敗點；不必真的耗盡系統記憶體。fuzzing 是自動產生大量輸入找缺陷，仍需要可檢查的結果或不變量。它不能代替契約與規格。

## 11.5 練習與解答

1. 基礎：沒有警告是否等於正確？解答：否，工具只能檢查部分問題；例如門檻寫成 61 仍可合法編譯。
2. 應用：補測 add_int 的 `INT_MIN,-1`、`INT_MIN,0`、`INT_MAX,-1`。解答：依序失敗、成功為 INT_MIN、成功為 INT_MAX-1；失敗保留 out。
3. 挑戰：排序後檢查非遞減仍漏了什麼？解答：可能遺失、重複或改壞元素；再核對元素多重集合。成功判準是能提出反例，而非只列工具名稱。

下一步：[選擇資料結構與演算法](chapters/12-algorithms.md)。

## 本章查閱與延伸

- [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)：Introduction、Usage 與平台限制段落。
- [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)：Introduction、Usage、Available checks。
- [Aspnes 教材](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html)：查 Debugging 與 Assertions；本書邊界測試及加法推導為原創。


---

# 第 12 章　資料結構、演算法與效能

先備：[第 11 章](chapters/11-debug.md)。目標是依工作負載選擇資料結構、寫比較器、推導時間與空間成本。若只要平均值，單次掃描就足夠；若要反覆查名次或中位數，排序與額外儲存才有價值。

## 12.1 用成長量描述成本

n 筆資料全部加總需要約 n 次處理，記為 O(n)；雙層完整比較約 n² 次，記為 O(n²)。O 是漸近上界，不是實測秒數；不同 n、常數、快取與配置器會影響實際速度。

線性搜尋逐筆找，最壞看 n 筆。已排序的陣列可二分搜尋：每次排除一半，經 k 次後剩約 n/2^k；令其不超過 1 得 k 約為 log₂n。前置排序有成本，單次查詢不一定值得。

使用半開區間 `[lo,hi)` 可讓空區間自然表示成 lo==hi。中點寫 `lo + (hi-lo)/2`，避免 lo+hi 相加溢位。比較後，若中點太小，設 lo=mid+1；否則 hi=mid。每一步都應保持答案候選在區間內，且區間嚴格縮小。

## 12.2 標準排序與回呼的完整示範

```c
#include <stdio.h>
#include <stdlib.h>

static int compare_int(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

int main(void)
{
    int a[] = {95, 70, 80, 70};
    size_t n = sizeof a / sizeof a[0];
    qsort(a, n, sizeof a[0], compare_int);
    for (size_t i = 0; i < n; ++i) {
        printf("%d%s", a[i], i + 1 == n ? "\n" : " ");
    }
    return 0;
}
```

未實測，推演 `70 70 80 95`。qsort 需要首位址、元素數、單一元素大小與比較函式。比較器回傳負數／零／正數，表示小於／等於／大於；不是必須回傳兩數差。[Microsoft qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170) 說明此介面。

`return a-b` 可能溢位；本書用兩個比較結果相減，只產生 -1、0、1。比較器必須一致且具傳遞性，不能比較時隨機改資料。相等元素的相對順序沒有穩定性保證；若要保留原順序，可把原索引列為第二比較鍵。不要只因函式名就推論 ISO 保證其內部一定用某個演算法或固定複雜度。

函式名稱在此轉成函式指標供 qsort 呼叫，這種「把處理方式交給另一函式」叫 callback（回呼）。可以寫 `int (*cmp)(const void *,const void *)=compare_int;` 再傳 cmp。[Aspnes 的函式指標段落](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html) 說明宣告與 callback；其中差值比較器範例不應直接用於全範圍 int，本書已改寫。

## 12.3 動態陣列的攤銷分析

容量滿時倍增，假設從 1 開始，搬移量依序約為 1+2+4+…+2^k，小於 2^(k+1)。到 n 筆時，總搬移量為 O(n)，因此 n 次追加的平均成本為 O(1)，叫攤銷常數時間；單次觸發擴充仍可能 O(n)。這不代表每筆實際耗時一樣，更不表示最壞延遲已受控制。記憶體受限或即時系統可能偏好預先配置。

## 12.4 鏈結串列：另一種成本

```c
struct Node {
    int value;
    struct Node *next;
};
/* cleanup inside a function, with head owned by the caller: */
/* while (head != NULL) {
       struct Node *next = head->next;
       free(head);
       head = next;
   } */
```

型別與清理片段未實測。清理要在 free 前保存 next，之後不能再讀 head->next。串列若已有前驅指標可便宜插刪，但找第 i 筆要逐節點走，配置次數多且快取區域性可能較差。動態陣列有直接索引與相鄰資料，擴充會使借用指標失效；串列節點位置較穩定，卻有額外指標與配置成本。選擇要看操作比例，不是某種結構永遠較進階。

效能量測前先定義輸入分布與正確性檢查，再以相同工具、選項、機器重複比較。把檔案 I/O 和排序時間分開觀察；不能憑一個小例子宣稱演算法快數倍。

## 12.5 練習與解答

1. 基礎：qsort 第三個引數為何不是 sizeof a？解答：它要的是單一元素寬度，sizeof a 是整個陣列大小。
2. 應用：改為遞減。解答：比較器用 `(a < b) - (a > b)`，推演 `95 80 70 70`；仍不做可能溢位的相減。
3. 挑戰：每次容量加一與倍增，各搬移多少？解答：前者總計 1+…+(n-1)=n(n-1)/2，O(n²)；倍增為幾何級數，O(n)。成功判準是寫出總和及空間取捨。

下一步：[完成可讀檔的作品](chapters/13-project.md)。

## 本章查閱與延伸

- [qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170)：參數、比較結果、相等項順序。
- [Aspnes 教材 §4.9.8](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html)：函式指標與回呼；本章攤銷推導、結構比較與範例為原創。
- [CERT MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/)：節點釋放後不可再讀其 next。


---

# 第 13 章　完整專題：檔案成績分析器

先備：[第 1–12 章](README.md)。目標是完成能解釋、修改與檢查的作品。原始碼是本次原創，完整檔案為 [gradebook.c](examples/gradebook.c)，不是需要讀者自行補齊的骨架。所有程式及下列命令未實測，預期結果由人工推演。

## 13.1 先決定輸入與失敗行為

每行一個十進位整數，範圍 0–100，可有前後空白；拒絕空白行、尾端垃圾、內嵌零與過長行。每行換行前至多 127 個讀取字元，最後一行可沒有 LF；CRLF 在文字模式的轉換依平台而定，剩餘 CR 也可作尾端空白處理。不接受 UTF-8 BOM。最多一百萬筆，實際可配置量可能更小。

成功顯示筆數、最小、最大、平均、中位數與及格數。空檔顯示沒有資料，不除以零。第一個錯誤即停止，不輸出部分統計，以免讀者把不完整結果當成全檔結果。成功結束狀態為 0，其他為 1；程式只讀取輸入檔，不建立報表檔。

## 13.2 資料流與責任

```text
檔案 → read_line → parse_score → append_score → 統計／排序 → 標準輸出
          完整行       值是否合法      所有權及容量        明確處理空資料
```

Scores 把 data、len、cap 綁在一起；main 是其唯一所有者，最後統一 free。read_line 用 fgetc 逐字元讀取，能同時判斷內嵌零、容量與實體行結尾；超長後仍讀到換行或 EOF，避免留下半行。fgetc 用 int 接結果，因為必須同時表示字元值與 EOF。這比第 5 章的保守 fgets 範例多了狀態，但清楚交代了邊界。

parse_score 重用第 10 章的分層檢查；append_score 重用第 8 章的暫存 realloc 與乘法前置檢查。只有所有輸入與關檔都成功才統計。標準 I/O 可能緩衝，所以最後還檢查 fflush(stdout)。有錯誤時無法保證終端一定顯示完整診斷，例如 stderr 本身也失效；程式仍以非零狀態回報失敗。

這些 API 的行為依據是 [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170)、[realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170)、[qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170) 與 [fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170)，專題架構與輸入政策是本書的設計選擇。

## 13.3 建置、執行與推演

在 examples 資料夾的 MSYS2 終端執行：

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -Wconversion -g gradebook.c -o gradebook.exe
./gradebook.exe scores.txt
```

輸入檔已提供三行 80、95、70。預期：

```text
count=3 min=70 max=95 mean=81.67 median=80.00 passes=3
```

總和 245、筆數 3，平均 245/3；排序後 70、80、95，中間索引 1 的值為 80。偶數筆如 0、100，兩中央值平均為 50。本書限制成績最高 100，所以兩中央值相加最大 200，在 int 中可表示。total 使用 unsigned long long；一百萬筆乘 100 為一億，小於該型別標準最低保證的最大值。

要小心容量仍可能超過有效長度。演算法只讀到 len，不讀 cap 末端的未初始化空間。qsort 只在 len>0 的分支呼叫，避免對空容器指標的可攜性作額外假設。

## 13.4 完整原始碼

以下程式與 examples/gradebook.c 相同，閱讀時先找 main 的流程，再回頭看各函式契約。

```c
/* Original C17 teaching example. Not compiled or executed in this environment. */
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RECORDS 1000000UL

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} Scores;

enum ReadStatus { READ_END, READ_OK, READ_INVALID, READ_ERROR };

/* Consume one whole physical line. Capacity includes the terminating zero.
   Reject embedded zero and excessive length; accept a final line without LF. */
static enum ReadStatus read_line(FILE *in, char *buffer, size_t cap)
{
    size_t used = 0;
    int bad = 0;
    int ch;
    while ((ch = fgetc(in)) != EOF && ch != '\n') {
        if (ch == '\0') {
            bad = 1;
        }
        if (used < cap - 1) {
            buffer[used++] = (char)ch;
        } else {
            bad = 1;
        }
    }
    if (ferror(in)) {
        return READ_ERROR;
    }
    if (ch == EOF && used == 0 && !bad) {
        return READ_END;
    }
    buffer[used] = '\0';
    return bad ? READ_INVALID : READ_OK;
}

static int parse_score(const char *text, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text || errno == ERANGE) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || value < 0 || value > 100) {
        return 0;
    }
    *out = (int)value;
    return 1;
}

/* On failure, the logical contents and ownership remain unchanged. */
static int append_score(Scores *s, int value)
{
    if (s->len >= MAX_RECORDS) {
        return 0;
    }
    if (s->len == s->cap) {
        size_t next_cap = 16;
        if (s->cap != 0) {
            if (s->cap > SIZE_MAX / 2) {
                return 0;
            }
            next_cap = s->cap * 2;
        }
        if (next_cap > SIZE_MAX / sizeof *s->data) {
            return 0;
        }
        int *next = realloc(s->data, next_cap * sizeof *s->data);
        if (next == NULL) {
            return 0;
        }
        s->data = next;
        s->cap = next_cap;
    }
    s->data[s->len++] = value;
    return 1;
}

static int compare_score(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: gradebook.exe FILE\n", stderr);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("open input");
        return 1;
    }

    Scores scores = {NULL, 0, 0};
    char line[128];
    size_t line_number = 0;
    int status = 0;
    for (;;) {
        enum ReadStatus r = read_line(in, line, sizeof line);
        if (r == READ_END) {
            break;
        }
        if (r == READ_ERROR) {
            fputs("input read failed\n", stderr);
            status = 1;
            break;
        }
        /* At most MAX_RECORDS valid lines are accepted before stopping. */
        if (line_number == SIZE_MAX) {
            fputs("too many input lines\n", stderr);
            status = 1;
            break;
        }
        ++line_number;
        int score;
        if (r == READ_INVALID || !parse_score(line, &score)) {
            fprintf(stderr, "invalid score at line %zu\n", line_number);
            status = 1;
            break;
        }
        if (!append_score(&scores, score)) {
            fputs("record limit or allocation failure\n", stderr);
            status = 1;
            break;
        }
    }
    if (fclose(in) == EOF) {
        fputs("input close failed\n", stderr);
        status = 1;
    }

    if (status == 0) {
        if (scores.len == 0) {
            if (puts("count=0 (no scores)") == EOF) {
                status = 1;
            }
        } else {
            unsigned long long total = 0;
            size_t passes = 0;
            for (size_t i = 0; i < scores.len; ++i) {
                total += (unsigned int)scores.data[i];
                if (scores.data[i] >= 60) {
                    ++passes;
                }
            }
            qsort(scores.data, scores.len, sizeof *scores.data, compare_score);
            size_t mid = scores.len / 2;
            double median = scores.data[mid];
            if (scores.len % 2 == 0) {
                median = (scores.data[mid - 1] + scores.data[mid]) / 2.0;
            }
            if (printf("count=%zu min=%d max=%d mean=%.2f median=%.2f passes=%zu\n",
                       scores.len, scores.data[0], scores.data[scores.len - 1],
                       (double)total / scores.len, median, passes) < 0) {
                status = 1;
            }
        }
        if (fflush(stdout) == EOF) {
            status = 1;
        }
    }
    free(scores.data);
    return status;
}
```

## 13.5 學習成果檢核與參考解答

1. 基礎：手算 0、59、60、100。解答：count=4、min=0、max=100、mean=54.75、median=59.50、passes=2。逐項對照，不只看平均。
2. 應用：第二行改成 `90x`。解答：stderr 應指出第 2 行，結束狀態 1，不列統計。檢核時分別觀察 stdout 與 stderr。
3. 應用：空檔、只有 `100` 且沒有最後換行。解答：前者顯示 count=0；後者是一筆合法資料，平均與中位數均為 100。這是第 5 章保守讀取器未支援、此處已加入的能力。
4. 挑戰：加入不及格百分比。解答：只在 len>0 時計算 `100.0 * (scores.len-passes) / scores.len`；空檔顯示無資料。先做浮點乘法，避免整數截斷與不必要的整數乘法溢位。
5. 挑戰：要保留原輸入順序又想算中位數，怎麼改？解答：另配置副本、複製有效元素後排序，再清理副本；多耗 O(n) 空間。或研究選擇演算法，但仍要交代是否破壞原資料。成功判準是說明資料修改與所有權，而不只貼函式名稱。

自學完成的判準：不看答案也能解釋 len/cap 不變量、三條失敗路徑的清理、偶數中位數公式，並自行新增一項統計。尚未執行的故障注入、Sanitizer 與跨平台測試不能宣稱已通過。

## 13.6 成本與下一步

載入與統計 O(n)，儲存 O(n)；排序成本依實作而定。若只需平均與極值，可串流統計而不保存全部資料，空間可降為 O(1)，但中位數就需要其他策略。對成績固定為 0–100 的特性，也可用 101 格頻率表求中位數，這是利用領域範圍換取效率的方向。

下一步：[深入語意與標準](chapters/14-expert.md)。

## 本章查閱與延伸

- [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170)：嚴格解析；[realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170)：所有權與失敗保留。
- [qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170)：比較器契約；[fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170)：關閉回傳值。


---

# 第 14 章　深入 C 的語意、並行與標準演進

先備：[第 13 章](chapters/13-project.md)。目標是把「在我電腦能跑」提升為「能說明語言保證與實作前提」，建立閱讀標準與研究邊界問題的方法。本章是深入研究的起點，並行片段不是可直接投入產品的執行緒框架。

## 14.1 四種問題不要混在一起

| 分類 | 含義 | 例子與處理 |
| --- | --- | --- |
| 有明確規定 | 符合前提時有指定結果 | 兩個可表示 int 的安全加法 |
| 實作定義 | 實作選擇且須文件說明 | plain char 的 signedness，讀平台文件 |
| 未指定 | 標準允許多種結果，不要求每次固定 | 不相依的函式引數求值先後，不依賴順序 |
| 未定義行為 UB | 標準不再提出要求 | 有號溢位、越界、資料競爭，先排除 |

分類依 [N1570 §3.4](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)。還要區分需要診斷的語法／限制違反與執行時 UB；「編譯器沒報錯」不是合法證明。

優先序決定如何組合運算式，不決定所有求值先後。`f(g(),h())` 不應依賴 g 或 h 誰先執行；若同時無順序地修改同一 scalar（純量，在 C17 包含算術型別與指標），可能超出「未指定」而成為 UB。像 `i = i++ + 1` 這種寫法，改成多個敘述使順序明確，比背誦每個版本的細節更可靠。

最佳化可以假設程式符合語言規則。例如以 signed x 寫 `x+1 > x`，不能把 x=INT_MAX 的溢位結果拿來證明一定是 false。硬體可能回繞與 C 程式是否有語言保證是兩個不同層次。

## 14.2 物件表示與有效型別

物件表示是實際佔用的位元組，值是型別賦予的解釋。對齊限制、padding、位元組順序與陷阱表示都可能影響存取。強制轉成 `float *` 後讀原本 int 物件，並不會把整數數值轉成浮點；還可能違反有效型別／對齊規則。[N1570 §6.5 p6–7](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) 列出允許的存取型別，包括字元型別的例外。

觀察表示可用下列完整 C17 例子，未實測：

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    unsigned int value = 1;
    unsigned char bytes[sizeof value];
    memcpy(bytes, &value, sizeof value);
    for (size_t i = 0; i < sizeof bytes; ++i) {
        printf("%02X%s", (unsigned int)bytes[i],
               i + 1 == sizeof bytes ? "\n" : " ");
    }
    return 0;
}
```

在常見 8-bit byte、32-bit unsigned int、小端系統，推演 `01 00 00 00`，但其他系統不保證相同。這是在觀察本機表示，不是可攜檔案格式。用 memcpy 避免不相容型別的直接存取，也不代表任意來源位元組都能安全解讀成任意目標型別。

配置取得的無宣告型別儲存區，其有效型別受寫入與複製方式影響。若要研究自訂配置器或泛型容器，需要精讀標準，而不是把所有物件視為可任意互換的 byte array。更嚴格對齊型別也可能需要專門配置介面。

## 14.3 restrict 是契約，不是加速按鈕

```c
/* Contract: n elements valid; written dst range does not overlap src range. */
void scale(int *restrict dst, const int *restrict src, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i] * 2;
    }
}
```

片段需 `<stddef.h>`，且每個 src 值乘 2 須可表示，未實測。此處給出比完整語意更容易檢查的「兩範圍不重疊」契約。呼叫 `scale(a,a,n)` 在 n>0 造成同資料透過不同 restricted 存取路徑被讀寫，違反要求；移除 restrict 才能重新考慮原地版本。不是指標數值永遠不能相等，而是執行期間對被修改物件的存取關係受到約束。[N1570 §6.7.3.1](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) 才是正式規則。

若 API 需要容許重疊，應換設計與實作，例如依方向處理或使用 memmove；不能靠關閉警告掩蓋契約違反。效能收益需量測，錯誤契約則會容許編譯器做出你未預期的轉換。

## 14.4 並行：原子性與可見性

執行緒可以同時執行不同工作。若不同執行緒對同物件有衝突存取，至少一個不是原子且沒有 happens-before 順序，會產生資料競爭。volatile 用於某些可觀察存取需求，不能取代執行緒同步；`counter++` 也不是自動原子。

以下是單次發布資料的**推理片段**，不是完整執行緒程式：

```c
#include <stdatomic.h>
static int payload;
static atomic_int ready = 0;

/* Producer, once only: */
/* payload = 42;
   atomic_store_explicit(&ready, 1, memory_order_release); */

/* Consumer, only read payload after observing 1: */
/* if (atomic_load_explicit(&ready, memory_order_acquire) == 1) {
       use(payload);
   } */
```

假設兩者啟動前初始化完成、只有一個 producer、payload 發布後永不再修改。consumer 的 acquire 若讀到這次 release 寫入的 1，就建立同步：發布前的 payload 寫入 happens-before 隨後讀取，因此可看到 42。若把兩邊改成 relaxed，ready 自己仍是原子，但不足以建立 payload 的發布順序；這是「原子旗標」和「整體協定正確」的差別。[N1570 §5.1.2.4、§7.17.3](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) 支持此推理。

本例沒有等待與排程保證，if 沒看到 1 就不讀，並不承諾一定何時取得資料。重複發布需要新協定，還牽涉物件生命期及回收。C17 的執行緒／原子支援可有條件性，先核對實作。正式專案通常先用互斥鎖，以簡單正確的設計作基準，再研究較弱的記憶體順序。

## 14.5 C23：有新功能，仍需看整套工具鏈

[WG14](https://open-std.org/jtc1/sc22/wg14/) 說明 C23 已於 2024 年採用。編譯器接受某語法，不代表標準函式庫的每個新標頭都可用。不要把 `-std=c23` 當作所有功能已完備的證明。

C23 引入 nullptr 與檢查整數運算標頭。以下是 **C23 專用完整範例**，未實測，不能用本書一般 C17 命令編譯：

```c
#include <limits.h>
#include <stdckdint.h>
#include <stdio.h>

int main(void)
{
    int result;
    if (ckd_add(&result, INT_MAX, 1)) {
        puts("overflow");
    } else {
        printf("%d\n", result);
    }
    return 0;
}
```

使用支援此標頭的實作，命令如 `gcc -std=c23 checked.c -o checked.exe`，推演 overflow。ckd_add 回傳 true 表示結果不能由目的型別正確表示；**不是** true 代表成功。功能與回傳規則核對 [N3096 §7.20](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf)。nullptr 對照 §6.3.2.3 與 §7.21.2；C17 章節仍用 NULL，不混用版本。

## 14.6 如何讀標準與評讀建議

先寫最小問題與前提，例如「指向同一陣列尾後一格，能否相減？」接著找術語、Constraints、Semantics、Returns，而不是只讀範例。區分規範要求與非規範備註，核對草案版本、正式修訂與缺陷報告。再用不同最佳化等級與不同實作觀察，但實驗只能展示某次結果，不能推翻標準對 UB 沒有保證的事實。

論壇適合找誤解與問題形狀。例如 [Are C arrays pointers?](https://www.reddit.com/r/C_Programming/comments/1d8z6o2/are_c_arrays_pointers/) 顯示學習者容易把轉換與型別等同；本書的語意結論仍由標準核對，不把留言票數當作證據。指標 provenance（指標與其來源儲存區的關聯）、儲存重用與精密別名推理仍是適合追蹤 WG14 提案與模型工作的方向，本書未系統整理這些研究，也不宣稱有單一簡單規則能解決所有情境。

## 14.7 練習與參考解答

1. 基礎：硬體測到 signed 溢位回繞，能否納入可攜程式契約？解答：不能，除非改用明確的實作擴充並標明依賴；ISO C 的 signed 溢位仍是 UB。
2. 應用：上面的 release/acquire 可以重複把 ready 設成 1 來發布新 payload 嗎？解答：不能直接推論。consumer 可能讀到舊的 1，且新 payload 寫入可能與讀取衝突；需要版本、確認或鎖等完整協定。
3. 挑戰：為什麼 memcpy 後的 bytes 可安全觀察，卻不代表可把外部任意 bytes 當 double 讀？解答：前者是複製既存有效物件的表示到 unsigned char；後者還需有效表示、大小、對齊與格式契約。兩者前提不同。
4. 挑戰：提出一份研究 restrict 的最小實驗。解答：各寫容許重疊與禁止重疊的 API，列出呼叫前提，再比較合法輸入在不同最佳化下的結果與生成碼；違約輸入僅用來辨識錯誤，不用來宣稱語言規則。達標需明示證據範圍。

完成後返回 [學習地圖](00-learning-map.md)，選擇系統、演算法、數值或編譯器分支，把下一個問題寫成可檢查的實作與語意契約。

## 本章查閱與延伸

- [N1570 C11 草案](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：本章閱讀 §3.4、§6.5 p6–7、§6.7.3.1、§5.1.2.4、§7.17.3 的相關段落。
- [N3096 C23 工作草案](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf)：§7.20 檢查整數運算與 nullptr 相關段落；未閱讀全部 761 頁。
- [WG14 官方狀態](https://open-std.org/jtc1/sc22/wg14/)：正式標準狀態；[GCC 方言選項](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html)：實作選項。
- [陣列論壇討論](https://www.reddit.com/r/C_Programming/comments/1d8z6o2/are_c_arrays_pointers/)：只作學習疑難案例，不支持技術規範。


---

# 術語表

| 術語 | 原文 | 白話解釋 | 回讀 |
| --- | --- | --- | --- |
| 原始碼 | source code | 供編譯器讀取的程式文字 | [1](chapters/01-start.md) |
| 編譯器 | compiler | 分析程式並產生目標形式的工具 | [1](chapters/01-start.md) |
| 連結器 | linker | 組合物件檔與必要定義 | [9](chapters/09-modules.md) |
| 翻譯單元 | translation unit | 前處理後可分別編譯的一份內容 | [9](chapters/09-modules.md) |
| 物件 | object | 有型別、值與生命期的資料儲存區，不是物件導向類別的同義詞 | [2](chapters/02-types.md) |
| 初始化 | initialization | 建立物件時給定初值 | [2](chapters/02-types.md) |
| 賦值 | assignment | 把值存入已存在的可修改目標 | [2](chapters/02-types.md) |
| 型別轉換 | conversion | 把某型別的值轉成另一型別 | [2](chapters/02-types.md) |
| 溢位 | overflow | 數學結果超出目的型別可表示範圍 | [11](chapters/11-debug.md) |
| 短路 | short-circuit evaluation | 邏輯結果已確定時，不計算另一側 | [3](chapters/03-control.md) |
| 不變量 | invariant | 在指定程序節點持續成立的條件 | [3](chapters/03-control.md) |
| 參數／引數 | parameter / argument | 函式宣告的接收位置／呼叫時提供的運算式 | [4](chapters/04-functions.md) |
| 契約 | contract | 函式前提、保證、錯誤與資源責任的約定 | [4](chapters/04-functions.md) |
| 遞迴 | recursion | 把問題縮小後再次呼叫相同工作 | [4](chapters/04-functions.md) |
| 陣列 | array | 連續排列的同型別元素 | [5](chapters/05-arrays.md) |
| 緩衝區 | buffer | 暫存資料的儲存空間，要另知其容量 | [5](chapters/05-arrays.md) |
| 零結尾字串 | null-terminated string | 以第一個零字元界定結尾的字元序列 | [5](chapters/05-arrays.md) |
| 指標 | pointer | 能指向物件或函式、也可為空等狀態的型別與值 | [6](chapters/06-pointers.md) |
| 解參照 | dereference | 透過指標指定其目標 | [6](chapters/06-pointers.md) |
| 作用域／生命期 | scope / lifetime | 名稱可見的範圍／物件存在的期間 | [6](chapters/06-pointers.md) |
| 懸空指標 | dangling pointer | 原目標已結束生命期的指標關係，不能繼續使用舊值 | [8](chapters/08-memory.md) |
| 結構／聯集 | struct / union | 多成員組合／多成員共用儲存區 | [7](chapters/07-records.md) |
| 填補／對齊 | padding / alignment | 為布局加入的空間／型別存放位置的要求 | [14](chapters/14-expert.md) |
| 所有權／借用 | ownership / borrowing | 誰負責清理／只在允許期間使用他人資源的約定 | [8](chapters/08-memory.md) |
| 長度／容量 | length / capacity | 有效元素數／已配置可容納元素數 | [8](chapters/08-memory.md) |
| 回呼 | callback | 傳入函式指標，由另一段流程呼叫 | [12](chapters/12-algorithms.md) |
| 攤銷分析 | amortized analysis | 分析一串操作的總成本，再分攤每次操作 | [12](chapters/12-algorithms.md) |
| 有效型別 | effective type | 用來限制物件值可透過哪些型別存取的規則 | [14](chapters/14-expert.md) |
| 未定義行為 | undefined behavior, UB | 標準對該情境不再提出要求 | [14](chapters/14-expert.md) |
| 原子操作 | atomic operation | 依記憶體模型提供不可分割存取等保證的操作 | [14](chapters/14-expert.md) |
| 先行發生 | happens-before | 推理跨執行緒可見性與資料競爭的順序關係 | [14](chapters/14-expert.md) |

這是本書用語索引與教學簡化，正式定義及已讀來源請回到各章與 [來源總表](references.md)。


---

# 速查、任務索引與常見問題

本附錄整理正文，不取代函式前置條件。所有命令仍屬未實測示範。

## 日常命令

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -g file.c -o app.exe
./app.exe
gcc -std=c17 -Wall -Wextra -Wpedantic main.c stats.c -o stats.exe
```

Sanitizer 使用條件及完整命令見 [第 11 章](chapters/11-debug.md)。C23 範例另見 [第 14 章](chapters/14-expert.md)。

## 型別與輸出格式

| 資料 | 常見宣告 | printf 格式 | 注意 |
| --- | --- | --- | --- |
| 一般整數 | int n | %d | 不假定固定 32-bit |
| 無號整數 | unsigned int n | %u | 回繞有規則，仍可能是邏輯錯誤 |
| 大小 | size_t n | %zu | signed／unsigned 混用要檢查 |
| 寬範圍非負整數 | unsigned long long n | %llu | 仍有最大值 |
| 浮點 | double x | %.2f | 近似值，顯示精度不是運算精度 |
| 字串 | const char *s | %s | 必須有效且零結尾 |
| 物件指標 | int *p | %p，傳 (void *)p | 只用於診斷，不推論可攜格式 |

## 常見工作對照

| 我想做什麼 | 起點 | 核心檢查 |
| --- | --- | --- |
| 算平均 | [2](chapters/02-types.md)、[6](chapters/06-pointers.md) | 分母非零，先轉浮點再除 |
| 遍歷陣列 | [5](chapters/05-arrays.md) | i < len，避免讀未初始化格 |
| 改呼叫端變數 | [6](chapters/06-pointers.md) | 傳位址，目標仍活著且可寫 |
| 把姓名與成績綁一起 | [7](chapters/07-records.md) | struct、字串容量、複製語意 |
| 接收未知筆數 | [8](chapters/08-memory.md) | len/cap 不變量，先驗證乘法 |
| 分檔 | [9](chapters/09-modules.md) | 宣告放標頭、定義放實作、一起連結 |
| 讀整數 | [10](chapters/10-io.md) | strtol、end、ERANGE、領域範圍 |
| 排序 | [12](chapters/12-algorithms.md) | 比較器一致，不直接用可能溢位的差 |
| 讀完整檔案並分析 | [13](chapters/13-project.md) | 完整行、錯誤分支、清理 |
| 判斷最佳化是否改壞程式 | [14](chapters/14-expert.md) | 先查 UB 與契約，再比較實作 |

## 問題與原因

- **平均總是少了小數？** 整數除法先發生；改 `total / 2.0`，不能只改接收變數型別。
- **陣列傳入函式後大小變小？** 參數調整成指標；另外傳元素數，不用 sizeof 猜。
- **兩個相同文字用 == 卻不相等？** 指標相等不是字串內容相等；用 strcmp 並確認字串有效。
- **realloc 後另一個指標壞了？** 舊借用指標可能已失效；保存索引並從新配置重新取址。
- **讀檔最後多一筆？** 常因 while(!feof(...))；改以實際讀取結果控制迴圈。
- **程式不當機，所以越界沒關係？** 錯誤可能尚未顯現；非法存取沒有語言保證。
- **free 後設 NULL 是否足夠？** 只更新該變數；其他別名仍須停止使用。
- **用了 const 就能多執行緒讀寫？** const 不是同步工具；見記憶體模型與資料競爭。

## 標準標頭速查

| 標頭 | 本書使用 |
| --- | --- |
| stdio.h | FILE、printf、fgets、fgetc、fopen、fclose |
| stdlib.h | malloc、realloc、free、strtol、qsort |
| stddef.h | size_t、NULL |
| string.h | strlen、strcmp、strcspn、memcpy |
| limits.h | INT_MIN、INT_MAX、CHAR_BIT |
| stdint.h | SIZE_MAX 與固定／至少寬度整數型別 |
| errno.h | errno、ERANGE |
| ctype.h | isspace，參數域須合法 |
| assert.h | assert，可能被 NDEBUG 移除 |
| stdatomic.h | C11 起的原子操作，先核對實作支援 |
| stdckdint.h | C23 檢查整數運算，正文 C17 不使用 |

語法與 API 細節可由 [來源表](references.md) 及各章外部連結核對。這裡的任務與 FAQ 均連回正文，不另列未實際閱讀的推薦書目。


---

# 來源、閱讀範圍與限制

查閱日期統一為 **2026-11-15（Asia/Taipei）**。沒有把搜尋摘要或 AI 摘要當作技術證據，也沒有宣稱閱讀標準草案全文。

## 搜尋方式

實際在背景瀏覽器開啟 Google，閱讀搜尋結果；查詢為 `C language beginner official documentation` 與 `C語言 入門 標準 指標 未定義行為`。網頁工具無法直接讀 Google 搜尋頁，改由瀏覽器確認成功。後續以一般網頁搜尋工具補查標準、GCC、MSYS2、Aspnes 教材、Clang 與 CERT；此工具未揭露搜尋引擎，**不稱為 Google 搜尋**。

補查包含 `site.open-std.org jtc1 sc22 wg14 n1570 n3096 C standard`、`site.gcc.gnu.org onlinedocs C Dialect Options`、`site.cs.yale.edu aspnes C programming notes`、`site.msys2.org docs environments ucrt64`、Clang sanitizers、Microsoft C 語言文件與 CERT 記憶體規則。Google 用於找入門、官方文件與常見誤解線索；下表是實際開啟且閱讀相關內容的原始資料。

## 已讀及核對來源

日期未能由已讀內容確定時記「未提供」，不以搜尋爬取日期當更新日期。Microsoft 頁面雖顯示通用授權提示，本文仍可讀；僅使用實際可見正文。每個來源只聲明表列閱讀範圍。

| 編號 | 原始來源 | 作者／機構 | 日期／版本 | 類型與已讀範圍 | 用途／章節 | 限制 |
| --- | --- | --- | --- | --- | --- | --- |
| S01 | [Getting Started](https://www.msys2.org/#installation) | MSYS2 | 頁面日期未提供；查閱時安裝器標示 20260927 | 官方文件，Installation 步驟與 GCC 套件命令 | 1 | 未實際安裝；本書命令為 x64 |
| S02 | [Environments](https://www.msys2.org/docs/environments/) | MSYS2 | 頁面版本未提供；可見 2026-03-15 變更紀錄 | 官方文件，Overview、UCRT64 與工具鏈差異 | 1 | 不泛用至所有 Windows 架構 |
| S03 | [C Dialect Options](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html) | GCC／GNU | 線上版本，未提供頁面更新日 | 官方文件，-std、c11/c17/c23、GNU 方言、hosted | 前言、1、14 | 預設值隨工具版本變動，本書固定 -std=c17 |
| S04 | [Type Conversions (C)](https://learn.microsoft.com/en-us/cpp/c-language/type-conversions-c?view=msvc-170) | Microsoft | 2021-08-03；msvc-170 | 官方文件，本文的轉換時機與整數提升 | 2 | 實作文件，不當成全部標準語意 |
| S05 | [if Statement (C)](https://learn.microsoft.com/en-us/cpp/c-language/if-statement-c?view=msvc-170) | Microsoft | 2023-01-25；msvc-170 | 官方文件，分支與巢狀 else 範例 | 3 | 不照抄其過度簡化的條件型別措辭 |
| S06 | [for Statement (C)](https://learn.microsoft.com/en-us/cpp/c-language/for-statement-c?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，迴圈順序、break、continue | 3 | 只取執行規則，不照抄原範例 |
| S07 | [C/Functions](https://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Functions.html) | James Aspnes／Yale | 靜態筆記 2003–2012；頁面個別日期未提供 | 原作者大學教材，函式定義、傳值、原型與呼叫 | 4、9 | 歷史教材；舊式括號寫法只作版本對照 |
| S08 | [Array Declarations](https://learn.microsoft.com/en-us/cpp/c-language/array-declarations?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，陣列宣告、多維陣列、列儲存 | 5 | 「array considered a pointer」措辭以 N1570 轉換規則修正解讀 |
| S09 | [Pointer Declarations](https://learn.microsoft.com/en-us/cpp/c-language/pointer-declarations?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，指標宣告、void 與不完整型別段落 | 6 | 不採其概括 const 說法替代逐層型別分析 |
| S10 | [Structure Declarations](https://learn.microsoft.com/en-us/cpp/c-language/structure-declarations?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，成員、tag、不完整結構與定義 | 7 | Microsoft 特定布局不視為可攜保證 |
| S11 | [C Bitwise Operators](https://learn.microsoft.com/en-us/cpp/c-language/c-bitwise-operators?view=msvc-170) | Microsoft | 2022-04-07；msvc-170 | 官方文件，AND／OR／XOR 與例子 | 7 | 不採 signed 的平台特定結論 |
| S12 | [fgets, fgetws](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，fgets 簽章、回傳、容量減一、保留換行與零結尾 | 5、10 | fgetws 與 Unicode 擴充未作本書規範 |
| S13 | [realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，非零失敗、搬移、內容保留及相容性注意 | 8、13 | 零大小行為與 C17／C23 不宜混用；本書避開 |
| S14 | [MEM35-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem35-c/) | Carnegie Mellon SEI CERT | 日期未提供；線上 C coding standard | 原機構規則，容量不足與 sizeof／SIZE_MAX 範例 | 8 | 規則指南不取代 ISO 標準 |
| S15 | [MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/) | Carnegie Mellon SEI CERT | 日期未提供；線上 C coding standard | 原機構規則，釋放後指標不確定值與重複釋放段落 | 8、12 | 未系統審閱全套 CERT 規則 |
| S16 | [#include](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，文字引入、標頭用途與搜尋區別 | 9 | 路徑順序屬實作，未套用到所有工具鏈 |
| S17 | [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，簽章、end_ptr、停止位置、ERANGE | 10、13 | 不使用 _l 擴充及無效參數處理器作 ISO 保證 |
| S18 | [fopen](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，回傳、讀寫模式與文字／二進位差異 | 10、13 | Windows 路徑與 Unicode 模式屬平台議題 |
| S19 | [fread](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，元素寬度與數目、短讀、部分元素 | 10 | 不把 MS 參數驗證視為標準保證 |
| S20 | [fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170) | Microsoft | 2022-12-02；msvc-170 | 官方文件，關閉、緩衝與回傳值 | 10、13 | _fcloseall 不用於本書程式 |
| S21 | [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html) | LLVM／Clang | 日期未提供；線上文件 | 官方文件，Introduction、Usage、Limitations、Supported Platforms | 11 | 沒有執行；Windows 支援與 runtime 需另外核對 |
| S22 | [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html) | LLVM／Clang | 日期未提供；線上文件 | 官方文件，Introduction、Usage、Available checks | 11 | 非所有 UB 都可檢出，未實測 |
| S23 | [qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，參數、比較器、相等順序 | 12、13 | 不從 Microsoft 的實作描述推論 ISO 演算法保證 |
| S24 | [Notes on Data Structures and Programming Techniques](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html) | James Aspnes／Yale | Spring 2022；頁首時間戳 2025-04-27 | 原作者教材，目錄、§4.9.8 函式指標／回呼、realloc 範例及輸入相關段落 | 8、12；11 僅導讀 | 未讀全書；原 realloc 直接指派及差值比較器不作可靠寫法示範 |
| S25 | [WG14 N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) | ISO/IEC JTC1/SC22/WG14 | 2011-04-12；C11 committee draft | 標準草案，選讀 §3.4、§6.2.4、§6.3.2.1、§6.5 p2/p5/p6/p7、§6.5.3.4、§6.5.5–6、§6.7.3.1、§6.7.6.3、§7.1.1、§7.22.1.4、§5.1.2.4、§7.17.3 | 2、4–6、8、14 | 非正式 C17 全文；未讀全部 701 頁，未逐項核對後續缺陷修正 |
| S26 | [WG14 N3096](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf) | ISO/IEC JTC1/SC22/WG14 | 2023-04-01；C23 working draft | 標準草案，封面、nullptr 轉換與型別、§7.20 checked arithmetic | 14 | 非最終出版標準，未讀全部 761 頁 |
| S27 | [WG14 官方頁](https://open-std.org/jtc1/sc22/wg14/) | WG14 | 頁面標示 2025-11-10；C23 狀態 | 標準機構，已讀標準採用狀態與草案／專案入口 | 前言、14 | 未取得付費正式全文 |
| S28 | [Are C arrays pointers?](https://www.reddit.com/r/C_Programming/comments/1d8z6o2/are_c_arrays_pointers/) | 提問者 TimFrod；r/C_Programming 參與者 | 精確發文日期未由已讀正文取得；顯示約 2 年前 | 論壇，已讀問題與前段陣列／轉換討論 | 14；5–6 的疑難線索 | 未標明 C 版本；留言措辭與成效經驗未視為已證實技術結論 |

所有條目查閱日皆為上列統一日期。程式、成績、練習、契約、推導與整合專題為本書原創教學內容；外部文件用來核對語意與工具行為，不代表它們審核或背書本書。

## 未採用與來源缺口

GNU C manual 的幾個頁面在此工具讀取逾時，僅取得搜尋線索，未列為已讀引用。Microsoft 的 Functions Definitions／Function Declarators 嘗試未成功，改用 Aspnes 原作者筆記與 N1570。沒有把這些失敗網址當成章末已讀來源。

沒有取得 ISO C17 正式全文；以公開 C11 草案核對穩定語意，並由 GCC 文件確認 C17 的修正關係。嚴格標準爭議、缺陷報告演進、完整 C23 差異、指標 provenance 文獻、無鎖記憶體回收不在本次已查證範圍。

## 編輯審閱範圍

已回讀第 1–14 章完成的解說、範例與解答，重點為先備銜接、傳值與指標、陣列大小、字串邊界、realloc 失敗保留、整數檢查、空資料、比較器及發布協定。補充了條件運算子、runtime、scalar 與 provenance 的首次解釋。人工推演了各章顯示結果、三筆與四筆專題統計；這些不是執行結果。

另以 Node.js 檢查書內相對連結目標存在、Markdown 程式圍欄成對、14 章各有練習／解答與章末網路來源，並核對專題內嵌程式與獨立 .c 檔一致。來源核對僅限表列已讀段落，不聲稱外部文件全數全文審閱。範例尚無 C 編譯／執行結果，不以 Markdown 格式或連結通過代替程式正確性；安裝、Sanitizer、GDB、跨平台與故障注入均未實測，未進行真人學習成效驗證。
