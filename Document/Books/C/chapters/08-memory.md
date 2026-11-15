# 第 8 章　動態記憶體與所有權

先備：[第 7 章](07-records.md)。目標是處理執行時才知道的資料量，並在成功與失敗路徑都交代清楚資源。固定大小陣列很簡單，但不能無限制接收檔案；動態配置能調整容量，也把清理責任交給你。

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

下一步：[把作品拆成模組](09-modules.md)。

## 本章查閱與延伸

- [realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170)：非零失敗、內容保留、新增範圍未初始化；Microsoft 零大小行為不採用。
- [MEM35-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem35-c/)：配置大小計算。
- [MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/)：懸空指標與釋放後存取。
