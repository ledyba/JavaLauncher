#include <stdlib.h>
#include <windows.h>

#define CMD_CHECK "java.exe"
#define CMD_RUN "java.exe -jar Leanan-Sidhe.jar"

int doCmd(char* command,int show_msg);

int WINAPI WinMain (HINSTANCE hInstance, 
			HINSTANCE hPrevInstance, 
			PSTR szCmdLine, 
			int iCmdShow){
			int ret;
			if(doCmd(CMD_CHECK,FALSE) == 0){
				if(szCmdLine && strlen(szCmdLine) > 0){
					/*引数がある場合は、連結してで実行。*/
					//データを収集。
					const char* cmd_base = CMD_RUN;
					int cmd_base_length = strlen(cmd_base);
					const char* cmd_add = szCmdLine;
					int cmd_add_length = strlen(cmd_add);
					//メモリ確保
					char* call_cmd_line = malloc(cmd_base_length+cmd_add_length+2);
					//Java用の部分をコピー。strcatとかはあんまり使いたくない。
					memcpy(call_cmd_line,cmd_base,cmd_base_length);
					//空白を追加
					call_cmd_line[cmd_base_length] = ' ';
					//引数を追加
					memcpy(call_cmd_line+cmd_base_length+1,cmd_add,cmd_add_length);
					//最後の\0を忘れずに。
					call_cmd_line[cmd_base_length+cmd_add_length+1] = '\0';
					//コマンド実行。
					ret = doCmd(call_cmd_line,TRUE);
					//忘れずに開放。
					free(call_cmd_line);
				}else{/*引数が無い場合は普通に実行*/
					ret = doCmd(CMD_RUN,TRUE);
				}
			}else{
				MessageBox(NULL,"Javaがインストールされていないようです。","エラー",MB_OK | MB_ICONERROR);
				ret = -1;
			}
			return 0;
}

int doCmd(char* command,int show_msg){
	int ret = 0;
	//構造体初期化処理
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;
	process_info.hProcess = NULL;
	memset(&startup_info, 0, sizeof(STARTUPINFO));
	startup_info.cb = sizeof(STARTUPINFO);
	//プロセス実行
	int code = CreateProcessA(
	    NULL,						// 実行ファイル名
	    command,					// コマンドラインパラメータ
	    NULL,						// プロセスの保護属性
	    NULL,						// スレッドの保護属性
	    FALSE,						// オブジェクトハンドル継承のフラグ
	    DETACHED_PROCESS | 
		CREATE_NEW_PROCESS_GROUP | 
		NORMAL_PRIORITY_CLASS,		// 属性フラグ
	    NULL,						// 環境変数情報へのポインタ
	    NULL,						// 起動時カレントディレクトリ
	    &startup_info,				// ウィンドウ表示設定
	    &process_info				// プロセス・スレッドの情報
	);
	if(code == 0){
		ret = -1;
		if(show_msg){
			char* msg;
			int error_code = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	// 動作フラグ
				0,																// メッセージ定義位置
				GetLastError(),													// メッセージID
				LANG_USER_DEFAULT,												// 言語ID
				(LPTSTR)&msg,													// バッファのアドレス
				0,																// バッファのサイズ
				0																// 挿入句の配列のアドレス
			);
			if(error_code == 0){
				MessageBox(NULL,"何らかのエラーが発生しました。","エラー",MB_OK | MB_ICONERROR);
			}else{
				MessageBox(NULL,msg, "エラー", MB_ICONERROR|MB_OK);
			}
			LocalFree(msg);
		}
	}
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
 	return ret;
}
