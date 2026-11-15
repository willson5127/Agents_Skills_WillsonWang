# Codex Skill 入門模板

這個工作區保存可修改的 Codex 技能，包含整理開發需求的 `requirement-brief`、產生資料表卡片的 `drawio-tablecard`，以及搜尋並編寫教材的 `research-textbook`。

## 搜尋並編寫系統教材

新增的 [research-textbook](.agents/skills/research-textbook/SKILL.md) 技能可接收學習主題，使用 Google 查找並核對官方文件、知識文獻與論壇，在指定資料夾建立從零入門、逐章深入到專家議題的教科書或工具書。每章包含解說、示範、練習與解答，以及可追溯的來源超連結；另提供學習地圖、術語表，以及涵蓋範圍與限制。學習成果檢核是書中的練習與判準，並非技能測試報告。

```text
請使用 $research-textbook。我想從零學 PostgreSQL，請用 Google 搜尋資料，
在 Document/Books/postgresql/ 建立繁體中文教科書，從基礎講到進階原理與專家實務，
每章附範例、練習解答及原始來源超連結。
```

未指定位置時依工作區規範預設放在 `Document/Books/<主題短名>/`，不修改個人技能設定。此技能需要可存取 Google 的瀏覽或搜尋能力；若無法使用 Google，會說明限制並詢問是否接受替代引擎，不冒稱已完成搜尋。教材預設為 Markdown；書稿涵蓋專家議題，不代表只讀完即可取得專家能力。

更多教科書與工具書的編寫提示見 [examples/research-textbook-request.md](examples/research-textbook-request.md)。

技能的核心品質要求：不虛構書名、作者、連結或引用；沒有讀到內容就不宣稱教材品質已驗證；清楚列出涵蓋內容與缺口，不宣稱無所不包；每章提供由淺入深的解釋、範例、練習、解答與學習成果檢核方式，並附實際開啟閱讀的網路來源連結。僅有目錄、來源清單或格式檢查不足以交付。
## 檔案位置

```text
Agents_Skills_WillsonWang/
├── .agents/skills/
│   ├── requirement-brief/SKILL.md
│   ├── drawio-tablecard/SKILL.md
│   └── research-textbook/
│       ├── SKILL.md
│       ├── agents/openai.yaml
│       └── references/book-structure.md
├── examples/
│   ├── requirement-notes.md
│   └── research-textbook-request.md   教科書與工具書的編寫提示
└── README.md              本教學，不是技能指令
```

技能最少只需要一個 `SKILL.md`，不需要安裝套件、撰寫程式或提供 API 金鑰。

## Codex 怎麼調用？

1. **發現技能**：專案技能放在 `.agents/skills/技能名稱/SKILL.md`。先讓 Codex 開啟這個工作區。
2. **選擇技能**：Codex 先取得 `name`、`description` 等資訊，再依任務判斷是否使用。自動選擇是語意判斷，不是固定關鍵字事件。
3. **讀取指令並執行**：選中後才讀完整 `SKILL.md`，按照內容處理任務。Skill 本身不會在背景執行；需要腳本時，仍由 Codex 使用工具執行。

可以直接貼上以下提示，明確指定技能：

```text
請使用 $requirement-brief，將 examples/requirement-notes.md 整理成需求規格，存到 output/requirement-brief.md。
```

也可以測試自然語言選擇：

```text
請將 examples/requirement-notes.md 的功能筆記整理成開發需求規格，列出驗收條件與待釐清問題。
```

前者適合確認技能效果，後者適合測試描述是否容易被選中。請觀察 Codex 是否讀取該 `SKILL.md`，以及輸出是否遵守其規則。若未被發現，先確認工作區與檔案位置，再重新啟動 Codex；也可暫時在提示中提供完整的 `SKILL.md` 路徑請它讀取。

## 怎麼寫才有用？

最重要的是寫出「什麼任務適用」及「比一般回答多了哪些具體規則」。

- `name`：使用小寫英文、數字與連字號，例如 `requirement-brief`；資料夾名稱保持一致。
- `description`：簡短交代工作、觸發情境及必要的界線。不要只寫「協助工作」或「你是專家」。
- 本文：寫清楚輸入、處理規則、輸出與完成標準。本例特別要求區分事實與假設，並產生可觀察的驗收條件。
- 將公司用語、特定格式、常見錯誤等真正影響結果的知識寫進去；避免塞入無關工作流程。
- 先保持簡單；只有需要重複且精確的運算時才加入腳本。

## 改成自己的技能

1. 複製 `requirement-brief` 資料夾，改為例如 `meeting-summary`。
2. 修改 `SKILL.md` 的 `name` 與 `description`，說明何時整理會議紀錄。
3. 修改本文，定義你需要的輸入、規則與輸出，例如決議、負責人、期限；未提供的資訊標示待確認。
4. 用明確指定與自然語言各試一次，再試一個不相關的任務，確認沒有誤用。

本範例保留預設的自動選擇行為，沒有額外的 UI 設定。需要擴充時，可以在技能資料夾內加入：

| 選用項目 | 用途 |
| --- | --- |
| `references/` | 較長的規範；在 `SKILL.md` 說明何時讀取哪個檔案 |
| `assets/` | 要套用到成品的範本或圖片 |
| `scripts/` | 可實際執行的重複處理程序 |
| `agents/openai.yaml` | 顯示資訊、調用政策或工具相依設定 |

`AGENTS.md` 適合專案中持續適用的共同規則；Skill 適合被特定任務選用的流程。此入門範例不需要新增 `AGENTS.md`。

## 驗證方式與限制

請使用 `examples/requirement-notes.md` 的檢查項目驗收結果。檔案格式通過檢查不代表自動調用一定成功，仍需實際測試提示與結果。

這次建立的是專案內技能；若要跨專案使用，官方現行使用者範圍路徑為 `~/.agents/skills/`。本範例不修改你的個人設定。

官方文件（查閱日期：2026-11-13）：[Build skills](https://learn.chatgpt.com/docs/build-skills)。文件說明技能結構、明確與自動調用、搜尋位置及更新偵測。
