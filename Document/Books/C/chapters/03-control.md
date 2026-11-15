# 第 3 章　條件、迴圈與不變量

先備：[第 2 章](02-types.md)。目標是讓程式依資料選路、重複工作，並用不變量解釋為什麼結果可信。統計成績時，你不會為每位學生手寫一套程式；你需要一個可重複的步驟。

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

下一步：[把可重複邏輯抽成函式](04-functions.md)。

## 本章查閱與延伸

- [Microsoft if](https://learn.microsoft.com/en-us/cpp/c-language/if-statement-c?view=msvc-170)：條件、分支、懸空 else。
- [Microsoft for](https://learn.microsoft.com/en-us/cpp/c-language/for-statement-c?view=msvc-170)：執行順序、break、continue；不變量證明是本書的教學推導。
