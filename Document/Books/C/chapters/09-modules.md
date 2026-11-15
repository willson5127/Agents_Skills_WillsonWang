# 第 9 章　標頭、分檔與前處理器

先備：[第 8 章](08-memory.md)。目標是分開公開介面與實作，從多個原始檔建置程式。當所有程式塞進 main，修改統計規則時容易碰壞輸入；分檔讓責任邊界可見，也讓編譯器幫忙核對介面。

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

下一步：[讓資料進出程式](10-io.md)。

## 本章查閱與延伸

- [Microsoft #include](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp?view=msvc-170)：引入標頭的機制；平台搜尋順序須查實際編譯器。
- [Aspnes Functions](https://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Functions.html)：宣告與定義的教學背景；本章模組與巨集反例為本書原創。
