# 第 7 章　結構、列舉、聯集與位元

先備：[第 6 章](06-pointers.md)。目標是把姓名與成績合成記錄，表示有種類的值，並用位元旗標記錄多個開關。不同欄位如果分散在兩個陣列，一次排序可能只移動成績而忘記姓名；結構讓相關資料成為一個整體。

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

下一步：[自己管理記憶體](08-memory.md)。

## 本章查閱與延伸

- [結構宣告](https://learn.microsoft.com/en-us/cpp/c-language/structure-declarations?view=msvc-170)：成員、結構型別與不完整型別。
- [位元運算](https://learn.microsoft.com/en-us/cpp/c-language/c-bitwise-operators?view=msvc-170)：AND／OR／XOR。本章 tagged union 與記錄案例為教學設計。
