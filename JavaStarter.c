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
					/*����������ꍇ�́A�A�����ĂŎ��s�B*/
					//�f�[�^�����W�B
					const char* cmd_base = CMD_RUN;
					int cmd_base_length = strlen(cmd_base);
					const char* cmd_add = szCmdLine;
					int cmd_add_length = strlen(cmd_add);
					//�������m��
					char* call_cmd_line = malloc(cmd_base_length+cmd_add_length+2);
					//Java�p�̕������R�s�[�Bstrcat�Ƃ��͂���܂�g�������Ȃ��B
					memcpy(call_cmd_line,cmd_base,cmd_base_length);
					//�󔒂�ǉ�
					call_cmd_line[cmd_base_length] = ' ';
					//������ǉ�
					memcpy(call_cmd_line+cmd_base_length+1,cmd_add,cmd_add_length);
					//�Ō��\0��Y�ꂸ�ɁB
					call_cmd_line[cmd_base_length+cmd_add_length+1] = '\0';
					//�R�}���h���s�B
					ret = doCmd(call_cmd_line,TRUE);
					//�Y�ꂸ�ɊJ���B
					free(call_cmd_line);
				}else{/*�����������ꍇ�͕��ʂɎ��s*/
					ret = doCmd(CMD_RUN,TRUE);
				}
			}else{
				MessageBox(NULL,"Java���C���X�g�[������Ă��Ȃ��悤�ł��B","�G���[",MB_OK | MB_ICONERROR);
				ret = -1;
			}
			return 0;
}

int doCmd(char* command,int show_msg){
	int ret = 0;
	//�\���̏���������
	STARTUPINFO startup_info;
	PROCESS_INFORMATION process_info;
	process_info.hProcess = NULL;
	memset(&startup_info, 0, sizeof(STARTUPINFO));
	startup_info.cb = sizeof(STARTUPINFO);
	//�v���Z�X���s
	int code = CreateProcessA(
	    NULL,						// ���s�t�@�C����
	    command,					// �R�}���h���C���p�����[�^
	    NULL,						// �v���Z�X�̕ی쑮��
	    NULL,						// �X���b�h�̕ی쑮��
	    FALSE,						// �I�u�W�F�N�g�n���h���p���̃t���O
	    DETACHED_PROCESS | 
		CREATE_NEW_PROCESS_GROUP | 
		NORMAL_PRIORITY_CLASS,		// �����t���O
	    NULL,						// ���ϐ����ւ̃|�C���^
	    NULL,						// �N�����J�����g�f�B���N�g��
	    &startup_info,				// �E�B���h�E�\���ݒ�
	    &process_info				// �v���Z�X�E�X���b�h�̏��
	);
	if(code == 0){
		ret = -1;
		if(show_msg){
			char* msg;
			int error_code = FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,	// ����t���O
				0,																// ���b�Z�[�W��`�ʒu
				GetLastError(),													// ���b�Z�[�WID
				LANG_USER_DEFAULT,												// ����ID
				(LPTSTR)&msg,													// �o�b�t�@�̃A�h���X
				0,																// �o�b�t�@�̃T�C�Y
				0																// �}����̔z��̃A�h���X
			);
			if(error_code == 0){
				MessageBox(NULL,"���炩�̃G���[���������܂����B","�G���[",MB_OK | MB_ICONERROR);
			}else{
				MessageBox(NULL,msg, "�G���[", MB_ICONERROR|MB_OK);
			}
			LocalFree(msg);
		}
	}
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
 	return ret;
}
