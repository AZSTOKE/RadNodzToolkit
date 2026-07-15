// Copyright (c) 2026 AZSTOKE Inc. All rights reserved.
// Licensed under the terms in LICENSE.txt (see project root).
// Modification and redistribution are prohibited except as
// permitted therein.



#include "ReaperLang.h"

namespace ReaperLang
{
	namespace
	{
		struct FLangEntry
		{
			const TCHAR* EN;
			const TCHAR* FR;
			const TCHAR* IT;
			const TCHAR* DE;
			const TCHAR* ES;
			const TCHAR* KO;
			const TCHAR* ZHS;
			const TCHAR* ZHT;
		};

		// 英語文字列をキーにした翻訳辞書。S(JP, EN) の EN 引数がそのままキーになる。
		// 未登録のキーは英語へフォールバックする（LookupTranslation参照）。
		static const FLangEntry GLangTable[] =
		{
			{ TEXT("(no HRTF data)"), TEXT("(aucune donnée HRTF)"), TEXT("(nessun dato HRTF)"), TEXT("(keine HRTF-Daten)"), TEXT("(sin datos HRTF)"), TEXT("(HRTF 데이터 없음)"), TEXT("(无 HRTF 数据)"), TEXT("(無 HRTF 資料)") },
			{ TEXT("- Hz / - bit   in - / out -"), TEXT("- Hz / - bit   entrée - / sortie -"), TEXT("- Hz / - bit   ingresso - / uscita -"), TEXT("- Hz / - bit   Eingang - / Ausgang -"), TEXT("- Hz / - bit   entrada - / salida -"), TEXT("- Hz / - bit   입력 - / 출력 -"), TEXT("- Hz / - bit   输入 - / 输出 -"), TEXT("- Hz / - bit   輸入 - / 輸出 -") },
			{ TEXT("{0} / {1}   in {2}ch / out {3}ch"), TEXT("{0} / {1}   entrée {2}ch / sortie {3}ch"), TEXT("{0} / {1}   ingresso {2}ch / uscita {3}ch"), TEXT("{0} / {1}   Eingang {2}ch / Ausgang {3}ch"), TEXT("{0} / {1}   entrada {2}ch / salida {3}ch"), TEXT("{0} / {1}   입력 {2}ch / 출력 {3}ch"), TEXT("{0} / {1}   输入 {2}ch / 输出 {3}ch"), TEXT("{0} / {1}   輸入 {2}ch / 輸出 {3}ch") },
			{ TEXT("{0} found (tap to connect)"), TEXT("{0} trouvé(s) (touchez pour vous connecter)"), TEXT("{0} trovato/i (tocca per connetterti)"), TEXT("{0} gefunden (zum Verbinden tippen)"), TEXT("{0} encontrado(s) (toca para conectar)"), TEXT("{0}개 발견됨(눌러서 연결)"), TEXT("找到{0}个（点击连接）"), TEXT("找到{0}個（點擊連接）") },
			{ TEXT("Searching…  {0}0-255 : port {1}"), TEXT("Recherche…  {0}0-255 : port {1}"), TEXT("Ricerca…  {0}0-255 : porta {1}"), TEXT("Suche…  {0}0-255 : Port {1}"), TEXT("Buscando…  {0}0-255 : puerto {1}"), TEXT("검색 중…  {0}0-255 : 포트 {1}"), TEXT("搜索中…  {0}0-255 : 端口{1}"), TEXT("搜尋中…  {0}0-255 : 連接埠{1}") },
			{ TEXT("3D Spatial"), TEXT("Spatial 3D"), TEXT("Spaziale 3D"), TEXT("3D-Raumklang"), TEXT("Espacial 3D"), TEXT("3D 공간음향"), TEXT("3D 空间音效"), TEXT("3D 空間音效") },
			{ TEXT("Accept"), TEXT("Accepter"), TEXT("Accetta"), TEXT("Annehmen"), TEXT("Aceptar"), TEXT("수락"), TEXT("接听"), TEXT("接聽") },
			{ TEXT("Add"), TEXT("Ajouter"), TEXT("Aggiungi"), TEXT("Hinzufügen"), TEXT("Añadir"), TEXT("추가"), TEXT("添加"), TEXT("新增") },
			{ TEXT("Add New Track"), TEXT("Ajouter une piste"), TEXT("Aggiungi traccia"), TEXT("Neue Spur hinzufügen"), TEXT("Añadir pista"), TEXT("새 트랙 추가"), TEXT("添加新音轨"), TEXT("新增音軌") },
			{ TEXT("Add under (parent track)"), TEXT("Ajouter sous (piste parente)"), TEXT("Aggiungi sotto (traccia madre)"), TEXT("Hinzufügen unter (übergeordnete Spur)"), TEXT("Añadir bajo (pista principal)"), TEXT("추가 위치(상위 트랙)"), TEXT("添加到(父音轨)"), TEXT("新增至(父音軌)") },
			{ TEXT("Allowed Sender IP"), TEXT("IP expéditeur autorisée"), TEXT("IP mittente consentito"), TEXT("Erlaubte Absender-IP"), TEXT("IP de remitente permitida"), TEXT("허용된 발신 IP"), TEXT("允许的发送方IP"), TEXT("允許的傳送端IP") },
			{ TEXT("All sheets"), TEXT("Toutes les feuilles"), TEXT("Tutti i fogli"), TEXT("Alle Blätter"), TEXT("Todas las hojas"), TEXT("전체 시트"), TEXT("全部工作表"), TEXT("全部工作表") },
			{ TEXT("Angle"), TEXT("Angle"), TEXT("Angolo"), TEXT("Winkel"), TEXT("Ángulo"), TEXT("각도"), TEXT("角度"), TEXT("角度") },
			{ TEXT("App"), TEXT("App"), TEXT("App"), TEXT("App"), TEXT("App"), TEXT("앱"), TEXT("应用"), TEXT("應用程式") },
			{ TEXT("Are you sure you want to delete them?"), TEXT("Voulez-vous vraiment les supprimer ?"), TEXT("Vuoi davvero eliminarle?"), TEXT("Möchten Sie sie wirklich löschen?"), TEXT("¿Seguro que quieres eliminarlas?"), TEXT("정말 삭제하시겠습니까?"), TEXT("确定要删除吗？"), TEXT("確定要刪除嗎？") },
			{ TEXT("Are you sure you want to unlock?"), TEXT("Voulez-vous vraiment déverrouiller ?"), TEXT("Vuoi davvero sbloccare?"), TEXT("Möchten Sie wirklich entsperren?"), TEXT("¿Seguro que quieres desbloquear?"), TEXT("정말 잠금을 해제하시겠습니까?"), TEXT("确定要解锁吗？"), TEXT("確定要解鎖嗎？") },
			{ TEXT("As-is"), TEXT("Tel quel"), TEXT("Come è"), TEXT("Unverändert"), TEXT("Tal cual"), TEXT("그대로"), TEXT("原样"), TEXT("原樣") },
			{ TEXT("As-is (Surround)"), TEXT("Tel quel (Surround)"), TEXT("Come è (Surround)"), TEXT("Unverändert (Surround)"), TEXT("Tal cual (Surround)"), TEXT("그대로(서라운드)"), TEXT("原样(环绕声)"), TEXT("原樣(環繞聲)") },
			{ TEXT("Auto"), TEXT("Auto"), TEXT("Auto"), TEXT("Auto"), TEXT("Auto"), TEXT("자동"), TEXT("自动"), TEXT("自動") },
			{ TEXT("Auto refresh interval (sec)"), TEXT("Intervalle d'actualisation auto (s)"), TEXT("Intervallo aggiornamento auto (s)"), TEXT("Intervall für Auto-Aktualisierung (Sek.)"), TEXT("Intervalo de actualización automática (s)"), TEXT("자동 새로고침 간격(초)"), TEXT("自动刷新间隔(秒)"), TEXT("自動重新整理間隔(秒)") },
			{ TEXT("Auto-assign input ch"), TEXT("Attribution auto des canaux d'entrée"), TEXT("Assegnazione auto canali di ingresso"), TEXT("Eingangskanäle automatisch zuweisen"), TEXT("Asignación automática de canales de entrada"), TEXT("입력 채널 자동 할당"), TEXT("自动分配输入声道"), TEXT("自動指派輸入聲道") },
			{ TEXT("Bluetooth (not connected)"), TEXT("Bluetooth (non connecté)"), TEXT("Bluetooth (non connesso)"), TEXT("Bluetooth (nicht verbunden)"), TEXT("Bluetooth (no conectado)"), TEXT("블루투스(연결 안 됨)"), TEXT("蓝牙(未连接)"), TEXT("藍牙(未連接)") },
			{ TEXT("Bot Token"), TEXT("Jeton du bot"), TEXT("Token del bot"), TEXT("Bot-Token"), TEXT("Token del bot"), TEXT("봇 토큰"), TEXT("Bot令牌"), TEXT("Bot權杖") },
			{ TEXT("Built-in"), TEXT("Intégré"), TEXT("Integrato"), TEXT("Integriert"), TEXT("Integrado"), TEXT("내장"), TEXT("内置"), TEXT("內建") },
			{ TEXT("Built-in Mic"), TEXT("Micro intégré"), TEXT("Microfono integrato"), TEXT("Internes Mikrofon"), TEXT("Micrófono integrado"), TEXT("내장 마이크"), TEXT("内置麦克风"), TEXT("內建麥克風") },
			{ TEXT("Call declined by peer"), TEXT("Appel refusé par le correspondant"), TEXT("Chiamata rifiutata dall'interlocutore"), TEXT("Anruf vom Gegenüber abgelehnt"), TEXT("Llamada rechazada por el interlocutor"), TEXT("상대방이 통화를 거절했습니다"), TEXT("对方拒绝了通话"), TEXT("對方拒絕了通話") },
			{ TEXT("Calling…"), TEXT("Appel en cours…"), TEXT("Chiamata in corso…"), TEXT("Anruf läuft…"), TEXT("Llamando…"), TEXT("호출 중…"), TEXT("呼叫中…"), TEXT("呼叫中…") },
			{ TEXT("Cancel"), TEXT("Annuler"), TEXT("Annulla"), TEXT("Abbrechen"), TEXT("Cancelar"), TEXT("취소"), TEXT("取消"), TEXT("取消") },
			{ TEXT("Cannot connect to spreadsheet"), TEXT("Impossible de se connecter à la feuille de calcul"), TEXT("Impossibile connettersi al foglio di calcolo"), TEXT("Verbindung zur Tabelle nicht möglich"), TEXT("No se puede conectar a la hoja de cálculo"), TEXT("스프레드시트에 연결할 수 없습니다"), TEXT("无法连接到电子表格"), TEXT("無法連接到試算表") },
			{ TEXT("Cannot open file"), TEXT("Impossible d'ouvrir le fichier"), TEXT("Impossibile aprire il file"), TEXT("Datei kann nicht geöffnet werden"), TEXT("No se puede abrir el archivo"), TEXT("파일을 열 수 없습니다"), TEXT("无法打开文件"), TEXT("無法開啟檔案") },
			{ TEXT("Channel Order"), TEXT("Ordre des canaux"), TEXT("Ordine canali"), TEXT("Kanalreihenfolge"), TEXT("Orden de canales"), TEXT("채널 순서"), TEXT("声道顺序"), TEXT("聲道順序") },
			{ TEXT("Checkbox"), TEXT("Case à cocher"), TEXT("Casella di controllo"), TEXT("Kontrollkästchen"), TEXT("Casilla de verificación"), TEXT("체크박스"), TEXT("复选框"), TEXT("核取方塊") },
			{ TEXT("Checked only"), TEXT("Cochées uniquement"), TEXT("Solo selezionate"), TEXT("Nur markierte"), TEXT("Solo marcadas"), TEXT("체크된 항목만"), TEXT("仅已勾选"), TEXT("僅已勾選") },
			{ TEXT("Clear All"), TEXT("Tout effacer"), TEXT("Cancella tutto"), TEXT("Alles zurücksetzen"), TEXT("Borrar todo"), TEXT("전체 해제"), TEXT("全部清除"), TEXT("全部清除") },
			{ TEXT("Close"), TEXT("Fermer"), TEXT("Chiudi"), TEXT("Schließen"), TEXT("Cerrar"), TEXT("닫기"), TEXT("关闭"), TEXT("關閉") },
			{ TEXT("Columns"), TEXT("Colonnes"), TEXT("Colonne"), TEXT("Spalten"), TEXT("Columnas"), TEXT("열 수"), TEXT("列数"), TEXT("欄數") },
			{ TEXT("Connect"), TEXT("Connecter"), TEXT("Connetti"), TEXT("Verbinden"), TEXT("Conectar"), TEXT("연결"), TEXT("连接"), TEXT("連接") },
			{ TEXT("Connect ▶"), TEXT("Connecter ▶"), TEXT("Connetti ▶"), TEXT("Verbinden ▶"), TEXT("Conectar ▶"), TEXT("연결 ▶"), TEXT("连接 ▶"), TEXT("連接 ▶") },
			{ TEXT("Connected"), TEXT("Connecté"), TEXT("Connesso"), TEXT("Verbunden"), TEXT("Conectado"), TEXT("연결됨"), TEXT("已连接"), TEXT("已連接") },
			{ TEXT("Connecting…"), TEXT("Connexion…"), TEXT("Connessione…"), TEXT("Verbindet…"), TEXT("Conectando…"), TEXT("연결 중…"), TEXT("连接中…"), TEXT("連接中…") },
			{ TEXT("Connection"), TEXT("Connexion"), TEXT("Connessione"), TEXT("Verbindung"), TEXT("Conexión"), TEXT("연결"), TEXT("连接"), TEXT("連接") },
			{ TEXT("Connection failed"), TEXT("Échec de la connexion"), TEXT("Connessione non riuscita"), TEXT("Verbindung fehlgeschlagen"), TEXT("Error de conexión"), TEXT("연결 실패"), TEXT("连接失败"), TEXT("連接失敗") },
			{ TEXT("Could not get this device's IP"), TEXT("Impossible d'obtenir l'IP de cet appareil"), TEXT("Impossibile ottenere l'IP di questo dispositivo"), TEXT("IP dieses Geräts konnte nicht ermittelt werden"), TEXT("No se pudo obtener la IP de este dispositivo"), TEXT("이 기기의 IP를 가져올 수 없습니다"), TEXT("无法获取本机IP"), TEXT("無法取得本機IP") },
			{ TEXT("Custom"), TEXT("Personnalisé"), TEXT("Personalizzato"), TEXT("Benutzerdefiniert"), TEXT("Personalizado"), TEXT("사용자 지정"), TEXT("自定义"), TEXT("自訂") },
			{ TEXT("Custom-made"), TEXT("Fait maison"), TEXT("Fatto in casa"), TEXT("Eigenentwicklung"), TEXT("Hecho a medida"), TEXT("자체 제작"), TEXT("自制"), TEXT("自製") },
			{ TEXT("Darker"), TEXT("Plus sombre"), TEXT("Più scuro"), TEXT("Dunkler"), TEXT("Más oscuro"), TEXT("더 어둡게"), TEXT("更暗"), TEXT("更暗") },
			{ TEXT("Decline"), TEXT("Refuser"), TEXT("Rifiuta"), TEXT("Ablehnen"), TEXT("Rechazar"), TEXT("거절"), TEXT("拒绝"), TEXT("拒絕") },
			{ TEXT("Default"), TEXT("Standard"), TEXT("Predefinito"), TEXT("Standard"), TEXT("Predeterminado"), TEXT("기본값"), TEXT("默认"), TEXT("預設") },
			{ TEXT("Delete"), TEXT("Supprimer"), TEXT("Elimina"), TEXT("Löschen"), TEXT("Eliminar"), TEXT("삭제"), TEXT("删除"), TEXT("刪除") },
			{ TEXT("Delete Tracks"), TEXT("Supprimer des pistes"), TEXT("Elimina tracce"), TEXT("Spuren löschen"), TEXT("Eliminar pistas"), TEXT("트랙 삭제"), TEXT("删除音轨"), TEXT("刪除音軌") },
			{ TEXT("Deselect All"), TEXT("Tout désélectionner"), TEXT("Deseleziona tutto"), TEXT("Alle abwählen"), TEXT("Deseleccionar todo"), TEXT("전체 선택 해제"), TEXT("取消全选"), TEXT("取消全選") },
			{ TEXT("Direct Call"), TEXT("Appel direct"), TEXT("Chiamata diretta"), TEXT("Direktanruf"), TEXT("Llamada directa"), TEXT("개별 통화"), TEXT("单独通话"), TEXT("單獨通話") },
			{ TEXT("Disconnect"), TEXT("Déconnecter"), TEXT("Disconnetti"), TEXT("Trennen"), TEXT("Desconectar"), TEXT("연결 해제"), TEXT("断开连接"), TEXT("中斷連接") },
			{ TEXT("Display"), TEXT("Affichage"), TEXT("Visualizzazione"), TEXT("Anzeige"), TEXT("Visualización"), TEXT("표시"), TEXT("显示"), TEXT("顯示") },
			{ TEXT("Distance"), TEXT("Distance"), TEXT("Distanza"), TEXT("Abstand"), TEXT("Distancia"), TEXT("거리"), TEXT("距离"), TEXT("距離") },
			{ TEXT("Done"), TEXT("Terminé"), TEXT("Fatto"), TEXT("Erledigt"), TEXT("Hecho"), TEXT("완료"), TEXT("已完成"), TEXT("已完成") },
			{ TEXT("Dot count"), TEXT("Nombre de points"), TEXT("Numero di pallini"), TEXT("Punktanzahl"), TEXT("Número de puntos"), TEXT("점 개수"), TEXT("圆点数量"), TEXT("圓點數量") },
			{ TEXT("Downmix Coeff"), TEXT("Coeff. de downmix"), TEXT("Coeff. downmix"), TEXT("Downmix-Koeffizient"), TEXT("Coef. de downmix"), TEXT("다운믹스 계수"), TEXT("缩混系数"), TEXT("縮混係數") },
			{ TEXT("Edit ch"), TEXT("Modifier le canal"), TEXT("Modifica canale"), TEXT("Kanal bearbeiten"), TEXT("Editar canal"), TEXT("편집 채널"), TEXT("编辑声道"), TEXT("編輯聲道") },
			{ TEXT("Empty or unreadable file"), TEXT("Fichier vide ou illisible"), TEXT("File vuoto o illeggibile"), TEXT("Datei leer oder nicht lesbar"), TEXT("Archivo vacío o ilegible"), TEXT("파일이 비어 있거나 읽을 수 없습니다"), TEXT("文件为空或无法读取"), TEXT("檔案為空或無法讀取") },
			{ TEXT("Export"), TEXT("Exporter"), TEXT("Esporta"), TEXT("Exportieren"), TEXT("Exportar"), TEXT("내보내기"), TEXT("导出"), TEXT("匯出") },
			{ TEXT("Export path is empty"), TEXT("Le chemin d'exportation est vide"), TEXT("Il percorso di esportazione è vuoto"), TEXT("Exportpfad ist leer"), TEXT("La ruta de exportación está vacía"), TEXT("내보내기 경로가 비어 있습니다"), TEXT("导出路径为空"), TEXT("匯出路徑為空") },
			{ TEXT("Export to"), TEXT("Exporter vers"), TEXT("Esporta in"), TEXT("Exportieren nach"), TEXT("Exportar a"), TEXT("내보낼 위치"), TEXT("导出到"), TEXT("匯出至") },
			{ TEXT("Exported"), TEXT("Exporté"), TEXT("Esportato"), TEXT("Exportiert"), TEXT("Exportado"), TEXT("내보내기 완료"), TEXT("已导出"), TEXT("已匯出") },
			{ TEXT("Fail loading the specified sheet"), TEXT("Échec du chargement de la feuille indiquée"), TEXT("Caricamento del foglio specificato non riuscito"), TEXT("Laden des angegebenen Blatts fehlgeschlagen"), TEXT("Error al cargar la hoja especificada"), TEXT("지정한 시트를 불러오지 못했습니다"), TEXT("指定工作表加载失败"), TEXT("指定工作表載入失敗") },
			{ TEXT("Failed to connect to spreadsheet"), TEXT("Échec de connexion à la feuille de calcul"), TEXT("Connessione al foglio di calcolo non riuscita"), TEXT("Verbindung zur Tabelle fehlgeschlagen"), TEXT("Error al conectar con la hoja de cálculo"), TEXT("스프레드시트 연결에 실패했습니다"), TEXT("连接电子表格失败"), TEXT("連接試算表失敗") },
			{ TEXT("Failed to create Excel"), TEXT("Échec de création du fichier Excel"), TEXT("Creazione del file Excel non riuscita"), TEXT("Erstellen der Excel-Datei fehlgeschlagen"), TEXT("Error al crear el archivo Excel"), TEXT("Excel 생성에 실패했습니다"), TEXT("创建Excel失败"), TEXT("建立Excel失敗") },
			{ TEXT("Failed to start"), TEXT("Échec du démarrage"), TEXT("Avvio non riuscito"), TEXT("Start fehlgeschlagen"), TEXT("Error al iniciar"), TEXT("시작에 실패했습니다"), TEXT("启动失败"), TEXT("啟動失敗") },
			{ TEXT("File"), TEXT("Fichier"), TEXT("File"), TEXT("Datei"), TEXT("Archivo"), TEXT("파일"), TEXT("文件"), TEXT("檔案") },
			{ TEXT("File not found (check path)"), TEXT("Fichier introuvable (vérifiez le chemin)"), TEXT("File non trovato (controlla il percorso)"), TEXT("Datei nicht gefunden (Pfad prüfen)"), TEXT("Archivo no encontrado (verifique la ruta)"), TEXT("파일을 찾을 수 없습니다(경로 확인)"), TEXT("找不到文件(请检查路径)"), TEXT("找不到檔案(請檢查路徑)") },
			{ TEXT("File path is empty"), TEXT("Le chemin du fichier est vide"), TEXT("Il percorso del file è vuoto"), TEXT("Dateipfad ist leer"), TEXT("La ruta del archivo está vacía"), TEXT("파일 경로가 비어 있습니다"), TEXT("文件路径为空"), TEXT("檔案路徑為空") },
			{ TEXT("Film"), TEXT("Film"), TEXT("Film"), TEXT("Film"), TEXT("Película"), TEXT("영화"), TEXT("电影"), TEXT("電影") },
			{ TEXT("Format"), TEXT("Format"), TEXT("Formato"), TEXT("Format"), TEXT("Formato"), TEXT("형식"), TEXT("格式"), TEXT("格式") },
			{ TEXT("Full meter"), TEXT("Vumètre complet"), TEXT("Misuratore completo"), TEXT("Vollmeter"), TEXT("Medidor completo"), TEXT("전체 미터"), TEXT("完整电平表"), TEXT("完整電平表") },
			{ TEXT("Full meter = detailed (heavier). Signal dot = green:signal / red:none (light).\nDot can be 'per channel' or 'one per track' (one = fewest queries, lightest).\nLarger update interval = lighter."),
			  TEXT("Vumètre complet = affichage détaillé (plus lourd). Point de signal = vert:signal / rouge:aucun (léger).\nLe point peut être « par canal » ou « un par piste » (un = moins de requêtes, le plus léger).\nUn intervalle de mise à jour plus grand = plus léger."),
			  TEXT("Misuratore completo = dettagliato (più pesante). Pallino di segnale = verde:segnale / rosso:assente (leggero).\nIl pallino può essere «per canale» o «uno per traccia» (uno = meno richieste, il più leggero).\nUn intervallo di aggiornamento maggiore = più leggero."),
			  TEXT("Vollmeter = detailliert (aufwendiger). Signalpunkt = grün:Signal / rot:keins (leicht).\nDer Punkt kann „pro Kanal\" oder „einer pro Spur\" sein (einer = am wenigsten Abfragen, am leichtesten).\nGrößeres Aktualisierungsintervall = leichter."),
			  TEXT("Medidor completo = detallado (más pesado). Punto de señal = verde:señal / rojo:ninguna (ligero).\nEl punto puede ser «por canal» o «uno por pista» (uno = menos consultas, el más ligero).\nUn intervalo de actualización mayor = más ligero."),
			  TEXT("전체 미터 = 상세 표시(다소 무거움). 신호 점 = 초록:신호 있음 / 빨강:없음(가벼움).\n점은 '채널별' 또는 '트랙당 1개'로 설정 가능(1개 = 요청이 가장 적어 가장 가벼움).\n업데이트 간격을 늘릴수록 가벼워집니다."),
			  TEXT("完整电平表＝详细显示（较重）。信号圆点＝绿色:有信号／红色:无信号（较轻）。\n圆点可选择「按声道」或「每轨道一个」（一个＝查询最少，最轻量）。\n更新间隔越大越轻量。"),
			  TEXT("完整電平表＝詳細顯示（較重）。信號圓點＝綠色:有信號／紅色:無信號（較輕）。\n圓點可選擇「依聲道」或「每軌一個」（一個＝查詢最少，最輕量）。\n更新間隔越大越輕量。") },
			{ TEXT("GoogleSheet Client ID"), TEXT("ID client GoogleSheet"), TEXT("ID client GoogleSheet"), TEXT("GoogleSheet-Client-ID"), TEXT("ID de cliente de GoogleSheet"), TEXT("GoogleSheet 클라이언트 ID"), TEXT("GoogleSheet客户端ID"), TEXT("GoogleSheet用戶端ID") },
			{ TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID"), TEXT("GoogleSheet ID") },
			{ TEXT("HRTF"), TEXT("HRTF"), TEXT("HRTF"), TEXT("HRTF"), TEXT("HRTF"), TEXT("HRTF"), TEXT("HRTF"), TEXT("HRTF") },
			{ TEXT("Head Shadow"), TEXT("Ombre de tête"), TEXT("Ombra della testa"), TEXT("Kopfschatten"), TEXT("Sombra de cabeza"), TEXT("머리 그림자 효과"), TEXT("头部遮蔽"), TEXT("頭部遮蔽") },
			{ TEXT("High"), TEXT("Élevé"), TEXT("Alto"), TEXT("Hoch"), TEXT("Alto"), TEXT("강"), TEXT("强"), TEXT("強") },
			{ TEXT("Home"), TEXT("Accueil"), TEXT("Home"), TEXT("Start"), TEXT("Inicio"), TEXT("홈"), TEXT("主页"), TEXT("主頁") },
			{ TEXT("Ignore"), TEXT("Ignorer"), TEXT("Ignora"), TEXT("Ignorieren"), TEXT("Ignorar"), TEXT("보지 않음"), TEXT("不看"), TEXT("不看") },
			{ TEXT("In Call"), TEXT("En appel"), TEXT("In chiamata"), TEXT("Im Gespräch"), TEXT("En llamada"), TEXT("통화 중"), TEXT("通话中"), TEXT("通話中") },
			{ TEXT("In call (receiving)"), TEXT("En appel (réception)"), TEXT("In chiamata (ricezione)"), TEXT("Im Gespräch (Empfang)"), TEXT("En llamada (recibiendo)"), TEXT("통화 중(수신 있음)"), TEXT("通话中(有接收)"), TEXT("通話中(有接收)") },
			{ TEXT("In call (waiting for peer audio)"), TEXT("En appel (en attente audio du correspondant)"), TEXT("In chiamata (in attesa dell'audio dell'interlocutore)"), TEXT("Im Gespräch (wartet auf Audio vom Gegenüber)"), TEXT("En llamada (esperando audio del interlocutor)"), TEXT("통화 중(상대방 음성 대기)"), TEXT("通话中(等待对方语音)"), TEXT("通話中(等待對方語音)") },
			{ TEXT("Incoming Call"), TEXT("Appel entrant"), TEXT("Chiamata in arrivo"), TEXT("Eingehender Anruf"), TEXT("Llamada entrante"), TEXT("수신 통화"), TEXT("来电"), TEXT("來電") },
			{ TEXT("Input"), TEXT("Entrée"), TEXT("Ingresso"), TEXT("Eingang"), TEXT("Entrada"), TEXT("입력"), TEXT("输入"), TEXT("輸入") },
			{ TEXT("Intercom"), TEXT("Interphone"), TEXT("Interfono"), TEXT("Interkom"), TEXT("Interfono"), TEXT("내선"), TEXT("内线"), TEXT("內線") },
			{ TEXT("Language"), TEXT("Langue"), TEXT("Lingua"), TEXT("Sprache"), TEXT("Idioma"), TEXT("언어"), TEXT("语言"), TEXT("語言") },
			{ TEXT("Licenses"), TEXT("Licences"), TEXT("Licenze"), TEXT("Lizenzen"), TEXT("Licencias"), TEXT("라이선스"), TEXT("许可证"), TEXT("授權") },
			{ TEXT("Lighter"), TEXT("Plus clair"), TEXT("Più chiaro"), TEXT("Heller"), TEXT("Más claro"), TEXT("더 밝게"), TEXT("更亮"), TEXT("更亮") },
			{ TEXT("List"), TEXT("Liste"), TEXT("Elenco"), TEXT("Liste"), TEXT("Lista"), TEXT("목록"), TEXT("列表"), TEXT("清單") },
			{ TEXT("Load"), TEXT("Charger"), TEXT("Carica"), TEXT("Laden"), TEXT("Cargar"), TEXT("불러오기"), TEXT("加载"), TEXT("載入") },
			{ TEXT("Loaded"), TEXT("Chargé"), TEXT("Caricato"), TEXT("Geladen"), TEXT("Cargado"), TEXT("불러오기 완료"), TEXT("已加载"), TEXT("已載入") },
			{ TEXT("Loading…"), TEXT("Chargement…"), TEXT("Caricamento…"), TEXT("Wird geladen…"), TEXT("Cargando…"), TEXT("불러오는 중…"), TEXT("加载中…"), TEXT("載入中…") },
			{ TEXT("Lock"), TEXT("Verrouiller"), TEXT("Blocca"), TEXT("Sperren"), TEXT("Bloquear"), TEXT("잠금"), TEXT("锁定"), TEXT("鎖定") },
			{ TEXT("Locked"), TEXT("Verrouillé"), TEXT("Bloccato"), TEXT("Gesperrt"), TEXT("Bloqueado"), TEXT("잠김"), TEXT("已锁定"), TEXT("已鎖定") },
			{ TEXT("Loudness"), TEXT("Sonie"), TEXT("Volume percepito"), TEXT("Lautheit"), TEXT("Sonoridad"), TEXT("라우드니스"), TEXT("响度"), TEXT("響度") },
			{ TEXT("Low"), TEXT("Faible"), TEXT("Basso"), TEXT("Niedrig"), TEXT("Bajo"), TEXT("약"), TEXT("弱"), TEXT("弱") },
			{ TEXT("Low Latency"), TEXT("Faible latence"), TEXT("Bassa latenza"), TEXT("Geringe Latenz"), TEXT("Baja latencia"), TEXT("저지연"), TEXT("低延迟"), TEXT("低延遲") },
			{ TEXT("Manual (button)"), TEXT("Manuel (bouton)"), TEXT("Manuale (pulsante)"), TEXT("Manuell (Schaltfläche)"), TEXT("Manual (botón)"), TEXT("수동(버튼)"), TEXT("手动(按钮)"), TEXT("手動(按鈕)") },
			{ TEXT("Material name"), TEXT("Nom du matériel"), TEXT("Nome del materiale"), TEXT("Materialname"), TEXT("Nombre del material"), TEXT("소재 이름"), TEXT("素材名称"), TEXT("素材名稱") },
			{ TEXT("Me"), TEXT("Moi"), TEXT("Io"), TEXT("Ich"), TEXT("Yo"), TEXT("나"), TEXT("我"), TEXT("我") },
			{ TEXT("Media name…"), TEXT("Nom du média…"), TEXT("Nome media…"), TEXT("Medienname…"), TEXT("Nombre de medio…"), TEXT("미디어 이름…"), TEXT("媒体名称…"), TEXT("媒體名稱…") },
			{ TEXT("Measure"), TEXT("Mesurer"), TEXT("Misura"), TEXT("Messen"), TEXT("Medir"), TEXT("측정"), TEXT("测量"), TEXT("測量") },
			{ TEXT("Measuring & normalizing…"), TEXT("Mesure et normalisation…"), TEXT("Misurazione e normalizzazione…"), TEXT("Messen & Normalisieren…"), TEXT("Midiendo y normalizando…"), TEXT("측정 및 음량 조정 중…"), TEXT("测量并调整音量中…"), TEXT("測量並調整音量中…") },
			{ TEXT("Measuring & sharing…"), TEXT("Mesure et partage…"), TEXT("Misurazione e condivisione…"), TEXT("Messen & Teilen…"), TEXT("Midiendo y compartiendo…"), TEXT("측정 후 공유 중…"), TEXT("测量并分享中…"), TEXT("測量並分享中…") },
			{ TEXT("Meter"), TEXT("Vumètre"), TEXT("Misuratore"), TEXT("Pegelanzeige"), TEXT("Medidor"), TEXT("미터"), TEXT("电平表"), TEXT("電平表") },
			{ TEXT("Meter Recv"), TEXT("Vumètre (réception)"), TEXT("Misuratore (ricezione)"), TEXT("Pegel-Empfang"), TEXT("Medidor (recepción)"), TEXT("미터 수신"), TEXT("电平接收"), TEXT("電平接收") },
			{ TEXT("Mic"), TEXT("Micro"), TEXT("Microfono"), TEXT("Mikrofon"), TEXT("Micrófono"), TEXT("마이크"), TEXT("麦克风"), TEXT("麥克風") },
			{ TEXT("Mic Input"), TEXT("Entrée micro"), TEXT("Ingresso microfono"), TEXT("Mikrofoneingang"), TEXT("Entrada de micrófono"), TEXT("마이크 입력"), TEXT("麦克风输入"), TEXT("麥克風輸入") },
			{ TEXT("Mic Registration"), TEXT("Enregistrement des micros"), TEXT("Registrazione microfoni"), TEXT("Mikrofonregistrierung"), TEXT("Registro de micrófonos"), TEXT("마이크 등록"), TEXT("麦克风注册"), TEXT("麥克風登錄") },
			{ TEXT("Mic count"), TEXT("Nombre de micros"), TEXT("Numero di microfoni"), TEXT("Anzahl der Mikrofone"), TEXT("Número de micrófonos"), TEXT("마이크 수"), TEXT("麦克风数量"), TEXT("麥克風數量") },
			{ TEXT("Mic name / Format / Input ch"), TEXT("Nom du micro / Format / Canal d'entrée"), TEXT("Nome microfono / Formato / Canale ingresso"), TEXT("Mikrofonname / Format / Eingangskanal"), TEXT("Nombre del micrófono / Formato / Canal de entrada"), TEXT("마이크 이름/구성/입력 채널"), TEXT("麦克风名称／格式／输入声道"), TEXT("麥克風名稱／格式／輸入聲道") },
			{ TEXT("Mid"), TEXT("Moyen"), TEXT("Medio"), TEXT("Mittel"), TEXT("Medio"), TEXT("중"), TEXT("中"), TEXT("中") },
			{ TEXT("Monitor"), TEXT("Monitoring"), TEXT("Monitor"), TEXT("Monitor"), TEXT("Monitor"), TEXT("모니터"), TEXT("监听"), TEXT("監聽") },
			{ TEXT("Monitor Output"), TEXT("Sortie monitoring"), TEXT("Uscita monitor"), TEXT("Monitor-Ausgang"), TEXT("Salida de monitor"), TEXT("모니터 출력"), TEXT("监听输出"), TEXT("監聽輸出") },
			{ TEXT("Monitor/Mic port"), TEXT("Port monitoring/micro"), TEXT("Porta monitor/microfono"), TEXT("Monitor-/Mikrofon-Port"), TEXT("Puerto de monitor/micrófono"), TEXT("모니터/마이크 포트"), TEXT("监听/麦克风端口"), TEXT("監聽/麥克風連接埠") },
			{ TEXT("Mono"), TEXT("Mono"), TEXT("Mono"), TEXT("Mono"), TEXT("Mono"), TEXT("모노"), TEXT("单声道"), TEXT("單聲道") },
			{ TEXT("My name"), TEXT("Mon nom"), TEXT("Il mio nome"), TEXT("Mein Name"), TEXT("Mi nombre"), TEXT("내 이름"), TEXT("我的名字"), TEXT("我的名字") },
			{ TEXT("No"), TEXT("Non"), TEXT("No"), TEXT("Nein"), TEXT("No"), TEXT("아니요"), TEXT("否"), TEXT("否") },
			{ TEXT("No answer"), TEXT("Pas de réponse"), TEXT("Nessuna risposta"), TEXT("Keine Antwort"), TEXT("Sin respuesta"), TEXT("응답 없음"), TEXT("无应答"), TEXT("無應答") },
			{ TEXT("No media"), TEXT("Aucun média"), TEXT("Nessun media"), TEXT("Kein Medium"), TEXT("Sin medios"), TEXT("미디어 없음"), TEXT("无媒体"), TEXT("無媒體") },
			{ TEXT("No mics registered. Please register from Settings."), TEXT("Aucun micro enregistré. Enregistrez-en un depuis les Paramètres."), TEXT("Nessun microfono registrato. Registrane uno dalle Impostazioni."), TEXT("Keine Mikrofone registriert. Bitte in den Einstellungen registrieren."), TEXT("No hay micrófonos registrados. Regístralos desde Ajustes."), TEXT("등록된 마이크가 없습니다. 설정에서 마이크 이름을 등록하세요."), TEXT("尚未注册麦克风。请在「设置」中注册麦克风名称。"), TEXT("尚未登錄麥克風。請在「設定」中登錄麥克風名稱。") },
			{ TEXT("No Reaper found on the same network"), TEXT("Aucun Reaper trouvé sur le même réseau"), TEXT("Nessun Reaper trovato sulla stessa rete"), TEXT("Kein Reaper im selben Netzwerk gefunden"), TEXT("No se encontró Reaper en la misma red"), TEXT("같은 네트워크에서 Reaper를 찾을 수 없습니다"), TEXT("同一网络中未找到Reaper"), TEXT("同一網路中未找到Reaper") },
			{ TEXT("No response"), TEXT("Aucune réponse"), TEXT("Nessuna risposta"), TEXT("Keine Antwort"), TEXT("Sin respuesta"), TEXT("응답 없음"), TEXT("无响应"), TEXT("無回應") },
			{ TEXT("Normal"), TEXT("Normal"), TEXT("Normale"), TEXT("Normal"), TEXT("Normal"), TEXT("표준"), TEXT("标准"), TEXT("標準") },
			{ TEXT("Normalize"), TEXT("Normaliser"), TEXT("Normalizza"), TEXT("Normalisieren"), TEXT("Normalizar"), TEXT("음량 조정"), TEXT("音量调整"), TEXT("音量調整") },
			{ TEXT("Normalized to"), TEXT("Normalisation terminée"), TEXT("Normalizzazione completata"), TEXT("Normalisierung abgeschlossen"), TEXT("Normalización completada"), TEXT("음량 조정 완료"), TEXT("音量调整完成"), TEXT("音量調整完成") },
			{ TEXT("Not connected"), TEXT("Non connecté"), TEXT("Non connesso"), TEXT("Nicht verbunden"), TEXT("No conectado"), TEXT("연결되지 않음"), TEXT("未连接"), TEXT("未連接") },
			{ TEXT("OFF"), TEXT("OFF"), TEXT("OFF"), TEXT("OFF"), TEXT("OFF"), TEXT("OFF"), TEXT("OFF"), TEXT("OFF") },
			{ TEXT("OK"), TEXT("OK"), TEXT("OK"), TEXT("OK"), TEXT("OK"), TEXT("확인"), TEXT("确定"), TEXT("確定") },
			{ TEXT("ON"), TEXT("ON"), TEXT("ON"), TEXT("ON"), TEXT("ON"), TEXT("ON"), TEXT("ON"), TEXT("ON") },
			{ TEXT("Off"), TEXT("Désactivé"), TEXT("Disattivato"), TEXT("Aus"), TEXT("Desactivado"), TEXT("꺼짐"), TEXT("关闭"), TEXT("關閉") },
			{ TEXT("On"), TEXT("Activé"), TEXT("Attivato"), TEXT("An"), TEXT("Activado"), TEXT("켜짐"), TEXT("开启"), TEXT("開啟") },
			{ TEXT("One per track"), TEXT("Un par piste"), TEXT("Uno per traccia"), TEXT("Einer pro Spur"), TEXT("Uno por pista"), TEXT("트랙당 1개"), TEXT("每轨道一个"), TEXT("每軌一個") },
			{ TEXT("Online"), TEXT("En ligne"), TEXT("Online"), TEXT("Online"), TEXT("En línea"), TEXT("온라인"), TEXT("在线"), TEXT("線上") },
			{ TEXT("Open"), TEXT("Ouvrir"), TEXT("Apri"), TEXT("Öffnen"), TEXT("Abrir"), TEXT("열기"), TEXT("打开"), TEXT("開啟") },
			{ TEXT("Output"), TEXT("Sortie"), TEXT("Uscita"), TEXT("Ausgang"), TEXT("Salida"), TEXT("출력"), TEXT("输出"), TEXT("輸出") },
			{ TEXT("Paused"), TEXT("En pause"), TEXT("In pausa"), TEXT("Pausiert"), TEXT("En pausa"), TEXT("일시정지"), TEXT("已暂停"), TEXT("已暫停") },
			{ TEXT("Peer IP"), TEXT("IP du correspondant"), TEXT("IP dell'interlocutore"), TEXT("Gegenstellen-IP"), TEXT("IP del interlocutor"), TEXT("상대방 IP"), TEXT("对方IP"), TEXT("對方IP") },
			{ TEXT("Per channel"), TEXT("Par canal"), TEXT("Per canale"), TEXT("Pro Kanal"), TEXT("Por canal"), TEXT("채널별"), TEXT("按声道"), TEXT("依聲道") },
			{ TEXT("Pick…"), TEXT("Choisir…"), TEXT("Scegli…"), TEXT("Auswählen…"), TEXT("Elegir…"), TEXT("목록에서 선택…"), TEXT("从列表选择…"), TEXT("從清單選擇…") },
			{ TEXT("Port"), TEXT("Port"), TEXT("Porta"), TEXT("Port"), TEXT("Puerto"), TEXT("포트"), TEXT("端口"), TEXT("連接埠") },
			{ TEXT("▶ PLAY"), TEXT("▶ LECTURE"), TEXT("▶ PLAY"), TEXT("▶ PLAY"), TEXT("▶ REPRODUCIR"), TEXT("▶ 재생"), TEXT("▶ 播放"), TEXT("▶ 播放") },
			{ TEXT("■ STOP"), TEXT("■ ARRÊT"), TEXT("■ STOP"), TEXT("■ STOPP"), TEXT("■ DETENER"), TEXT("■ 정지"), TEXT("■ 停止"), TEXT("■ 停止") },
			{ TEXT("REC"), TEXT("ENR."), TEXT("REG."), TEXT("AUFN."), TEXT("GRAB."), TEXT("녹음"), TEXT("录音"), TEXT("錄音") },
			{ TEXT("Press Load to show the configured file."), TEXT("Appuyez sur Charger pour afficher le fichier configuré."), TEXT("Premi Carica per mostrare il file configurato."), TEXT("Tippen Sie auf Laden, um die konfigurierte Datei anzuzeigen."), TEXT("Pulsa Cargar para mostrar el archivo configurado."), TEXT("'불러오기'를 눌러 설정한 파일을 표시합니다."), TEXT("点击「加载」以显示已设置的文件。"), TEXT("點擊「載入」以顯示已設定的檔案。") },
			{ TEXT("REAPER"), TEXT("REAPER"), TEXT("REAPER"), TEXT("REAPER"), TEXT("REAPER"), TEXT("REAPER"), TEXT("REAPER"), TEXT("REAPER") },
			{ TEXT("ReaStream Monitor (Receive)"), TEXT("Monitoring ReaStream (Réception)"), TEXT("Monitor ReaStream (Ricezione)"), TEXT("ReaStream-Monitor (Empfang)"), TEXT("Monitor ReaStream (Recepción)"), TEXT("ReaStream 모니터(수신)"), TEXT("ReaStream监听(接收)"), TEXT("ReaStream監聽(接收)") },
			{ TEXT("Reaper IP"), TEXT("Reaper IP"), TEXT("Reaper IP"), TEXT("Reaper IP"), TEXT("Reaper IP"), TEXT("Reaper IP"), TEXT("Reaper IP"), TEXT("Reaper IP") },
			{ TEXT("Reaper comm time"), TEXT("Temps de communication Reaper"), TEXT("Tempo di comunicazione Reaper"), TEXT("Reaper-Kommunikationszeit"), TEXT("Tiempo de comunicación con Reaper"), TEXT("Reaper 통신 시간"), TEXT("Reaper通信时间"), TEXT("Reaper通訊時間") },
			{ TEXT("Receive Buffer"), TEXT("Tampon de réception"), TEXT("Buffer di ricezione"), TEXT("Empfangspuffer"), TEXT("Búfer de recepción"), TEXT("수신 버퍼"), TEXT("接收缓冲"), TEXT("接收緩衝") },
			{ TEXT("Receiving"), TEXT("Réception"), TEXT("Ricezione"), TEXT("Empfang"), TEXT("Recibiendo"), TEXT("수신 중"), TEXT("接收中"), TEXT("接收中") },
			{ TEXT("Recording"), TEXT("Enregistrement en cours"), TEXT("Registrazione in corso"), TEXT("Aufnahme läuft"), TEXT("Grabando"), TEXT("녹음 중"), TEXT("录音中"), TEXT("錄音中") },
			{ TEXT("Recording List"), TEXT("Liste d'enregistrement"), TEXT("Elenco di registrazione"), TEXT("Aufnahmeliste"), TEXT("Lista de grabación"), TEXT("녹음 리스트"), TEXT("收录列表"), TEXT("收錄清單") },
			{ TEXT("Recording list results"), TEXT("Résultats de la liste d'enregistrement"), TEXT("Risultati dell'elenco di registrazione"), TEXT("Ergebnisse der Aufnahmeliste"), TEXT("Resultados de la lista de grabación"), TEXT("녹음 리스트 결과"), TEXT("收录列表结果"), TEXT("收錄清單結果") },
			{ TEXT("Recv"), TEXT("Récep."), TEXT("Ricez."), TEXT("Empf."), TEXT("Recep."), TEXT("수신"), TEXT("接收"), TEXT("接收") },
			{ TEXT("Recv:"), TEXT("Récep. :"), TEXT("Ricez.:"), TEXT("Empf.:"), TEXT("Recep.:"), TEXT("수신:"), TEXT("接收："), TEXT("接收：") },
			{ TEXT("Refresh"), TEXT("Actualiser"), TEXT("Aggiorna"), TEXT("Aktualisieren"), TEXT("Actualizar"), TEXT("새로고침"), TEXT("刷新"), TEXT("重新整理") },
			{ TEXT("Refresh mode"), TEXT("Mode d'actualisation"), TEXT("Modalità di aggiornamento"), TEXT("Aktualisierungsmodus"), TEXT("Modo de actualización"), TEXT("새로고침 방식"), TEXT("刷新方式"), TEXT("重新整理方式") },
			{ TEXT("Registered mics (tracks added for each)"), TEXT("Micros enregistrés (une piste ajoutée pour chacun)"), TEXT("Microfoni registrati (traccia aggiunta per ciascuno)"), TEXT("Registrierte Mikrofone (je eine Spur wird hinzugefügt)"), TEXT("Micrófonos registrados (se añade una pista por cada uno)"), TEXT("등록된 마이크(각 마이크별로 트랙 추가)"), TEXT("已注册的麦克风(将为每个添加音轨)"), TEXT("已登錄的麥克風(將為每個新增音軌)") },
			{ TEXT("Rescan"), TEXT("Relancer la recherche"), TEXT("Cerca di nuovo"), TEXT("Erneut suchen"), TEXT("Buscar de nuevo"), TEXT("다시 검색"), TEXT("重新搜索"), TEXT("重新搜尋") },
			{ TEXT("Resolving device name…"), TEXT("Résolution du nom de l'appareil…"), TEXT("Risoluzione nome dispositivo…"), TEXT("Gerätename wird ermittelt…"), TEXT("Resolviendo nombre del dispositivo…"), TEXT("기기 이름 확인 중…"), TEXT("正在解析设备名称…"), TEXT("正在解析裝置名稱…") },
			{ TEXT("Ringing… waiting for the peer to answer."), TEXT("Ça sonne… en attente de réponse du correspondant."), TEXT("Squillo in corso… in attesa che l'interlocutore risponda."), TEXT("Es klingelt… wartet auf Antwort des Gegenübers."), TEXT("Sonando… esperando respuesta del interlocutor."), TEXT("호출 중… 상대방의 응답을 기다리는 중입니다."), TEXT("呼叫中… 正在等待对方接听。"), TEXT("呼叫中… 正在等待對方接聽。") },
			{ TEXT("Save"), TEXT("Enregistrer"), TEXT("Salva"), TEXT("Speichern"), TEXT("Guardar"), TEXT("저장"), TEXT("保存"), TEXT("儲存") },
			{ TEXT("Scan"), TEXT("Rechercher"), TEXT("Cerca"), TEXT("Suchen"), TEXT("Buscar"), TEXT("검색"), TEXT("搜索"), TEXT("搜尋") },
			{ TEXT("Script Match"), TEXT("Correspondance script"), TEXT("Corrispondenza script"), TEXT("Skript-Abgleich"), TEXT("Coincidencia de guion"), TEXT("대본 일치"), TEXT("台本匹配"), TEXT("台本比對") },
			{ TEXT("Script match check state"), TEXT("État de coche (correspondance script)"), TEXT("Stato spunta (corrispondenza script)"), TEXT("Häkchenstatus (Skript-Abgleich)"), TEXT("Estado de marca (coincidencia de guion)"), TEXT("대본 일치 체크 상태"), TEXT("台本匹配勾选状态"), TEXT("台本比對勾選狀態") },
			{ TEXT("Script match scope"), TEXT("Portée de correspondance script"), TEXT("Ambito corrispondenza script"), TEXT("Umfang des Skript-Abgleichs"), TEXT("Alcance de coincidencia de guion"), TEXT("대본 일치 대상 범위"), TEXT("台本匹配范围"), TEXT("台本比對範圍") },
			{ TEXT("Search for Reaper (same network)"), TEXT("Rechercher Reaper (même réseau)"), TEXT("Cerca Reaper (stessa rete)"), TEXT("Reaper suchen (gleiches Netzwerk)"), TEXT("Buscar Reaper (misma red)"), TEXT("Reaper 검색(같은 네트워크)"), TEXT("搜索Reaper(同一网络)"), TEXT("搜尋Reaper(同一網路)") },
			{ TEXT("Searching…"), TEXT("Recherche…"), TEXT("Ricerca…"), TEXT("Suche…"), TEXT("Buscando…"), TEXT("검색 중…"), TEXT("搜索中…"), TEXT("搜尋中…") },
			{ TEXT("Select All"), TEXT("Tout sélectionner"), TEXT("Seleziona tutto"), TEXT("Alle auswählen"), TEXT("Seleccionar todo"), TEXT("전체 선택"), TEXT("全选"), TEXT("全選") },
			{ TEXT("Send"), TEXT("Envoyer"), TEXT("Invia"), TEXT("Senden"), TEXT("Enviar"), TEXT("송신"), TEXT("发送"), TEXT("傳送") },
			{ TEXT("Send:"), TEXT("Envoyer :"), TEXT("Invia:"), TEXT("Senden:"), TEXT("Enviar:"), TEXT("송신:"), TEXT("发送："), TEXT("傳送：") },
			{ TEXT("Sent to Slack"), TEXT("Envoyé sur Slack"), TEXT("Inviato su Slack"), TEXT("An Slack gesendet"), TEXT("Enviado a Slack"), TEXT("Slack로 전송했습니다"), TEXT("已发送到Slack"), TEXT("已傳送至Slack") },
			{ TEXT("Sequential"), TEXT("Séquentiel"), TEXT("Sequenziale"), TEXT("Fortlaufend"), TEXT("Secuencial"), TEXT("연번"), TEXT("连号"), TEXT("連號") },
			{ TEXT("Set 'My name' first to use group talk"), TEXT("Définissez d'abord « Mon nom » pour utiliser l'appel de groupe"), TEXT("Imposta prima \"Il mio nome\" per usare la chiamata di gruppo"), TEXT("Legen Sie zuerst „Mein Name\" fest, um die Gruppensprechfunktion zu nutzen"), TEXT("Configura primero «Mi nombre» para usar la llamada grupal"), TEXT("그룹 통화를 사용하려면 먼저 '내 이름'을 설정하세요"), TEXT("使用群组通话前请先设置「我的名字」"), TEXT("使用群組通話前請先設定「我的名字」") },
			{ TEXT("Set Slack first"), TEXT("Configurez d'abord Slack"), TEXT("Configura prima Slack"), TEXT("Zuerst Slack einrichten"), TEXT("Configura Slack primero"), TEXT("먼저 Slack을 설정하세요"), TEXT("请先设置Slack"), TEXT("請先設定Slack") },
			{ TEXT("Set your name; while connected you auto-register. Pick an online peer and press Direct Call to ring them (the call starts once they answer)."),
			  TEXT("Définissez votre nom : une fois connecté, vous êtes enregistré automatiquement. Choisissez un correspondant en ligne et appuyez sur Appel direct pour le contacter (l'appel démarre dès qu'il répond)."),
			  TEXT("Imposta il tuo nome; mentre sei connesso vieni registrato automaticamente. Scegli un interlocutore online e premi Chiamata diretta per contattarlo (la chiamata inizia quando risponde)."),
			  TEXT("Legen Sie Ihren Namen fest; während der Verbindung werden Sie automatisch registriert. Wählen Sie einen Online-Teilnehmer aus und tippen Sie auf Direktanruf, um ihn anzurufen (das Gespräch beginnt, sobald er antwortet)."),
			  TEXT("Configura tu nombre; mientras estés conectado, te registrarás automáticamente. Elige un interlocutor en línea y pulsa Llamada directa para llamarlo (la llamada comenzará cuando responda)."),
			  TEXT("이름을 설정하면 연결 중에 자동으로 등록됩니다. 온라인 상대를 선택하고 '개별 통화'를 눌러 호출하세요(상대가 응답하면 통화가 시작됩니다)."),
			  TEXT("设置名字后，连接期间会自动登记在线状态。选择一位在线对象并点击「单独通话」即可呼叫（对方接听后通话开始）。"),
			  TEXT("設定名字後，連線期間會自動登記在線狀態。選擇一位在線對象並點擊「單獨通話」即可呼叫（對方接聽後通話開始）。") },
			{ TEXT("Settings"), TEXT("Paramètres"), TEXT("Impostazioni"), TEXT("Einstellungen"), TEXT("Ajustes"), TEXT("설정"), TEXT("设置"), TEXT("設定") },
			{ TEXT("Share failed"), TEXT("Échec du partage"), TEXT("Condivisione non riuscita"), TEXT("Teilen fehlgeschlagen"), TEXT("Error al compartir"), TEXT("공유에 실패했습니다"), TEXT("分享失败"), TEXT("分享失敗") },
			{ TEXT("Shared to Slack"), TEXT("Partagé sur Slack"), TEXT("Condiviso su Slack"), TEXT("Auf Slack geteilt"), TEXT("Compartido en Slack"), TEXT("Slack에 공유했습니다"), TEXT("已分享到Slack"), TEXT("已分享至Slack") },
			{ TEXT("Sheet"), TEXT("Feuille"), TEXT("Foglio"), TEXT("Blatt"), TEXT("Hoja"), TEXT("시트"), TEXT("工作表"), TEXT("工作表") },
			{ TEXT("Sheet (empty=all)"), TEXT("Feuille (vide = toutes)"), TEXT("Foglio (vuoto = tutti)"), TEXT("Blatt (leer = alle)"), TEXT("Hoja (vacío = todas)"), TEXT("시트(비어있으면 전체)"), TEXT("工作表(留空=全部)"), TEXT("工作表(留空=全部)") },
			{ TEXT("Show device name/IP"), TEXT("Afficher le nom/IP de l'appareil"), TEXT("Mostra nome/IP del dispositivo"), TEXT("Gerätename/IP anzeigen"), TEXT("Mostrar nombre/IP del dispositivo"), TEXT("기기 이름/IP 표시"), TEXT("显示设备名称/IP"), TEXT("顯示裝置名稱/IP") },
			{ TEXT("Signal"), TEXT("Signal"), TEXT("Segnale"), TEXT("Signal"), TEXT("Señal"), TEXT("신호"), TEXT("信号"), TEXT("信號") },
			{ TEXT("Signal dot (light)"), TEXT("Point de signal (léger)"), TEXT("Pallino di segnale (leggero)"), TEXT("Signalpunkt (leicht)"), TEXT("Punto de señal (ligero)"), TEXT("신호 점(가벼움)"), TEXT("信号圆点(轻量)"), TEXT("信號圓點(輕量)") },
			{ TEXT("Slack"), TEXT("Slack"), TEXT("Slack"), TEXT("Slack"), TEXT("Slack"), TEXT("Slack"), TEXT("Slack"), TEXT("Slack") },
			{ TEXT("Slack Share"), TEXT("Partage Slack"), TEXT("Condivisione Slack"), TEXT("Slack teilen"), TEXT("Compartir en Slack"), TEXT("Slack 공유"), TEXT("Slack分享"), TEXT("Slack分享") },
			{ TEXT("Slack send failed"), TEXT("Échec de l'envoi sur Slack"), TEXT("Invio su Slack non riuscito"), TEXT("Senden an Slack fehlgeschlagen"), TEXT("Error al enviar a Slack"), TEXT("Slack 전송에 실패했습니다"), TEXT("Slack发送失败"), TEXT("Slack傳送失敗") },
			{ TEXT("Spreadsheet ID is empty"), TEXT("L'ID de la feuille de calcul est vide"), TEXT("L'ID del foglio di calcolo è vuoto"), TEXT("Tabellen-ID ist leer"), TEXT("El ID de la hoja de cálculo está vacío"), TEXT("스프레드시트 ID가 비어 있습니다"), TEXT("电子表格ID为空"), TEXT("試算表ID為空") },
			{ TEXT("Stable"), TEXT("Stable"), TEXT("Stabile"), TEXT("Stabil"), TEXT("Estable"), TEXT("안정"), TEXT("稳定"), TEXT("穩定") },
			{ TEXT("Start"), TEXT("Démarrer"), TEXT("Avvia"), TEXT("Start"), TEXT("Iniciar"), TEXT("시작"), TEXT("开始"), TEXT("開始") },
			{ TEXT("Start col"), TEXT("Colonne de début"), TEXT("Colonna iniziale"), TEXT("Startspalte"), TEXT("Columna inicial"), TEXT("시작 열"), TEXT("起始列"), TEXT("起始列") },
			{ TEXT("Start row"), TEXT("Ligne de début"), TEXT("Riga iniziale"), TEXT("Startzeile"), TEXT("Fila inicial"), TEXT("시작 행"), TEXT("起始行"), TEXT("起始行") },
			{ TEXT("Stereo"), TEXT("Stéréo"), TEXT("Stereo"), TEXT("Stereo"), TEXT("Estéreo"), TEXT("스테레오"), TEXT("立体声"), TEXT("立體聲") },
			{ TEXT("Stereo Downmix"), TEXT("Downmix stéréo"), TEXT("Downmix stereo"), TEXT("Stereo-Downmix"), TEXT("Downmix estéreo"), TEXT("스테레오 다운믹스"), TEXT("立体声缩混"), TEXT("立體聲縮混") },
			{ TEXT("Stop"), TEXT("Arrêter"), TEXT("Ferma"), TEXT("Stopp"), TEXT("Detener"), TEXT("정지"), TEXT("停止"), TEXT("停止") },
			{ TEXT("Stopped"), TEXT("Arrêté"), TEXT("Fermato"), TEXT("Gestoppt"), TEXT("Detenido"), TEXT("정지됨"), TEXT("已停止"), TEXT("已停止") },
			{ TEXT("Stopped: pick a peer from the online list and press Direct Call to ring them (manually entered IPs connect immediately instead)."),
			  TEXT("Arrêté : choisissez un correspondant dans la liste en ligne et appuyez sur Appel direct pour le contacter (les IP saisies manuellement se connectent immédiatement)."),
			  TEXT("Fermato: scegli un interlocutore dall'elenco online e premi Chiamata diretta per contattarlo (gli IP inseriti manualmente si connettono immediatamente)."),
			  TEXT("Gestoppt: Wählen Sie einen Teilnehmer aus der Online-Liste und tippen Sie auf Direktanruf, um ihn anzurufen (manuell eingegebene IPs verbinden sich sofort)."),
			  TEXT("Detenido: elige un interlocutor de la lista en línea y pulsa Llamada directa para llamarlo (las IP introducidas manualmente se conectan de inmediato)."),
			  TEXT("정지됨: 온라인 목록에서 상대를 선택하고 '개별 통화'를 눌러 호출하세요(직접 입력한 IP는 즉시 연결됩니다)."),
			  TEXT("已停止：从在线列表选择对象并点击「单独通话」即可呼叫（手动输入IP则会立即连接）。"),
			  TEXT("已停止：從線上清單選擇對象並點擊「單獨通話」即可呼叫（手動輸入IP則會立即連接）。") },
			{ TEXT("Target (LUFS)"), TEXT("Cible (LUFS)"), TEXT("Target (LUFS)"), TEXT("Zielwert (LUFS)"), TEXT("Objetivo (LUFS)"), TEXT("목표(LUFS)"), TEXT("目标(LUFS)"), TEXT("目標(LUFS)") },
			{ TEXT("The following tracks will be deleted."), TEXT("Les pistes suivantes seront supprimées."), TEXT("Le seguenti tracce verranno eliminate."), TEXT("Die folgenden Spuren werden gelöscht."), TEXT("Se eliminarán las siguientes pistas."), TEXT("다음 트랙이 삭제됩니다."), TEXT("将删除以下音轨。"), TEXT("將刪除以下音軌。") },
			{ TEXT("Theme (BG)"), TEXT("Thème (fond)"), TEXT("Tema (sfondo)"), TEXT("Thema (Hintergrund)"), TEXT("Tema (fondo)"), TEXT("테마(배경)"), TEXT("主题(背景)"), TEXT("主題(背景)") },
			{ TEXT("This device IP"), TEXT("IP de cet appareil"), TEXT("IP di questo dispositivo"), TEXT("IP dieses Geräts"), TEXT("IP de este dispositivo"), TEXT("이 기기의 IP"), TEXT("本机IP"), TEXT("本機IP") },
			{ TEXT("To User"), TEXT("Destinataire"), TEXT("Destinatario"), TEXT("An Benutzer"), TEXT("Para usuario"), TEXT("수신 사용자"), TEXT("收件人"), TEXT("收件人") },
			{ TEXT("Track List Refresh"), TEXT("Actualisation de la liste des pistes"), TEXT("Aggiornamento elenco tracce"), TEXT("Aktualisierung der Spurliste"), TEXT("Actualización de lista de pistas"), TEXT("트랙 목록 새로고침"), TEXT("音轨列表刷新"), TEXT("音軌清單重新整理") },
			{ TEXT("Track name…"), TEXT("Nom de la piste…"), TEXT("Nome traccia…"), TEXT("Spurname…"), TEXT("Nombre de pista…"), TEXT("트랙 이름…"), TEXT("音轨名称…"), TEXT("音軌名稱…") },
			{ TEXT("Tracks"), TEXT("Pistes"), TEXT("Tracce"), TEXT("Spuren"), TEXT("Pistas"), TEXT("트랙"), TEXT("音轨"), TEXT("音軌") },
			{ TEXT("Unknown"), TEXT("Inconnu"), TEXT("Sconosciuto"), TEXT("Unbekannt"), TEXT("Desconocido"), TEXT("알 수 없음"), TEXT("未知"), TEXT("未知") },
			{ TEXT("Unchecked only"), TEXT("Non cochées uniquement"), TEXT("Solo non selezionate"), TEXT("Nur nicht markierte"), TEXT("Solo no marcadas"), TEXT("체크 안 된 항목만"), TEXT("仅未勾选"), TEXT("僅未勾選") },
			{ TEXT("Unlock"), TEXT("Déverrouiller"), TEXT("Sblocca"), TEXT("Entsperren"), TEXT("Desbloquear"), TEXT("잠금 해제"), TEXT("解锁"), TEXT("解鎖") },
			{ TEXT("Update interval (ms)"), TEXT("Intervalle de mise à jour (ms)"), TEXT("Intervallo di aggiornamento (ms)"), TEXT("Aktualisierungsintervall (ms)"), TEXT("Intervalo de actualización (ms)"), TEXT("업데이트 간격(ms)"), TEXT("更新间隔(ms)"), TEXT("更新間隔(ms)") },
			{ TEXT("Volume"), TEXT("Volume"), TEXT("Volume"), TEXT("Lautstärke"), TEXT("Volumen"), TEXT("음량"), TEXT("音量"), TEXT("音量") },
			{ TEXT("Width (ITD)"), TEXT("Largeur (ITD)"), TEXT("Ampiezza (ITD)"), TEXT("Breite (ITD)"), TEXT("Amplitud (ITD)"), TEXT("공간감(ITD)"), TEXT("空间感(ITD)"), TEXT("空間感(ITD)") },
			{ TEXT("Yes (Delete)"), TEXT("Oui (Supprimer)"), TEXT("Sì (Elimina)"), TEXT("Ja (Löschen)"), TEXT("Sí (Eliminar)"), TEXT("예(삭제)"), TEXT("是(删除)"), TEXT("是(刪除)") },
			{ TEXT("Yes (Unlock)"), TEXT("Oui (Déverrouiller)"), TEXT("Sì (Sblocca)"), TEXT("Ja (Entsperren)"), TEXT("Sí (Desbloquear)"), TEXT("예(해제)"), TEXT("是(解锁)"), TEXT("是(解鎖)") },
			{ TEXT("e.g. OH"), TEXT("ex. OH"), TEXT("es. OH"), TEXT("z. B. OH"), TEXT("p. ej. OH"), TEXT("예: OH"), TEXT("例：OH"), TEXT("例：OH") },
			{ TEXT("e.g. Steel frame"), TEXT("ex. Charpente métallique"), TEXT("es. Struttura in acciaio"), TEXT("z. B. Stahlrahmen"), TEXT("p. ej. Estructura de acero"), TEXT("예: 철골"), TEXT("例：钢架"), TEXT("例：鋼架") },
			{ TEXT("e.g. Tanaka"), TEXT("ex. Tanaka"), TEXT("es. Tanaka"), TEXT("z. B. Tanaka"), TEXT("p. ej. Tanaka"), TEXT("예: 다나카"), TEXT("例：田中"), TEXT("例：田中") },
			{ TEXT("empty = allow all"), TEXT("vide = tout autoriser"), TEXT("vuoto = consenti tutto"), TEXT("leer = alles erlauben"), TEXT("vacío = permitir todo"), TEXT("비어있으면 모두 허용"), TEXT("留空=全部允许"), TEXT("留空=全部允許") },
			{ TEXT("is ringing..."), TEXT("sonne…"), TEXT("sta squillando…"), TEXT("klingelt…"), TEXT("está sonando…"), TEXT("호출 중…"), TEXT("正在呼叫…"), TEXT("正在呼叫…") },
			{ TEXT("press Direct Call"), TEXT("appuyez sur Appel direct"), TEXT("premi Chiamata diretta"), TEXT("tippen Sie auf Direktanruf"), TEXT("pulsa Llamada directa"), TEXT("'개별 통화'를 눌러주세요"), TEXT("请点击「单独通话」"), TEXT("請點擊「單獨通話」") },
			{ TEXT("rows"), TEXT("lignes"), TEXT("righe"), TEXT("Zeilen"), TEXT("filas"), TEXT("행"), TEXT("行"), TEXT("行") },
			{ TEXT("sheets"), TEXT("feuilles"), TEXT("fogli"), TEXT("Blätter"), TEXT("hojas"), TEXT("시트"), TEXT("工作表"), TEXT("工作表") },
			{ TEXT("■ Stopped"), TEXT("■ Arrêté"), TEXT("■ Fermato"), TEXT("■ Gestoppt"), TEXT("■ Detenido"), TEXT("■ 정지 중"), TEXT("■ 停止中"), TEXT("■ 停止中") },
			{ TEXT("▶ Playing"), TEXT("▶ Lecture"), TEXT("▶ Riproduzione"), TEXT("▶ Wiedergabe"), TEXT("▶ Reproduciendo"), TEXT("▶ 재생 중"), TEXT("▶ 播放中"), TEXT("▶ 播放中") },
			{ TEXT("⚠ Could not start mic. Check mic permission / input device / peer IP."), TEXT("⚠ Impossible de démarrer le micro. Vérifiez l'autorisation du micro / le périphérique d'entrée / l'IP du correspondant."), TEXT("⚠ Impossibile avviare il microfono. Controlla il permesso del microfono / il dispositivo di ingresso / l'IP dell'interlocutore."), TEXT("⚠ Mikrofon konnte nicht gestartet werden. Prüfen Sie Mikrofonberechtigung / Eingabegerät / Gegenstellen-IP."), TEXT("⚠ No se pudo iniciar el micrófono. Comprueba el permiso del micrófono / el dispositivo de entrada / la IP del interlocutor."), TEXT("⚠ 마이크를 시작할 수 없습니다. 마이크 권한/입력 장치/상대방 IP를 확인하세요."), TEXT("⚠ 无法启动麦克风。请检查麦克风权限／输入设备／对方IP。"), TEXT("⚠ 無法啟動麥克風。請檢查麥克風權限／輸入裝置／對方IP。") },

			// ダウンミックス係数の受信chラベル（DmChannelLabelから。JP/ENは変数のため辞書化が必要）
			{ TEXT("L (Front L)"), TEXT("L (avant G)"), TEXT("L (anteriore Sx)"), TEXT("L (vorne links)"), TEXT("L (frontal izq.)"), TEXT("L(전방 좌)"), TEXT("L(前左)"), TEXT("L(前左)") },
			{ TEXT("R (Front R)"), TEXT("R (avant D)"), TEXT("R (anteriore Dx)"), TEXT("R (vorne rechts)"), TEXT("R (frontal der.)"), TEXT("R(전방 우)"), TEXT("R(前右)"), TEXT("R(前右)") },
			{ TEXT("C (Center)"), TEXT("C (centre)"), TEXT("C (centrale)"), TEXT("C (Center)"), TEXT("C (centro)"), TEXT("C(센터)"), TEXT("C(中置)"), TEXT("C(中置)") },
			{ TEXT("Ls (Surround L)"), TEXT("Ls (surround G)"), TEXT("Ls (surround Sx)"), TEXT("Ls (Surround links)"), TEXT("Ls (surround izq.)"), TEXT("Ls(서라운드 좌)"), TEXT("Ls(环绕左)"), TEXT("Ls(環繞左)") },
			{ TEXT("Rs (Surround R)"), TEXT("Rs (surround D)"), TEXT("Rs (surround Dx)"), TEXT("Rs (Surround rechts)"), TEXT("Rs (surround der.)"), TEXT("Rs(서라운드 우)"), TEXT("Rs(环绕右)"), TEXT("Rs(環繞右)") },
			{ TEXT("Lb (Rear L)"), TEXT("Lb (arrière G)"), TEXT("Lb (posteriore Sx)"), TEXT("Lb (hinten links)"), TEXT("Lb (trasero izq.)"), TEXT("Lb(리어 좌)"), TEXT("Lb(后左)"), TEXT("Lb(後左)") },
			{ TEXT("Rb (Rear R)"), TEXT("Rb (arrière D)"), TEXT("Rb (posteriore Dx)"), TEXT("Rb (hinten rechts)"), TEXT("Rb (trasero der.)"), TEXT("Rb(리어 우)"), TEXT("Rb(后右)"), TEXT("Rb(後右)") },
		};

		static const TMap<FString, const FLangEntry*>& GetLangMap()
		{
			static TMap<FString, const FLangEntry*> Map = [&]()
			{
				TMap<FString, const FLangEntry*> M;
				M.Reserve(UE_ARRAY_COUNT(GLangTable));
				for (const FLangEntry& E : GLangTable)
				{
					M.Add(FString(E.EN), &E);
				}
				return M;
			}();
			return Map;
		}
	}

	const TCHAR* NativeName(ELanguage Lang)
	{
		switch (Lang)
		{
		case ELanguage::JP:  return TEXT("日本語");
		case ELanguage::EN:  return TEXT("English");
		case ELanguage::FR:  return TEXT("Français");
		case ELanguage::IT:  return TEXT("Italiano");
		case ELanguage::DE:  return TEXT("Deutsch");
		case ELanguage::ES:  return TEXT("Español");
		case ELanguage::KO:  return TEXT("한국어");
		case ELanguage::ZHS: return TEXT("简体中文");
		case ELanguage::ZHT: return TEXT("繁體中文");
		default:             return TEXT("English");
		}
	}

	ELanguage FromNativeName(const FString& Name)
	{
		for (uint8 i = 0; i < static_cast<uint8>(ELanguage::Count); ++i)
		{
			const ELanguage L = static_cast<ELanguage>(i);
			if (Name.Equals(NativeName(L)))
			{
				return L;
			}
		}
		return ELanguage::JP;
	}

	FString LookupTranslation(ELanguage Lang, const TCHAR* EN)
	{
		if (const FLangEntry* const* Found = GetLangMap().Find(FString(EN)))
		{
			const FLangEntry& E = **Found;
			switch (Lang)
			{
			case ELanguage::FR:  return FString(E.FR);
			case ELanguage::IT:  return FString(E.IT);
			case ELanguage::DE:  return FString(E.DE);
			case ELanguage::ES:  return FString(E.ES);
			case ELanguage::KO:  return FString(E.KO);
			case ELanguage::ZHS: return FString(E.ZHS);
			case ELanguage::ZHT: return FString(E.ZHT);
			default: break;
			}
		}
		// 辞書に未登録（またはJP/EN）なら英語へフォールバック
		return FString(EN);
	}
}
