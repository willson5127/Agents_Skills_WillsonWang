# 範例程式

所有程式為本書原創，尚未在本環境編譯或執行。各章程式區塊是閱讀依據，這裡提供完整程式的獨立檔案。部分片段只存在正文，不能單獨編譯。

- [01-1.c](01-1.c)：對應[第 01 章](../chapters/01-start.md)。
- [02-1.c](02-1.c)：對應[第 02 章](../chapters/02-types.md)。
- [03-2.c](03-2.c)：對應[第 03 章](../chapters/03-control.md)。
- [04-1.c](04-1.c)：對應[第 04 章](../chapters/04-functions.md)。
- [04-2.c](04-2.c)：對應[第 04 章](../chapters/04-functions.md)。
- [05-1.c](05-1.c)：對應[第 05 章](../chapters/05-arrays.md)。
- [05-2.c](05-2.c)：對應[第 05 章](../chapters/05-arrays.md)。
- [06-1.c](06-1.c)：對應[第 06 章](../chapters/06-pointers.md)。
- [07-1.c](07-1.c)：對應[第 07 章](../chapters/07-records.md)。
- [08-1.c](08-1.c)：對應[第 08 章](../chapters/08-memory.md)。
- [10-2.c](10-2.c)：對應[第 10 章](../chapters/10-io.md)。
- [11-1.c](11-1.c)：對應[第 11 章](../chapters/11-debug.md)。
- [12-1.c](12-1.c)：對應[第 12 章](../chapters/12-algorithms.md)。
- [14-1.c](14-1.c)：對應[第 14 章](../chapters/14-expert.md)。
- [14-4.c](14-4.c)：對應[第 14 章](../chapters/14-expert.md)。
- [modules/main.c](modules/main.c)、[stats.h](modules/stats.h)、[stats.c](modules/stats.c)：第 9 章三檔範例，須一起建置。
- [gradebook.c](gradebook.c) 與 [scores.txt](scores.txt)：第 13 章整合專題。

除第 14 章的 C23 範例外，使用 `gcc -std=c17 -Wall -Wextra -Wpedantic 檔名.c -o app.exe`。`14-4.c` 屬 C23，請依第 14 章核對標頭支援並改用 `-std=c23`。模組在 modules 目錄用 `gcc -std=c17 -Wall -Wextra -Wpedantic main.c stats.c -o stats.exe`。

請先預測輸出，再執行與記錄差異；不要把本檔案的存在當作已通過編譯。
