# 速查、任務索引與常見問題

本附錄整理正文，不取代函式前置條件。所有命令仍屬未實測示範。

## 日常命令

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -g file.c -o app.exe
./app.exe
gcc -std=c17 -Wall -Wextra -Wpedantic main.c stats.c -o stats.exe
```

Sanitizer 使用條件及完整命令見 [第 11 章](chapters/11-debug.md)。C23 範例另見 [第 14 章](chapters/14-expert.md)。

## 型別與輸出格式

| 資料 | 常見宣告 | printf 格式 | 注意 |
| --- | --- | --- | --- |
| 一般整數 | int n | %d | 不假定固定 32-bit |
| 無號整數 | unsigned int n | %u | 回繞有規則，仍可能是邏輯錯誤 |
| 大小 | size_t n | %zu | signed／unsigned 混用要檢查 |
| 寬範圍非負整數 | unsigned long long n | %llu | 仍有最大值 |
| 浮點 | double x | %.2f | 近似值，顯示精度不是運算精度 |
| 字串 | const char *s | %s | 必須有效且零結尾 |
| 物件指標 | int *p | %p，傳 (void *)p | 只用於診斷，不推論可攜格式 |

## 常見工作對照

| 我想做什麼 | 起點 | 核心檢查 |
| --- | --- | --- |
| 算平均 | [2](chapters/02-types.md)、[6](chapters/06-pointers.md) | 分母非零，先轉浮點再除 |
| 遍歷陣列 | [5](chapters/05-arrays.md) | i < len，避免讀未初始化格 |
| 改呼叫端變數 | [6](chapters/06-pointers.md) | 傳位址，目標仍活著且可寫 |
| 把姓名與成績綁一起 | [7](chapters/07-records.md) | struct、字串容量、複製語意 |
| 接收未知筆數 | [8](chapters/08-memory.md) | len/cap 不變量，先驗證乘法 |
| 分檔 | [9](chapters/09-modules.md) | 宣告放標頭、定義放實作、一起連結 |
| 讀整數 | [10](chapters/10-io.md) | strtol、end、ERANGE、領域範圍 |
| 排序 | [12](chapters/12-algorithms.md) | 比較器一致，不直接用可能溢位的差 |
| 讀完整檔案並分析 | [13](chapters/13-project.md) | 完整行、錯誤分支、清理 |
| 判斷最佳化是否改壞程式 | [14](chapters/14-expert.md) | 先查 UB 與契約，再比較實作 |

## 問題與原因

- **平均總是少了小數？** 整數除法先發生；改 `total / 2.0`，不能只改接收變數型別。
- **陣列傳入函式後大小變小？** 參數調整成指標；另外傳元素數，不用 sizeof 猜。
- **兩個相同文字用 == 卻不相等？** 指標相等不是字串內容相等；用 strcmp 並確認字串有效。
- **realloc 後另一個指標壞了？** 舊借用指標可能已失效；保存索引並從新配置重新取址。
- **讀檔最後多一筆？** 常因 while(!feof(...))；改以實際讀取結果控制迴圈。
- **程式不當機，所以越界沒關係？** 錯誤可能尚未顯現；非法存取沒有語言保證。
- **free 後設 NULL 是否足夠？** 只更新該變數；其他別名仍須停止使用。
- **用了 const 就能多執行緒讀寫？** const 不是同步工具；見記憶體模型與資料競爭。

## 標準標頭速查

| 標頭 | 本書使用 |
| --- | --- |
| stdio.h | FILE、printf、fgets、fgetc、fopen、fclose |
| stdlib.h | malloc、realloc、free、strtol、qsort |
| stddef.h | size_t、NULL |
| string.h | strlen、strcmp、strcspn、memcpy |
| limits.h | INT_MIN、INT_MAX、CHAR_BIT |
| stdint.h | SIZE_MAX 與固定／至少寬度整數型別 |
| errno.h | errno、ERANGE |
| ctype.h | isspace，參數域須合法 |
| assert.h | assert，可能被 NDEBUG 移除 |
| stdatomic.h | C11 起的原子操作，先核對實作支援 |
| stdckdint.h | C23 檢查整數運算，正文 C17 不使用 |

語法與 API 細節可由 [來源表](references.md) 及各章外部連結核對。這裡的任務與 FAQ 均連回正文，不另列未實際閱讀的推薦書目。
