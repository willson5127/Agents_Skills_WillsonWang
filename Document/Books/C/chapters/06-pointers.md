# 第 6 章　指標、陣列轉換與生命期

先備：[第 5 章](05-arrays.md)。目標是透過指標改動呼叫端資料，並判斷可存取範圍與生命期。單靠「指標存位址」仍不足以保證安全：你還必須知道它指向哪個物件、能讀寫幾個元素、物件是否仍存在。

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

下一步：[組合不同型別資料](07-records.md)。

## 本章查閱與延伸

- [Microsoft 指標宣告](https://learn.microsoft.com/en-us/cpp/c-language/pointer-declarations?view=msvc-170)：指向物件、陣列與其他指標的宣告。
- [N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)：已讀 §6.2.4 生命期、§6.5.3.4 sizeof、§6.5.6 指標算術相關段落。
