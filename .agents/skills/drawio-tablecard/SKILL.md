---
name: drawio-tablecard
description: 解析資料庫或相關文件，將各表格撰寫並歸類成各單一drawio的entity元件。適用於單純資料庫分析，以及新建表；單純問答或直接修改程式碼時不使用，且只考慮表名、表鍵、欄位、中文欄位名，不考慮關聯、結構、特殊功能、排程。
---

# 分析撰寫文件

將資料表的任何表整理成一份份drawio文件，使用繁體中文，除非使用者指定其他語言。

## 輸入

依照指示閱讀指定資料庫或相關文件。若沒有參考指示，則依照工作環境文件找尋資料庫線索。

## 處理方式

1. 從指示文件或工作環境文件找出指定資料庫。
2. 請在./docs/，找尋或建立資料夾drawio。
3. 請在./docs/drawio/，找尋或建立資料夾名為，IP連線位置【資料庫俗稱】。
4. 請在./docs/drawio/IP連線位置【資料庫俗稱】/，建立多個資料夾，名為資料庫名稱。若資料庫種類存在模式分層，請在各./docs/drawio/IP連線位置【資料庫俗稱】/資料庫名稱/，之中建立多個資料庫Schema名稱的資料夾。
5. 請在各./docs/drawio/IP連線位置【資料庫俗稱】/資料庫名稱/，找尋或建立兩個資料夾名為，Tables，和Views。若資料庫種類存在模式分層，請進一步在資料庫Schema層建立。
6. 請在各./docs/drawio/IP連線位置【資料庫俗稱】/資料庫名稱/，建立多個drawio，名為資料表名稱，或View表名稱，名稱格式`db名稱.schema名稱.table/view名稱【中文名稱】`。若資料庫種類存在模式分層，請進一步在資料庫Schema層建立。
7. 解析出各Tables&Views的結構，並且將其以輸出格式建立drawio表格結構卡。若有舊版的文件，請直接覆蓋。

## 輸出

使用下列結構，可依需求大小縮短內容：

```xml
<?xml version='1.0' encoding='utf-8'?>
<mxfile host="Electron">
  <diagram name="第 1 頁" id="attendance-daily_attendance">
    <mxGraphModel grid="1" gridSize="10" tooltips="1" page="1" pageScale="1" pageWidth="827"
      pageHeight="1169">
      <root>
        <mxCell id="0" />
        <mxCell id="1" parent="0" />
        <mxCell id="entity" parent="1" vertex="1"
          value="&lt;div style=&quot;box-sizing:border-box;width:100%;background:rgb(228,228,228);padding:2px 3px;font-size:10.5px;line-height:13px;white-space:nowrap&quot;&gt;&lt;b&gt;hr_demo.attendance.daily_attendance【每日出勤】&lt;/b&gt;&lt;/div&gt;&lt;table style=&quot;width:100%;table-layout:fixed;font-family:Arial;font-size:11px;line-height:14px&quot; cellpadding=&quot;1&quot; cellspacing=&quot;0&quot;&gt;&lt;colgroup&gt;&lt;col style=&quot;width:31px&quot;/&gt;&lt;col style=&quot;width:76px&quot;/&gt;&lt;col/&gt;&lt;/colgroup&gt;&lt;tbody&gt;&lt;tr&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap&quot;&gt;&lt;b&gt;PK&lt;/b&gt;&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap;padding-right:8px&quot; title=&quot;int · NULL NO | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;employee_id&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:normal;overflow-wrap:anywhere;word-break:break-all&quot; title=&quot;int · NULL NO | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;&lt;b&gt;員工編號&lt;/b&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap&quot;&gt;&lt;b&gt;PK&lt;/b&gt;&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap;padding-right:8px&quot; title=&quot;date · NULL NO | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;work_date&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:normal;overflow-wrap:anywhere;word-break:break-all&quot; title=&quot;date · NULL NO | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;&lt;b&gt;出勤日期&lt;/b&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap&quot;&gt;&lt;b&gt;&lt;/b&gt;&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap;padding-right:8px&quot; title=&quot;datetime2 · NULL YES | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;clock_in&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:normal;overflow-wrap:anywhere;word-break:break-all&quot; title=&quot;datetime2 · NULL YES | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;&lt;b&gt;上班打卡時間&lt;/b&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap&quot;&gt;&lt;b&gt;&lt;/b&gt;&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap;padding-right:8px&quot; title=&quot;datetime2 · NULL YES | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;clock_out&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:normal;overflow-wrap:anywhere;word-break:break-all&quot; title=&quot;datetime2 · NULL YES | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;&lt;b&gt;下班打卡時間&lt;/b&gt;&lt;/td&gt;&lt;/tr&gt;&lt;tr&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap&quot;&gt;&lt;b&gt;&lt;/b&gt;&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:nowrap;padding-right:8px&quot; title=&quot;nvarchar(200) · NULL YES | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;remark&lt;/td&gt;&lt;td style=&quot;vertical-align:top;white-space:normal;overflow-wrap:anywhere;word-break:break-all&quot; title=&quot;nvarchar(200) · NULL YES | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：既有註解&quot;&gt;&lt;b&gt;備註&lt;/b&gt;&lt;/td&gt;&lt;/tr&gt;&lt;/tbody&gt;&lt;/table&gt;"
          style="verticalAlign=top;align=left;overflow=fill;html=1;whiteSpace=wrap;fontFamily=Arial;fontSize=11;spacing=0;">
          <mxGeometry x="0" y="0" width="285" height="101" as="geometry" />
        </mxCell>
      </root>
    </mxGraphModel>
  </diagram>
</mxfile>
```

* 以上範例中的`hr_demo.attendance.daily_attendance【每日出勤】`是遵循檔名格式`db名稱.schema名稱.table/view名稱【中文名稱】`。
* 表小卡的大小請能越小越好，但要保持表名稱不可換行、表欄位名稱不可換行、表鍵不可換行，中文名稱可以換行。
* 此單一物件請放置於drawio的原點。
* Tables的顏色請使用entity元件原色，灰黑白。
* Views的顏色請使用紅色系，不可太鮮豔，就粉紅、紅、淡紅、白。文字請明顯可用黑或白。