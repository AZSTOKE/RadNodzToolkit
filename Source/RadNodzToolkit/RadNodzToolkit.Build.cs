// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.md (see project root).

using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using Microsoft.Extensions.Logging;
using UnrealBuildTool;

public class RadNodzToolkit : ModuleRules
{
	public RadNodzToolkit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// HRTFData_*.cpp は自動生成でファイルスコープに同名シンボル(Az/L/R/Reg)を定義するため、
		// ユニティビルドで結合されると再定義・名前衝突を起こす。非ユニティで個別にコンパイルする。
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",
			"UMG", "Slate", "SlateCore",
			"ApplicationCore",
			"Sockets", "Networking",
			"Json",
			"AudioCaptureCore",
			"AndroidPermission",
			"RadnodzClientPlugin",
			// PARAKNOT とのやり取りでは型付きラッパーの無い AZ_PARAKNOT_* を
			// 直接呼ぶ（RpcCall / Execute）。プラグイン DLL は静的ライブラリの
			// シンボルを再公開しないので、こちらでもリンクする。
			"RADNODZ_client",
			// PARAKNOT と音声をやり取りするライブラリ（ビルド済み配布物）。
			"AudioLink",
			"RigdocksSilverPlugin",
			"RigdocksBronzePlugin",
			"RigdocksGoldPlugin",

			// PARAKNOT の配信を購読するモニター経路(Plugins/PARAKNOT_UE)。
			// ReaStream 方式は設定で戻せるよう残してある。
			"ParaknotAudio"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			// UParaknotPullSoundWave (USoundWaveProcedural の派生) が要る。
			// 基底の IAudioProxyDataFactory の実体がこのモジュールにあり、
			// エディタ (モジュールごとの DLL) では足さないとリンクで未解決になる。
			// モノリシック (Game / Android) では無くても通ってしまうので注意。
			"AudioExtensions",
		});

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			// バックグラウンド音声継続用の前面サービスをUPLで組み込む
			// （AndroidManifest追記・Javaソースコピー・GameActivityへのThunk追加）。
			AdditionalPropertiesForReceipt.Add("AndroidPlugin",
				Path.Combine(ModuleDirectory, "RadNodzToolkit_UPL.xml"));

			// FAppEventManager（GameThreadTicker：サスペンド中にGTを短時間動かす）用。
			// Androidはモノリシックリンクなのでヘッダ参照のみでシンボルは解決される。
			PrivateIncludePathModuleNames.Add("Launch");

			// UPL の gameActivityReplacement で使う GameActivity.java.template を生成する。
			GenerateGameActivityTemplate();
		}
	}

	/// <summary>
	/// エンジンの GameActivity.java.template に半角スペースの入力処理を足したものを
	/// Intermediate/RadNodzToolkit/ に書き出す（RadNodzToolkit_UPL.xml から参照する）。
	///
	/// エンジン標準の一体型キーボードは、VirtualKeyboardInputConnection.sendKeyEvent が
	/// 数字/BackSpace/Enter 以外のキーイベントを捨てるため、IME がキーイベントとして送る
	/// 半角スペースがテキストボックスに入らない。エンジン本体は書き換えず、ビルドのたびに
	/// テンプレートを読んで KEYCODE_DEL の分岐の後ろに KEYCODE_SPACE の分岐を差し込む。
	/// 差し込み位置が見つからない（エンジンの更新で形が変わった）場合は警告を出し、
	/// テンプレートをそのまま使う。
	/// </summary>
	private void GenerateGameActivityTemplate()
	{
		string SourcePath = Path.Combine(EngineDirectory, "Build", "Android", "Java", "src",
			"com", "epicgames", "unreal", "GameActivity.java.template");
		string OutputPath = Path.Combine(ModuleDirectory, "..", "..", "Intermediate", "RadNodzToolkit",
			"GameActivity.java.template");

		string Text = File.ReadAllText(SourcePath);
		string NewLine = Text.Contains("\r\n") ? "\r\n" : "\n";

		int MethodIndex = Text.IndexOf("public boolean sendKeyEvent(KeyEvent event)");
		Match DelBranch = MethodIndex < 0 ? Match.Empty : new Regex(
			@"(?<indent>[ \t]*)else if \(event\.getKeyCode\(\) == KeyEvent\.KEYCODE_DEL\)\s*\{[^{}]*\}")
			.Match(Text, MethodIndex);

		if (!DelBranch.Success)
		{
			Logger.LogWarning("RadNodzToolkit: GameActivity.java.template に半角スペースの処理を差し込めませんでした" +
				"（エンジンのテンプレートの形が変わった可能性があります）。テンプレートをそのまま使います。");
		}
		else if (Text.IndexOf("KeyEvent.KEYCODE_SPACE", MethodIndex) < 0)
		{
			string Indent = DelBranch.Groups["indent"].Value;
			string SpaceBranch = NewLine +
				Indent + "else if (event.getKeyCode() == KeyEvent.KEYCODE_SPACE)" + NewLine +
				Indent + "{" + NewLine +
				Indent + "\t// [RadNodzToolkit] half-width space sent as a key event by the IME" + NewLine +
				Indent + "\treplaceSubstring(\" \");" + NewLine +
				Indent + "}";
			int InsertAt = DelBranch.Index + DelBranch.Length;
			Text = Text.Substring(0, InsertAt) + SpaceBranch + Text.Substring(InsertAt);
		}

		// 内容が変わらないときは書き込まない（タイムスタンプを動かして再パッケージを誘発しないため）。
		if (File.Exists(OutputPath) && File.ReadAllText(OutputPath) == Text)
		{
			return;
		}
		Directory.CreateDirectory(Path.GetDirectoryName(OutputPath));
		File.WriteAllText(OutputPath, Text, new UTF8Encoding(false));
	}
}
