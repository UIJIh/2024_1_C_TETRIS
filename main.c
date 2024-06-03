#include "Windows.h"		
#include "WindowManager.h"		
#include "UserManager.h"		
#include "Game.h"		
#include "FileManager.h"		

int main() {
	play_music(); // bgm
	srand((unsigned)time(NULL)); //����ǥ����		
	setcursortype(NOCURSOR); //Ŀ�� ����		
	initialize_user_info(); // user, dynamic allocation		
	read_from_file(); // ���� �ҷ����� (ó�� �ѹ���!)		
	start_window(); // 1��° ȭ��		
	option_window(); // 2��° �ɼ� ȭ��		
	reset(); // ���� ȭ��		
	cleanup_user_info(); // deallocation		

	return 0;
}