# Noah2026

Windows 向けアーカイバフロントエンド **Noah** のソースです。

## 履歴

| 区分 | 内容 |
|------|------|
| **初期コミット** | デスクトップの `nsrc3199`（元は LZH にまとめられていたソース一式）をそのまま Git 化したものです。 |
| **以降（本リポジトリの main）** | Visual Studio 現行ツールチェーンでのビルド、日本語リソース（UTF-8）、一部セキュリティ対策などの改造版です。 |

元の説明・構成はリポジトリ内の [ReadMe.txt](ReadMe.txt) を参照してください（VC++ 6.0 / 2010 向けの記述が中心です）。

## 利用条件

ソースの扱いについては [ReadMe.txt](ReadMe.txt) の記載どおりで、**バイナリ側に付いている利用条件に従います**（基本的には自由に利用可能、という説明）。

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
