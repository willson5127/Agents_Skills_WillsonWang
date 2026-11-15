# 第 2 章　變數、型別與運算

先備：[第 1 章](01-start.md)。目標是表示資料、預測運算的型別，寫出正確平均值。假設兩次成績是 80 與 95，若電腦顯示平均 87，你應先問「它用了什麼型別算除法」，而不是改顯示的小數位數。

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

下一步：[用條件與迴圈控制運算](03-control.md)。

## 本章查閱與延伸

- [Microsoft 型別轉換](https://learn.microsoft.com/en-us/cpp/c-language/type-conversions-c?view=msvc-170)：整數提升與轉換時機。
- [WG14 N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：查 §6.3 轉換、§6.5.5 除法與餘數；這是 C11 草案，閱讀範圍見來源總表。
