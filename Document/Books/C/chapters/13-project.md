# 第 13 章　完整專題：檔案成績分析器

先備：[第 1–12 章](../README.md)。目標是完成能解釋、修改與檢查的作品。原始碼是本次原創，完整檔案為 [gradebook.c](../examples/gradebook.c)，不是需要讀者自行補齊的骨架。所有程式及下列命令未實測，預期結果由人工推演。

## 13.1 先決定輸入與失敗行為

每行一個十進位整數，範圍 0–100，可有前後空白；拒絕空白行、尾端垃圾、內嵌零與過長行。每行換行前至多 127 個讀取字元，最後一行可沒有 LF；CRLF 在文字模式的轉換依平台而定，剩餘 CR 也可作尾端空白處理。不接受 UTF-8 BOM。最多一百萬筆，實際可配置量可能更小。

成功顯示筆數、最小、最大、平均、中位數與及格數。空檔顯示沒有資料，不除以零。第一個錯誤即停止，不輸出部分統計，以免讀者把不完整結果當成全檔結果。成功結束狀態為 0，其他為 1；程式只讀取輸入檔，不建立報表檔。

## 13.2 資料流與責任

```text
檔案 → read_line → parse_score → append_score → 統計／排序 → 標準輸出
          完整行       值是否合法      所有權及容量        明確處理空資料
```

Scores 把 data、len、cap 綁在一起；main 是其唯一所有者，最後統一 free。read_line 用 fgetc 逐字元讀取，能同時判斷內嵌零、容量與實體行結尾；超長後仍讀到換行或 EOF，避免留下半行。fgetc 用 int 接結果，因為必須同時表示字元值與 EOF。這比第 5 章的保守 fgets 範例多了狀態，但清楚交代了邊界。

parse_score 重用第 10 章的分層檢查；append_score 重用第 8 章的暫存 realloc 與乘法前置檢查。只有所有輸入與關檔都成功才統計。標準 I/O 可能緩衝，所以最後還檢查 fflush(stdout)。有錯誤時無法保證終端一定顯示完整診斷，例如 stderr 本身也失效；程式仍以非零狀態回報失敗。

這些 API 的行為依據是 [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170)、[realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170)、[qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170) 與 [fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170)，專題架構與輸入政策是本書的設計選擇。

## 13.3 建置、執行與推演

在 examples 資料夾的 MSYS2 終端執行：

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -Wconversion -g gradebook.c -o gradebook.exe
./gradebook.exe scores.txt
```

輸入檔已提供三行 80、95、70。預期：

```text
count=3 min=70 max=95 mean=81.67 median=80.00 passes=3
```

總和 245、筆數 3，平均 245/3；排序後 70、80、95，中間索引 1 的值為 80。偶數筆如 0、100，兩中央值平均為 50。本書限制成績最高 100，所以兩中央值相加最大 200，在 int 中可表示。total 使用 unsigned long long；一百萬筆乘 100 為一億，小於該型別標準最低保證的最大值。

要小心容量仍可能超過有效長度。演算法只讀到 len，不讀 cap 末端的未初始化空間。qsort 只在 len>0 的分支呼叫，避免對空容器指標的可攜性作額外假設。

## 13.4 完整原始碼

以下程式與 examples/gradebook.c 相同，閱讀時先找 main 的流程，再回頭看各函式契約。

```c
/* Original C17 teaching example. Not compiled or executed in this environment. */
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RECORDS 1000000UL

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} Scores;

enum ReadStatus { READ_END, READ_OK, READ_INVALID, READ_ERROR };

/* Consume one whole physical line. Capacity includes the terminating zero.
   Reject embedded zero and excessive length; accept a final line without LF. */
static enum ReadStatus read_line(FILE *in, char *buffer, size_t cap)
{
    size_t used = 0;
    int bad = 0;
    int ch;
    while ((ch = fgetc(in)) != EOF && ch != '\n') {
        if (ch == '\0') {
            bad = 1;
        }
        if (used < cap - 1) {
            buffer[used++] = (char)ch;
        } else {
            bad = 1;
        }
    }
    if (ferror(in)) {
        return READ_ERROR;
    }
    if (ch == EOF && used == 0 && !bad) {
        return READ_END;
    }
    buffer[used] = '\0';
    return bad ? READ_INVALID : READ_OK;
}

static int parse_score(const char *text, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text || errno == ERANGE) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || value < 0 || value > 100) {
        return 0;
    }
    *out = (int)value;
    return 1;
}

/* On failure, the logical contents and ownership remain unchanged. */
static int append_score(Scores *s, int value)
{
    if (s->len >= MAX_RECORDS) {
        return 0;
    }
    if (s->len == s->cap) {
        size_t next_cap = 16;
        if (s->cap != 0) {
            if (s->cap > SIZE_MAX / 2) {
                return 0;
            }
            next_cap = s->cap * 2;
        }
        if (next_cap > SIZE_MAX / sizeof *s->data) {
            return 0;
        }
        int *next = realloc(s->data, next_cap * sizeof *s->data);
        if (next == NULL) {
            return 0;
        }
        s->data = next;
        s->cap = next_cap;
    }
    s->data[s->len++] = value;
    return 1;
}

static int compare_score(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: gradebook.exe FILE\n", stderr);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("open input");
        return 1;
    }

    Scores scores = {NULL, 0, 0};
    char line[128];
    size_t line_number = 0;
    int status = 0;
    for (;;) {
        enum ReadStatus r = read_line(in, line, sizeof line);
        if (r == READ_END) {
            break;
        }
        if (r == READ_ERROR) {
            fputs("input read failed\n", stderr);
            status = 1;
            break;
        }
        /* At most MAX_RECORDS valid lines are accepted before stopping. */
        if (line_number == SIZE_MAX) {
            fputs("too many input lines\n", stderr);
            status = 1;
            break;
        }
        ++line_number;
        int score;
        if (r == READ_INVALID || !parse_score(line, &score)) {
            fprintf(stderr, "invalid score at line %zu\n", line_number);
            status = 1;
            break;
        }
        if (!append_score(&scores, score)) {
            fputs("record limit or allocation failure\n", stderr);
            status = 1;
            break;
        }
    }
    if (fclose(in) == EOF) {
        fputs("input close failed\n", stderr);
        status = 1;
    }

    if (status == 0) {
        if (scores.len == 0) {
            if (puts("count=0 (no scores)") == EOF) {
                status = 1;
            }
        } else {
            unsigned long long total = 0;
            size_t passes = 0;
            for (size_t i = 0; i < scores.len; ++i) {
                total += (unsigned int)scores.data[i];
                if (scores.data[i] >= 60) {
                    ++passes;
                }
            }
            qsort(scores.data, scores.len, sizeof *scores.data, compare_score);
            size_t mid = scores.len / 2;
            double median = scores.data[mid];
            if (scores.len % 2 == 0) {
                median = (scores.data[mid - 1] + scores.data[mid]) / 2.0;
            }
            if (printf("count=%zu min=%d max=%d mean=%.2f median=%.2f passes=%zu\n",
                       scores.len, scores.data[0], scores.data[scores.len - 1],
                       (double)total / scores.len, median, passes) < 0) {
                status = 1;
            }
        }
        if (fflush(stdout) == EOF) {
            status = 1;
        }
    }
    free(scores.data);
    return status;
}
```

## 13.5 學習成果檢核與參考解答

1. 基礎：手算 0、59、60、100。解答：count=4、min=0、max=100、mean=54.75、median=59.50、passes=2。逐項對照，不只看平均。
2. 應用：第二行改成 `90x`。解答：stderr 應指出第 2 行，結束狀態 1，不列統計。檢核時分別觀察 stdout 與 stderr。
3. 應用：空檔、只有 `100` 且沒有最後換行。解答：前者顯示 count=0；後者是一筆合法資料，平均與中位數均為 100。這是第 5 章保守讀取器未支援、此處已加入的能力。
4. 挑戰：加入不及格百分比。解答：只在 len>0 時計算 `100.0 * (scores.len-passes) / scores.len`；空檔顯示無資料。先做浮點乘法，避免整數截斷與不必要的整數乘法溢位。
5. 挑戰：要保留原輸入順序又想算中位數，怎麼改？解答：另配置副本、複製有效元素後排序，再清理副本；多耗 O(n) 空間。或研究選擇演算法，但仍要交代是否破壞原資料。成功判準是說明資料修改與所有權，而不只貼函式名稱。

自學完成的判準：不看答案也能解釋 len/cap 不變量、三條失敗路徑的清理、偶數中位數公式，並自行新增一項統計。尚未執行的故障注入、Sanitizer 與跨平台測試不能宣稱已通過。

## 13.6 成本與下一步

載入與統計 O(n)，儲存 O(n)；排序成本依實作而定。若只需平均與極值，可串流統計而不保存全部資料，空間可降為 O(1)，但中位數就需要其他策略。對成績固定為 0–100 的特性，也可用 101 格頻率表求中位數，這是利用領域範圍換取效率的方向。

下一步：[深入語意與標準](14-expert.md)。

## 本章查閱與延伸

- [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170)：嚴格解析；[realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170)：所有權與失敗保留。
- [qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170)：比較器契約；[fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170)：關閉回傳值。
