# 術語表

| 術語 | 原文 | 白話解釋 | 回讀 |
| --- | --- | --- | --- |
| 原始碼 | source code | 供編譯器讀取的程式文字 | [1](chapters/01-start.md) |
| 編譯器 | compiler | 分析程式並產生目標形式的工具 | [1](chapters/01-start.md) |
| 連結器 | linker | 組合物件檔與必要定義 | [9](chapters/09-modules.md) |
| 翻譯單元 | translation unit | 前處理後可分別編譯的一份內容 | [9](chapters/09-modules.md) |
| 物件 | object | 有型別、值與生命期的資料儲存區，不是物件導向類別的同義詞 | [2](chapters/02-types.md) |
| 初始化 | initialization | 建立物件時給定初值 | [2](chapters/02-types.md) |
| 賦值 | assignment | 把值存入已存在的可修改目標 | [2](chapters/02-types.md) |
| 型別轉換 | conversion | 把某型別的值轉成另一型別 | [2](chapters/02-types.md) |
| 溢位 | overflow | 數學結果超出目的型別可表示範圍 | [11](chapters/11-debug.md) |
| 短路 | short-circuit evaluation | 邏輯結果已確定時，不計算另一側 | [3](chapters/03-control.md) |
| 不變量 | invariant | 在指定程序節點持續成立的條件 | [3](chapters/03-control.md) |
| 參數／引數 | parameter / argument | 函式宣告的接收位置／呼叫時提供的運算式 | [4](chapters/04-functions.md) |
| 契約 | contract | 函式前提、保證、錯誤與資源責任的約定 | [4](chapters/04-functions.md) |
| 遞迴 | recursion | 把問題縮小後再次呼叫相同工作 | [4](chapters/04-functions.md) |
| 陣列 | array | 連續排列的同型別元素 | [5](chapters/05-arrays.md) |
| 緩衝區 | buffer | 暫存資料的儲存空間，要另知其容量 | [5](chapters/05-arrays.md) |
| 零結尾字串 | null-terminated string | 以第一個零字元界定結尾的字元序列 | [5](chapters/05-arrays.md) |
| 指標 | pointer | 能指向物件或函式、也可為空等狀態的型別與值 | [6](chapters/06-pointers.md) |
| 解參照 | dereference | 透過指標指定其目標 | [6](chapters/06-pointers.md) |
| 作用域／生命期 | scope / lifetime | 名稱可見的範圍／物件存在的期間 | [6](chapters/06-pointers.md) |
| 懸空指標 | dangling pointer | 原目標已結束生命期的指標關係，不能繼續使用舊值 | [8](chapters/08-memory.md) |
| 結構／聯集 | struct / union | 多成員組合／多成員共用儲存區 | [7](chapters/07-records.md) |
| 填補／對齊 | padding / alignment | 為布局加入的空間／型別存放位置的要求 | [14](chapters/14-expert.md) |
| 所有權／借用 | ownership / borrowing | 誰負責清理／只在允許期間使用他人資源的約定 | [8](chapters/08-memory.md) |
| 長度／容量 | length / capacity | 有效元素數／已配置可容納元素數 | [8](chapters/08-memory.md) |
| 回呼 | callback | 傳入函式指標，由另一段流程呼叫 | [12](chapters/12-algorithms.md) |
| 攤銷分析 | amortized analysis | 分析一串操作的總成本，再分攤每次操作 | [12](chapters/12-algorithms.md) |
| 有效型別 | effective type | 用來限制物件值可透過哪些型別存取的規則 | [14](chapters/14-expert.md) |
| 未定義行為 | undefined behavior, UB | 標準對該情境不再提出要求 | [14](chapters/14-expert.md) |
| 原子操作 | atomic operation | 依記憶體模型提供不可分割存取等保證的操作 | [14](chapters/14-expert.md) |
| 先行發生 | happens-before | 推理跨執行緒可見性與資料競爭的順序關係 | [14](chapters/14-expert.md) |

這是本書用語索引與教學簡化，正式定義及已讀來源請回到各章與 [來源總表](references.md)。
