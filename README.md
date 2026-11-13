# Codex Skill 入門模板

這個工作區包含一個可修改的實際技能：`requirement-brief`，用來把零散筆記整理成開發需求規格。

## 檔案位置

```text
Delta_Codex/
├── .agents/skills/requirement-brief/
│   └── SKILL.md           技能本體，複製這個資料夾即可改作其他技能
├── examples/
│   └── requirement-notes.md   練習輸入與測試方式
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
