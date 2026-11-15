# 來源、閱讀範圍與限制

查閱日期統一為 **2026-11-15（Asia/Taipei）**。沒有把搜尋摘要或 AI 摘要當作技術證據，也沒有宣稱閱讀標準草案全文。

## 搜尋方式

實際在背景瀏覽器開啟 Google，閱讀搜尋結果；查詢為 `C language beginner official documentation` 與 `C語言 入門 標準 指標 未定義行為`。網頁工具無法直接讀 Google 搜尋頁，改由瀏覽器確認成功。後續以一般網頁搜尋工具補查標準、GCC、MSYS2、Aspnes 教材、Clang 與 CERT；此工具未揭露搜尋引擎，**不稱為 Google 搜尋**。

補查包含 `site.open-std.org jtc1 sc22 wg14 n1570 n3096 C standard`、`site.gcc.gnu.org onlinedocs C Dialect Options`、`site.cs.yale.edu aspnes C programming notes`、`site.msys2.org docs environments ucrt64`、Clang sanitizers、Microsoft C 語言文件與 CERT 記憶體規則。Google 用於找入門、官方文件與常見誤解線索；下表是實際開啟且閱讀相關內容的原始資料。

## 已讀及核對來源

日期未能由已讀內容確定時記「未提供」，不以搜尋爬取日期當更新日期。Microsoft 頁面雖顯示通用授權提示，本文仍可讀；僅使用實際可見正文。每個來源只聲明表列閱讀範圍。

| 編號 | 原始來源 | 作者／機構 | 日期／版本 | 類型與已讀範圍 | 用途／章節 | 限制 |
| --- | --- | --- | --- | --- | --- | --- |
| S01 | [Getting Started](https://www.msys2.org/#installation) | MSYS2 | 頁面日期未提供；查閱時安裝器標示 20260927 | 官方文件，Installation 步驟與 GCC 套件命令 | 1 | 未實際安裝；本書命令為 x64 |
| S02 | [Environments](https://www.msys2.org/docs/environments/) | MSYS2 | 頁面版本未提供；可見 2026-03-15 變更紀錄 | 官方文件，Overview、UCRT64 與工具鏈差異 | 1 | 不泛用至所有 Windows 架構 |
| S03 | [C Dialect Options](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html) | GCC／GNU | 線上版本，未提供頁面更新日 | 官方文件，-std、c11/c17/c23、GNU 方言、hosted | 前言、1、14 | 預設值隨工具版本變動，本書固定 -std=c17 |
| S04 | [Type Conversions (C)](https://learn.microsoft.com/en-us/cpp/c-language/type-conversions-c?view=msvc-170) | Microsoft | 2021-08-03；msvc-170 | 官方文件，本文的轉換時機與整數提升 | 2 | 實作文件，不當成全部標準語意 |
| S05 | [if Statement (C)](https://learn.microsoft.com/en-us/cpp/c-language/if-statement-c?view=msvc-170) | Microsoft | 2023-01-25；msvc-170 | 官方文件，分支與巢狀 else 範例 | 3 | 不照抄其過度簡化的條件型別措辭 |
| S06 | [for Statement (C)](https://learn.microsoft.com/en-us/cpp/c-language/for-statement-c?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，迴圈順序、break、continue | 3 | 只取執行規則，不照抄原範例 |
| S07 | [C/Functions](https://www.cs.yale.edu/homes/aspnes/pinewiki/C%282f%29Functions.html) | James Aspnes／Yale | 靜態筆記 2003–2012；頁面個別日期未提供 | 原作者大學教材，函式定義、傳值、原型與呼叫 | 4、9 | 歷史教材；舊式括號寫法只作版本對照 |
| S08 | [Array Declarations](https://learn.microsoft.com/en-us/cpp/c-language/array-declarations?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，陣列宣告、多維陣列、列儲存 | 5 | 「array considered a pointer」措辭以 N1570 轉換規則修正解讀 |
| S09 | [Pointer Declarations](https://learn.microsoft.com/en-us/cpp/c-language/pointer-declarations?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，指標宣告、void 與不完整型別段落 | 6 | 不採其概括 const 說法替代逐層型別分析 |
| S10 | [Structure Declarations](https://learn.microsoft.com/en-us/cpp/c-language/structure-declarations?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，成員、tag、不完整結構與定義 | 7 | Microsoft 特定布局不視為可攜保證 |
| S11 | [C Bitwise Operators](https://learn.microsoft.com/en-us/cpp/c-language/c-bitwise-operators?view=msvc-170) | Microsoft | 2022-04-07；msvc-170 | 官方文件，AND／OR／XOR 與例子 | 7 | 不採 signed 的平台特定結論 |
| S12 | [fgets, fgetws](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fgets-fgetws?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，fgets 簽章、回傳、容量減一、保留換行與零結尾 | 5、10 | fgetws 與 Unicode 擴充未作本書規範 |
| S13 | [realloc](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/realloc?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，非零失敗、搬移、內容保留及相容性注意 | 8、13 | 零大小行為與 C17／C23 不宜混用；本書避開 |
| S14 | [MEM35-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem35-c/) | Carnegie Mellon SEI CERT | 日期未提供；線上 C coding standard | 原機構規則，容量不足與 sizeof／SIZE_MAX 範例 | 8 | 規則指南不取代 ISO 標準 |
| S15 | [MEM30-C](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/rules/memory-management-mem/mem30-c/) | Carnegie Mellon SEI CERT | 日期未提供；線上 C coding standard | 原機構規則，釋放後指標不確定值與重複釋放段落 | 8、12 | 未系統審閱全套 CERT 規則 |
| S16 | [#include](https://learn.microsoft.com/en-us/cpp/preprocessor/hash-include-directive-c-cpp?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，文字引入、標頭用途與搜尋區別 | 9 | 路徑順序屬實作，未套用到所有工具鏈 |
| S17 | [strtol](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/strtol-wcstol-strtol-l-wcstol-l?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，簽章、end_ptr、停止位置、ERANGE | 10、13 | 不使用 _l 擴充及無效參數處理器作 ISO 保證 |
| S18 | [fopen](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-wfopen?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，回傳、讀寫模式與文字／二進位差異 | 10、13 | Windows 路徑與 Unicode 模式屬平台議題 |
| S19 | [fread](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fread?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，元素寬度與數目、短讀、部分元素 | 10 | 不把 MS 參數驗證視為標準保證 |
| S20 | [fclose](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/fclose-fcloseall?view=msvc-170) | Microsoft | 2022-12-02；msvc-170 | 官方文件，關閉、緩衝與回傳值 | 10、13 | _fcloseall 不用於本書程式 |
| S21 | [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html) | LLVM／Clang | 日期未提供；線上文件 | 官方文件，Introduction、Usage、Limitations、Supported Platforms | 11 | 沒有執行；Windows 支援與 runtime 需另外核對 |
| S22 | [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html) | LLVM／Clang | 日期未提供；線上文件 | 官方文件，Introduction、Usage、Available checks | 11 | 非所有 UB 都可檢出，未實測 |
| S23 | [qsort](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/qsort?view=msvc-170) | Microsoft | 日期未提供；msvc-170 | 官方文件，參數、比較器、相等順序 | 12、13 | 不從 Microsoft 的實作描述推論 ISO 演算法保證 |
| S24 | [Notes on Data Structures and Programming Techniques](https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html) | James Aspnes／Yale | Spring 2022；頁首時間戳 2025-04-27 | 原作者教材，目錄、§4.9.8 函式指標／回呼、realloc 範例及輸入相關段落 | 8、12；11 僅導讀 | 未讀全書；原 realloc 直接指派及差值比較器不作可靠寫法示範 |
| S25 | [WG14 N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) | ISO/IEC JTC1/SC22/WG14 | 2011-04-12；C11 committee draft | 標準草案，選讀 §3.4、§6.2.4、§6.3.2.1、§6.5 p2/p5/p6/p7、§6.5.3.4、§6.5.5–6、§6.7.3.1、§6.7.6.3、§7.1.1、§7.22.1.4、§5.1.2.4、§7.17.3 | 2、4–6、8、14 | 非正式 C17 全文；未讀全部 701 頁，未逐項核對後續缺陷修正 |
| S26 | [WG14 N3096](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3096.pdf) | ISO/IEC JTC1/SC22/WG14 | 2023-04-01；C23 working draft | 標準草案，封面、nullptr 轉換與型別、§7.20 checked arithmetic | 14 | 非最終出版標準，未讀全部 761 頁 |
| S27 | [WG14 官方頁](https://open-std.org/jtc1/sc22/wg14/) | WG14 | 頁面標示 2025-11-10；C23 狀態 | 標準機構，已讀標準採用狀態與草案／專案入口 | 前言、14 | 未取得付費正式全文 |
| S28 | [Are C arrays pointers?](https://www.reddit.com/r/C_Programming/comments/1d8z6o2/are_c_arrays_pointers/) | 提問者 TimFrod；r/C_Programming 參與者 | 精確發文日期未由已讀正文取得；顯示約 2 年前 | 論壇，已讀問題與前段陣列／轉換討論 | 14；5–6 的疑難線索 | 未標明 C 版本；留言措辭與成效經驗未視為已證實技術結論 |

所有條目查閱日皆為上列統一日期。程式、成績、練習、契約、推導與整合專題為本書原創教學內容；外部文件用來核對語意與工具行為，不代表它們審核或背書本書。

## 未採用與來源缺口

GNU C manual 的幾個頁面在此工具讀取逾時，僅取得搜尋線索，未列為已讀引用。Microsoft 的 Functions Definitions／Function Declarators 嘗試未成功，改用 Aspnes 原作者筆記與 N1570。沒有把這些失敗網址當成章末已讀來源。

沒有取得 ISO C17 正式全文；以公開 C11 草案核對穩定語意，並由 GCC 文件確認 C17 的修正關係。嚴格標準爭議、缺陷報告演進、完整 C23 差異、指標 provenance 文獻、無鎖記憶體回收不在本次已查證範圍。

## 編輯審閱範圍

已回讀第 1–14 章完成的解說、範例與解答，重點為先備銜接、傳值與指標、陣列大小、字串邊界、realloc 失敗保留、整數檢查、空資料、比較器及發布協定。補充了條件運算子、runtime、scalar 與 provenance 的首次解釋。人工推演了各章顯示結果、三筆與四筆專題統計；這些不是執行結果。

另以 Node.js 檢查書內相對連結目標存在、Markdown 程式圍欄成對、14 章各有練習／解答與章末網路來源，並核對專題內嵌程式與獨立 .c 檔一致。來源核對僅限表列已讀段落，不聲稱外部文件全數全文審閱。範例尚無 C 編譯／執行結果，不以 Markdown 格式或連結通過代替程式正確性；安裝、Sanitizer、GDB、跨平台與故障注入均未實測，未進行真人學習成效驗證。
