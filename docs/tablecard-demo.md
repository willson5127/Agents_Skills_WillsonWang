# drawio 表格結構卡範例

本範例完全虛構，不連接資料庫。來源為 `examples/tablecard-demo.json`。

共 3 張 Table、2 張 View，依資料庫與 Schema 分層；每份 drawio 只有一個頂點，座標為 (0, 0)，不含關聯線。

新版維持 11px 欄位字級，使用 Windows 字型量測表頭及英文欄位名，依內容計算緊湊尺寸；表頭、欄位名與表鍵設為不換行，中文欄位名稱可換行。卡片高度含中文換行估算與少量餘裕，未宣稱為所有渲染器下的絕對最小值。

PK 表示主鍵；每日出勤的兩個 PK 欄位共同構成複合主鍵。UK1 表示唯一鍵群組 1。View 未宣告表鍵，所以保持空白。

中文名稱取自來源資料字典；legacy_tag 沒有中文註解，明確標示待確認。欄位型別、可空性及來源放在 HTML title 提示中。

## 開啟方式

在 diagrams.net／draw.io 使用「開啟現有圖表」選擇下列檔案；每張卡片可作為單一物件移動，編輯其 HTML 標籤。

- [hr_demo.hr.employees【員工資料】](drawio/192.0.2.10【人事教學範例】/hr_demo/hr/Tables/hr_demo.hr.employees【員工資料】.drawio)
- [hr_demo.hr.departments【部門資料】](drawio/192.0.2.10【人事教學範例】/hr_demo/hr/Tables/hr_demo.hr.departments【部門資料】.drawio)
- [hr_demo.attendance.daily_attendance【每日出勤】](drawio/192.0.2.10【人事教學範例】/hr_demo/attendance/Tables/hr_demo.attendance.daily_attendance【每日出勤】.drawio)
- [hr_demo.hr.v_employee_directory【員工通訊名錄】](drawio/192.0.2.10【人事教學範例】/hr_demo/hr/Views/hr_demo.hr.v_employee_directory【員工通訊名錄】.drawio)
- [hr_demo.attendance.v_daily_summary【每日出勤摘要】](drawio/192.0.2.10【人事教學範例】/hr_demo/attendance/Views/hr_demo.attendance.v_daily_summary【每日出勤摘要】.drawio)

可用瀏覽器開啟 [HTML 預覽](tablecard-preview.html) 一次檢視所有卡片；此預覽不等同 drawio 編輯器渲染驗證。

## 重建

在 Windows 工作區執行 `python examples/build_tablecard_demo.py`，使用 Python 標準函式庫及 PowerShell 的 System.Drawing 字型量測，會覆寫這份範例的輸出。

產生器適用於這份資料字典格式，並非通用 SQL 解析器。實際資料庫工作需另提供 DDL、資料字典或已授權的連線資訊。
