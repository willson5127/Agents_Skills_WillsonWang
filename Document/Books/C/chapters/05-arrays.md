# 第 5 章　陣列、字串與文字輸入

先備：[第 4 章](04-functions.md)。目標是保存多筆同型別資料、理解字串終止符與容量。上一章資料只活在單一變數；現在我們要讓每筆成績能被再次查看。

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

下一步：[指標與陣列的真正關係](06-pointers.md)。

## 本章查閱與延伸

- [Microsoft 陣列宣告](https://learn.microsoft.com/en-us/cpp/c-language/array-declarations?view=msvc-170)：宣告、二維陣列與 row-major；簡化措辭的限制如正文。
- [fgets](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-170)：讀取上限、換行、終止字元與回傳值。
- [N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：§6.5.3.4 的陣列大小與指標參數差別。
