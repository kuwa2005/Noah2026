# Noah2026

Windows 向けアーカイバフロントエンド **Noah** のソースです。

## 履歴

| 区分 | 内容 |
|------|------|
| **初期コミット** | デスクトップの `nsrc3199`（元は LZH にまとめられていたソース一式）をそのまま Git 化したものです。 |
| **以降（本リポジトリの main）** | Visual Studio 現行ツールチェーンでのビルド、日本語リソース（UTF-8）、一部セキュリティ対策などの改造版です。 |

元の説明・構成はリポジトリ内の [ReadMe.txt](ReadMe.txt) を参照してください（VC++ 6.0 / 2010 向けの記述が中心です）。

## ライセンス

| 対象 | 内容 |
|------|------|
| **オリジナル Noah**（k.inaba 氏） | **NYSL Version 0.9982**。リポジトリルートの [LICENSE](LICENSE) に全文を記載しています。 |
| **公式の解説・FAQ** | [NYSL（kmonos.net）](https://www.kmonos.net/nysl/) ※FAQ 本文は条項そのものではありません（公式サイトの説明どおり）。 |
| **本リポジトリ（Noah2026）** | 上記オリジナルをベースとした**改造版**です（ビルド更新、UTF-8 リソース、セキュリティ対策など）。利用・再配布・改変の自由度は NYSL の趣旨に沿う想定です。**免責は NYSL B 項どおり各自の責任**でお願いします。 |
| **従来の README 文言** | [ReadMe.txt](ReadMe.txt) 冒頭の「バイナリ側の利用条件に従う」という説明とも整合する位置づけです。 |

GPL のようなコピーレフトはなく、派生物を別ライセンスで公開しても NYSL 上は問題ありません（詳細は公式 FAQ）。

### License (English summary)

Original **Noah** by **k.inaba** is under **NYSL 0.9982** (full text in [LICENSE](LICENSE): Everyone'sWare, free redistribution/modification, no warranty, moral rights remain with k.inaba, copyright disclaimed). This repo is a **modified fork** (toolchain, resources, hardening); use at your own risk per section B. See [NYSL official page](https://www.kmonos.net/nysl/) for the FAQ (not part of the legal terms).

## ビルド（改造版）

- **推奨**: Visual Studio 2022 以降（Windows SDK 10、`PlatformToolset` v145 想定）
- ソリューション: `Noah.sln`
- メインプロジェクト: `Noah.vcxproj`（Win32）

リソース内の日本語を再生成する場合は `tools/EmitNoahRcJapanese.ps1` を参照してください（手編集で `Noah.rc` の UTF-8 が壊れやすいため）。

## リポジトリ

- GitHub: <https://github.com/kuwa2005/Noah2026>

## 関連パス（ローカル作業用メモ）

- 上流相当の展開元: `C:\Users\pcm_n\OneDrive\デスクトップ\nsrc3199`
- 改造作業ディレクトリ: `D:\00_project\Noah2026`（本リポジトリのクローン）
