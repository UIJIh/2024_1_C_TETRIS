#include "Windows.h"		
#include "WindowManager.h"		
#include "UserManager.h"		
#include "Game.h"		
#include "FileManager.h"		

int main() {
	play_music(); // bgm
	srand((unsigned)time(NULL)); //난수표생성		
	setcursortype(NOCURSOR); //커서 없앰		
	initialize_user_info(); // user, dynamic allocation		
	read_from_file(); // 파일 불러오기 (처음 한번만!)		
	start_window(); // 1번째 화면		
	option_window(); // 2번째 옵션 화면		
	reset(); // 게임 화면		
	cleanup_user_info(); // deallocation		

	return 0;
}