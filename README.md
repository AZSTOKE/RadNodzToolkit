# RadNodzToolkit

RadNodzToolkit は、株式会社AZSTOKE の RIGDOCKS / RADNODZ を使って、REAPER での収録作業を
Android 端末や Windows PC から操作するアプリ(RIGDRED)の Unreal Engine プロジェクトです。

主な機能:

- REAPER のトラック・メディアアイテム・マーカーの確認と操作、録音の開始/停止
- ラウドネス計測と音量調整
- REAPER の出力のモニター再生(PARAKNOT / ReaStream)と、端末どうしのグループトーク
- 収録リスト(Excel / CSV / Google スプレッドシート)の読み込みと、結果の書き出し・Slack への共有
- インパルス応答(IR)の測定

## 動作環境

- Unreal Engine 5
- 接続先: RIGDOCKS Bridge と RADNODZ プラグインがインストールされた PC
  (RADNODZ と、使う API に対応する RIGDOCKS プラグインのライセンスが別途必要です)

## Android 版のビルドと署名

このリポジトリでは「For Distribution」(配布用ビルド)を無効にしてあります。
そのままパッケージ化すると、開発用の鍵(`%USERPROFILE%\.android\debug.keystore`)で署名された APK ができます。

Google Play などで配布する場合は、自分の鍵で署名します。

1. 自分の keystore を作成し、`Build/Android/` に置きます。
2. `Config/Android/AndroidEngine.ini` を作成し、署名の設定を書きます。
   ```ini
   [/Script/AndroidRuntimeSettings.AndroidRuntimeSettings]
   KeyStore=<keystore のファイル名>
   KeyAlias=<鍵の別名>
   KeyStorePassword=<keystore のパスワード>
   KeyPassword=<鍵のパスワード(KeyStorePassword と同じなら空欄)>
   ```
3. プロジェクト設定 > パッケージ化 で「For Distribution」を有効にしてから、パッケージ化します。

> keystore と `Config/Android/AndroidEngine.ini` は `.gitignore` で除外しているため、リポジトリには含まれません。
> プロジェクト設定の Android > Distribution Signing から入力すると、パスワードが `Config/DefaultEngine.ini` に保存され、リポジトリに含まれてしまいます。署名の設定は上の `AndroidEngine.ini` に書いてください。

## ライセンス

- 本プロジェクト: [LICENSE.md](LICENSE.md)
- `Plugins/` 配下のプラグイン: 各プラグインのライセンスに従います(改変はできません)。
- 同梱している第三者のソフトウェア: [third_party_notices.txt](third_party_notices.txt)
