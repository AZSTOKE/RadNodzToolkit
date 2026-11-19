# PARAKNOT_UE

PARAKNOT_UE は、株式会社AZSTOKE の PARAKNOT(VST3 / AU プラグイン)が配信する音声を、Unreal Engine のアプリで受信するためのプラグインです。
音声は UDP で直接届き、XChaCha20-Poly1305 で暗号化・認証されます。接続先とセッション鍵は RADNODZ 経由で受け取ります。

> 本プラグインの利用には、RADNODZ プラグインと、接続先の PARAKNOT のライセンスが別途必要です。

## 動作環境

- Unreal Engine 5
- 対応プラットフォーム: Windows (x64) / Android (arm64, x86_64) / macOS
- 必要なプラグイン: [RADNODZ](https://www.azstoke.jp/rigdocks/radnodz-howtouse)(`Plugins/RADNODZ_UE` に配置)
- 接続先: PARAKNOT を「配信」にして動かしている PC(RIGDOCKS と RADNODZ プラグインがインストールされていること)

## 導入方法

1. [Releases](../../releases) から `PARAKNOT_UE_v<バージョン>.zip` をダウンロードします。
   (自動で添付される「Source code (zip)」にはコンパイル済みライブラリが含まれないため、使用できません。)
2. ZIP の中にある `PARAKNOT_UE` フォルダを、プロジェクトの `Plugins` フォルダにコピーします(`Plugins/PARAKNOT_UE` となるように配置します)。
   Windows の「すべて展開」を使うと、ZIP 名のフォルダ(`PARAKNOT_UE_v<バージョン>`)の中に `PARAKNOT_UE` ができます。その場合も、中の `PARAKNOT_UE` フォルダだけを `Plugins` に移してください。
3. RADNODZ プラグインも `Plugins/RADNODZ_UE` に配置します。
4. プロジェクトファイルを再生成してビルドします。
5. エディタの「プラグイン」で PARAKNOT を有効にします。

ビルドが必要なため、Blueprint のみのプロジェクトでは利用できません。

> このリポジトリには、コンパイル済みライブラリ(`Source/Libs` 以下の `.lib` / `.a`)は含まれていません。
> リポジトリをクローンしただけではビルドできないため、必ず Releases の ZIP を使用してください。

## 使い方

1. RADNODZ の `URadnodzClient` で接続先の PC に接続します。
2. `UParaknotSessionClient::ConnectToBroadcast()` で、配信中の PARAKNOT の接続先(host / port)とセッション鍵を受け取ります。
3. `UParaknotAudioReceiver::Start()` に受け取った値を渡すと、音声の受信が始まります。

## セキュリティについて

- セッション鍵を持たない相手とは通信しません(接続時にハンドシェイクで確認します)。
- 同じ配信を購読している相手どうしは、同じ鍵(待受ポートごとに 1 本)を共有します。
- 鍵交換に Diffie-Hellman を使っていないため、前方秘匿性はありません。セッション鍵が漏れると、記録された過去の通信も復号できます。
- セッション鍵は保存しないでください。

## 構成

| モジュール | 内容 |
|---|---|
| ParaknotAudio | 接続先の解決(`UParaknotSessionClient`)と音声の受信(`UParaknotAudioReceiver`) |
| AudioLink | 音声の送受信ライブラリ(コンパイル済みライブラリとそのヘッダ) |

`Source/Libs` 以下はコンパイル済みライブラリとそのヘッダです(ライブラリ本体は Releases の ZIP にのみ含まれます)。

## ライセンス

本プラグインの利用条件は、株式会社AZSTOKE の利用規約(エンドユーザーライセンス)に従います。ダウンロードまたは使用の前に必ずお読みください。

- 利用規約: https://www.azstoke.jp/rule
- 概要: [LICENSE.md](LICENSE.md)

本プラグイン自体の再配布・改変は禁止されています。

同梱している第三者ソフトウェアの表示は [third_party_notices.txt](third_party_notices.txt) にあります。

## お問い合わせ

https://www.azstoke.jp/contact-supprot
