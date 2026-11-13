"""Generate and validate the local fictional tablecard demo (standard library only)."""
from pathlib import Path
import html
import json
import math
import subprocess
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "examples" / "tablecard-demo.json"
data = json.loads(SOURCE.read_text(encoding="utf-8"))
measurements = json.loads(subprocess.check_output(
    ["powershell.exe", "-NoProfile", "-Command",
     "[Console]::OutputEncoding = [System.Text.UTF8Encoding]::new(); "
     + "$PSScriptRoot = '" + str(ROOT / "examples").replace("'", "''") + "'; "
     + (ROOT / "examples" / "measure_tablecard.ps1").read_text(encoding="utf-8")],
    encoding="utf-8-sig"))
base = ROOT / "docs" / "drawio" / f'{data["ip"]}【{data["alias"]}】' / data["database"]
previews = []
links = []

def esc(value):
    return html.escape(str(value), quote=True)

for obj in data["objects"]:
    for kind in ("Tables", "Views"):
        (base / obj["schema"] / kind).mkdir(parents=True, exist_ok=True)
    title = f'{data["database"]}.{obj["schema"]}.{obj["name"]}【{obj["label"]}】'
    measured = measurements[f'{obj["schema"]}.{obj["name"]}']
    key_width = measured['key'] + 4
    name_width = max(measured['names'].values()) + 8
    # Keep readable 11px text; title and identifier widths are measured with Windows fonts.
    width = max(measured['title'] + 12, key_width + name_width + 70)
    chinese_width = width - key_width - name_width - 6
    row_height = 0
    rows = []
    for col in obj["columns"]:
        label = col.get("label") or "待確認（原始資料無中文註解）"
        # Conservative CJK glyph bound, including bold text and cell padding.
        row_height += max(1, math.ceil(len(label) * 11 / (chinese_width - 4))) * 14 + 2
        tooltip = f'{col["type"]} · NULL {"YES" if col["nullable"] else "NO"} | 虛構資料字典：examples/tablecard-demo.json | 中文名稱：{"既有註解" if col.get("label") else "待確認"}'
        rows.append(
            f'<tr><td style="vertical-align:top;white-space:nowrap"><b>{esc(col.get("key", ""))}</b></td>'
            f'<td style="vertical-align:top;white-space:nowrap;padding-right:8px" title="{esc(tooltip)}">{esc(col["name"])}</td>'
            f'<td style="vertical-align:top;white-space:normal;overflow-wrap:anywhere;word-break:break-all" title="{esc(tooltip)}"><b>{esc(label)}</b></td></tr>'
        )
    card = (
        '<div style="box-sizing:border-box;width:100%;background:rgb(228,228,228);padding:2px 3px;font-size:10.5px;line-height:13px;white-space:nowrap">'
        f'<b>{esc(title)}</b></div>'
        f'<table style="width:100%;table-layout:fixed;font-family:Arial;font-size:11px;line-height:14px" cellpadding="1" cellspacing="0"><colgroup><col style="width:{key_width}px"/><col style="width:{name_width}px"/><col/></colgroup><tbody>'
        + "".join(rows) + '</tbody></table>'
    )
    mxfile = ET.Element("mxfile", host="Electron")
    diagram = ET.SubElement(mxfile, "diagram", name="第 1 頁", id=f'{obj["schema"]}-{obj["name"]}')
    model = ET.SubElement(diagram, "mxGraphModel", grid="1", gridSize="10", tooltips="1", page="1", pageScale="1", pageWidth="827", pageHeight="1169")
    root = ET.SubElement(model, "root")
    ET.SubElement(root, "mxCell", id="0")
    ET.SubElement(root, "mxCell", id="1", parent="0")
    cell = ET.SubElement(root, "mxCell", id="entity", parent="1", vertex="1", value=card,
        style="verticalAlign=top;align=left;overflow=fill;html=1;whiteSpace=wrap;fontFamily=Arial;fontSize=11;spacing=0;")
    height = 17 + row_height + 4
    ET.SubElement(cell, "mxGeometry", x="0", y="0", width=str(width), height=str(height), **{"as": "geometry"})
    path = base / obj["schema"] / obj["kind"] / f"{title}.drawio"
    ET.indent(mxfile)
    ET.ElementTree(mxfile).write(path, encoding="utf-8", xml_declaration=True)
    checked = ET.parse(path)
    vertices = checked.findall('.//mxCell[@vertex="1"]')
    assert len(vertices) == 1 and not checked.findall('.//mxCell[@edge="1"]')
    geometry = vertices[0].find("mxGeometry")
    assert geometry.get("x") == "0" and geometry.get("y") == "0"
    label_tree = ET.fromstring("<section>" + vertices[0].get("value") + "</section>")
    assert len(label_tree.findall(".//tr")) == len(obj["columns"])
    for row, column in zip(label_tree.findall(".//tr"), obj["columns"]):
        assert row[0].find("b").text == (column.get("key") or None)
        assert row[1].text == column["name"]
        assert 'white-space:nowrap' in row[0].get('style')
        assert 'white-space:nowrap' in row[1].get('style')
    assert 'white-space:nowrap' in label_tree.find('div').get('style')
    previews.append(f'<article><p>{esc(obj["kind"])} · {width} × {height} px</p><div style="width:{width}px;height:{height}px;border:1px solid #aaa;font-size:11px">{card}</div></article>')
    links.append(f'- [{title}]({path.relative_to(ROOT / "docs").as_posix()})')

preview = ROOT / "docs" / "tablecard-preview.html"
preview.write_text('<!doctype html><html lang="zh-Hant"><meta charset="utf-8"><title>資料表卡片範例</title>'
    '<style>body{font-family:Arial,"Microsoft JhengHei",sans-serif;background:#f5f6f8;padding:24px}main{display:flex;flex-wrap:wrap;align-items:flex-start;gap:24px}article{background:white;padding:12px}article p{font-size:12px;color:#555}</style>'
    '<h1>資料表卡片範例</h1><p>虛構教學資料。下列為相同 HTML 標籤的預覽，drawio 實際呈現仍以編輯器為準。</p>'
    '<p>PK＝主鍵（同表多欄 PK 為複合主鍵）；UK1＝唯一鍵群組 1。View 未宣告表鍵，不從欄位名稱推測。</p><main>'
    + ''.join(previews) + '</main></html>', encoding="utf-8")
(ROOT / "docs" / "tablecard-demo.md").write_text(
    '# drawio 表格結構卡範例\n\n'
    '本範例完全虛構，不連接資料庫。來源為 `examples/tablecard-demo.json`。\n\n'
    '共 3 張 Table、2 張 View，依資料庫與 Schema 分層；每份 drawio 只有一個頂點，座標為 (0, 0)，不含關聯線。\n\n'
    '新版維持 11px 欄位字級，使用 Windows 字型量測表頭及英文欄位名，依內容計算緊湊尺寸；表頭、欄位名與表鍵設為不換行，中文欄位名稱可換行。卡片高度含中文換行估算與少量餘裕，未宣稱為所有渲染器下的絕對最小值。\n\n'
    'PK 表示主鍵；每日出勤的兩個 PK 欄位共同構成複合主鍵。UK1 表示唯一鍵群組 1。View 未宣告表鍵，所以保持空白。\n\n'
    '中文名稱取自來源資料字典；legacy_tag 沒有中文註解，明確標示待確認。欄位型別、可空性及來源放在 HTML title 提示中。\n\n'
    '## 開啟方式\n\n在 diagrams.net／draw.io 使用「開啟現有圖表」選擇下列檔案；每張卡片可作為單一物件移動，編輯其 HTML 標籤。\n\n'
    + '\n'.join(links) + '\n\n'
    '可用瀏覽器開啟 [HTML 預覽](tablecard-preview.html) 一次檢視所有卡片；此預覽不等同 drawio 編輯器渲染驗證。\n\n'
    '## 重建\n\n在 Windows 工作區執行 `python examples/build_tablecard_demo.py`，使用 Python 標準函式庫及 PowerShell 的 System.Drawing 字型量測，會覆寫這份範例的輸出。\n\n'
    '產生器適用於這份資料字典格式，並非通用 SQL 解析器。實際資料庫工作需另提供 DDL、資料字典或已授權的連線資訊。\n', encoding="utf-8")
print(f"Validated {len(data['objects'])} drawio files: XML, one vertex, origin, no edges, column order and keys.")
print(preview)
