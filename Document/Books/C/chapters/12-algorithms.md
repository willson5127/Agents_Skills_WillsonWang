# 第 12 章　資料結構、演算法與效能

先備：[第 11 章](11-debug.md)。目標是依工作負載選擇資料結構、寫比較器、推導時間與空間成本。若只要平均值，單次掃描就足夠；若要反覆查名次或中位數，排序與額外儲存才有價值。

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

下一步：[完成可讀檔的作品](13-project.md)。

## 本章查閱與延伸

- [qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170)：參數、比較結果、相等項順序。
- [Aspnes 教材 §4.9.8](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html)：函式指標與回呼；本章攤銷推導、結構比較與範例為原創。
- [CERT MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/)：節點釋放後不可再讀其 next。
