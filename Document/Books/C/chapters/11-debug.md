# 第 11 章　除錯、測試與可靠性

先備：[第 10 章](10-io.md)。目標是把「有時怪怪的」變成可重現的問題，並理解工具能證明什麼。看到當機位置不一定就是根因：較早的越界寫入可能直到後面配置記憶體時才顯現。

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

下一步：[選擇資料結構與演算法](12-algorithms.md)。

## 本章查閱與延伸

- [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html)：Introduction、Usage 與平台限制段落。
- [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)：Introduction、Usage、Available checks。
- [Aspnes 教材](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html)：查 Debugging 與 Assertions；本書邊界測試及加法推導為原創。
