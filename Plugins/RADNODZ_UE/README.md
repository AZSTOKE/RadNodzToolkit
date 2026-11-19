# RADNODZ

RADNODZ は、株式会社AZSTOKE の RIGDOCKS を Unreal Engine から操作するためのプラグインです。
Blueprint / C++ から AZ_* API を呼び出し、REAPER や Wwise などのオーディオ制作ツールをリモート操作できます。

> 本プラグインの利用には、RADNODZプラグイン及び実行するAPIに対応するRIGDOCKSプラグイン（BRONZE/SILVER/GOLD）のライセンスが別途必要です。

## 動作環境

- Unreal Engine 5
- 対応プラットフォーム: Windows (x64) / Android (arm64, x86_64) / macOS (x86_64, arm64)
- 接続先: RIGDOCKS、RADNODZプラグイン がインストールされた PC

## 導入方法

1. [Releases](../../releases) から `RADNODZ_UE_v<バージョン>.zip` をダウンロードします。
   (自動で添付される「Source code (zip)」にはコンパイル済みライブラリが含まれないため、使用できません。)
2. ZIP の中にある `RADNODZ_UE` フォルダを、プロジェクトの `Plugins` フォルダにコピーします(`Plugins/RADNODZ_UE` となるように配置します)。
   Windows の「すべて展開」を使うと、ZIP 名のフォルダ(`RADNODZ_UE_v<バージョン>`)の中に `RADNODZ_UE` ができます。その場合も、中の `RADNODZ_UE` フォルダだけを `Plugins` に移してください。
3. プロジェクトファイルを再生成してビルドします。
4. エディタの「プラグイン」で RADNODZ を有効にします。

ビルドが必要なため、Blueprint のみのプロジェクトでは利用できません。

> このリポジトリには、コンパイル済みライブラリ(`Source/Libs` 以下の `.lib` / `.a`)は含まれていません。
> リポジトリをクローンしただけではビルドできないため、必ず Releases の ZIP を使用してください。

使い方は [ドキュメント](https://www.azstoke.jp/rigdocks/radnodz-howtouse) を参照してください。

## 構成

| モジュール | 内容 |
|---|---|
| RadnodzClientPlugin | 接続・通信 |
| RadnodzClientEditerPlugin | エディタ拡張 |
| RigdocksBronzePlugin / RigdocksSilverPlugin / RigdocksGoldPlugin | 各ランクの AZ_* API |

`Source/Libs` 以下はコンパイル済みライブラリとそのヘッダです(ライブラリ本体は Releases の ZIP にのみ含まれます)。

## ライセンス

本プラグインの利用条件は、株式会社AZSTOKE の利用規約(エンドユーザーライセンス)に従います。ダウンロードまたは使用の前に必ずお読みください。

- 利用規約: https://www.azstoke.jp/rule
- 概要: [LICENSE.md](LICENSE.md)

本プラグイン自体の再配布・改変は禁止されています。

同梱している第三者ソフトウェアの表示は [third_party_notices.txt](third_party_notices.txt) にあります。

## お問い合わせ

https://www.azstoke.jp/contact-supprot
