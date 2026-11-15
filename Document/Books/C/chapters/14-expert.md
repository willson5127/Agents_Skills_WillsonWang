# 第 14 章　深入 C 的語意、並行與標準演進

先備：[第 13 章](13-project.md)。目標是把「在我電腦能跑」提升為「能說明語言保證與實作前提」，建立閱讀標準與研究邊界問題的方法。本章是深入研究的起點，並行片段不是可直接投入產品的執行緒框架。

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

完成後返回 [學習地圖](../00-learning-map.md)，選擇系統、演算法、數值或編譯器分支，把下一個問題寫成可檢查的實作與語意契約。

## 本章查閱與延伸

- [N1570 C11 草案](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：本章閱讀 §3.4、§6.5 p6–7、§6.7.3.1、§5.1.2.4、§7.17.3 的相關段落。
- [N3096 C23 工作草案](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf)：§7.20 檢查整數運算與 nullptr 相關段落；未閱讀全部 761 頁。
- [WG14 官方狀態](https://open-std.org/jtc1/sc22/wg14/)：正式標準狀態；[GCC 方言選項](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html)：實作選項。
- [陣列論壇討論](https://www.reddit.com/r/C_Programming/comments/1d8z6o2/are_c_arrays_pointers/)：只作學習疑難案例，不支持技術規範。
