#define _CRT_SECURE_NO_WARNINGS				
#define MAX_NAME_LENGTH 21				
#include "Windows.h"				
#include "WindowManager.h"				
#include "FileManager.h"				
#include "UserManager.h"				
#include "Game.h"				
#include <stdio.h>				
#include <stdlib.h>				
#include <conio.h>				
#include <string.h>				
#include <Windows.h>				
#include <mmsystem.h>				

void start_window() {
	int x = 5; // 타이틀 화면이 표시되는 x좌표				
	int y = 4; // 타이틀 화면이 표시되는 y좌표				
	system("cls");

	// 콘솔 출력 인코딩 설정 (UTF-8)
	// SetConsoleOutputCP(65001);

	// ASCII 아트 출력
	setTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	gotoxy(x, y + 0); printf(" /$$$$$$$$ /$$$$$$$$ /$$$$$$$$ /$$$$$$$  /$$$$$$  /$$$$$$ ");
	Sleep(100);
	setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 1); printf("|__  $$__/| $$_____/|__  $$__/| $$__  $$|_  $$_/ /$$__  $$");
	Sleep(100);
	setTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 2); printf("   | $$   | $$         | $$   | $$  \\ $$  | $$  | $$  \\__/");
	Sleep(100);
	setTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 3); printf("   | $$   | $$$$$      | $$   | $$$$$$$/  | $$  |  $$$$$$ ");
	Sleep(100);
	setTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 4); printf("   | $$   | $$__/      | $$   | $$__  $$  | $$   \\____  $$");
	Sleep(100);
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 5); printf("   | $$   | $$         | $$   | $$  \\ $$  | $$   /$$  \\ $$");
	Sleep(100);
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 6); printf("   | $$   | $$$$$$$$   | $$   | $$  | $$ /$$$$$$|  $$$$$$/");
	Sleep(100);
	setTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 7); printf("   |__/   |________/   |__/   |__/  |__/|______/ \\______/ ");
	Sleep(100);

	// 컨트롤 안내를 네모 박스 안에 가둠
	//setTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // green
	gotoxy(x + 1, y + 11); printf("\t   ┌────────────────────────────────────────┐");
	gotoxy(x + 1, y + 12); printf("\t   │  △   : Up                              │");
	gotoxy(x + 1, y + 13); printf("\t   │ ◁  ▷ : Shift / SoftDrop                │");
	gotoxy(x + 1, y + 14); printf("\t   │  ▽   : Down                            │");
	gotoxy(x + 1, y + 15); printf("\t   │ SPACE : Hard Drop                      │");
	gotoxy(x + 1, y + 16); printf("\t   │   P   : Pause                          │");
	gotoxy(x + 1, y + 17); printf("\t   │  ESC  : Quit                           │");
	gotoxy(x + 1, y + 18); printf("\t   │ BONUS FOR HARD DROPS / COMBOS          │");
	gotoxy(x + 1, y + 19); printf("\t   └────────────────────────────────────────┘");

	setTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	gotoxy(x, y + 9); printf("\t       Press Any Key to Login/Register!");

	// print_scores();

	int cnt = 0;
	while (1) {
		// 콘솔 핸들 얻기
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// 키입력이 있으면 무한루프 종료
		if (_kbhit()) break;

		// 깜빡이는 애니메이션 효과
		if (cnt % 200 == 0) {
			gotoxy(x + 4, y + 1);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("★");
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		if ((cnt % 200 - 100) == 0) {
			gotoxy(x + 4, y + 1);
			printf("  ");
		}
		if (cnt % 350 == 0) {
			gotoxy(x + 13, y + 2);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("☆");
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		if ((cnt % 350) - 100 == 0) {
			gotoxy(x + 13, y + 2);
			printf("  ");
		}
		Sleep(10); // 0.1초 딜레이
		cnt++;
	}
	while (_kbhit()) _getch(); // 버퍼에 기록된 키값을 버림
}

void option_window() {
	int x = 10; // 옵션 화면이 표시되는 x좌표				
	int y = 5;  // 옵션 화면이 표시되는 y좌표				
	char choice; // 사용자의 선택을 저장할 변수				

	system("cls"); // 화면 초기화				

	// ASCII 아트 타이틀
	setTextColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 0); printf(":'#######::'########::'########:'####::'#######::'##::: ##:");
	gotoxy(x, y + 1); printf("'##.... ##: ##.... ##:... ##..::. ##::'##.... ##: ###:: ##:");
	gotoxy(x, y + 2); printf(" ##:::: ##: ##:::: ##:::: ##::::: ##:: ##:::: ##: ####: ##:");
	gotoxy(x, y + 3); printf(" ##:::: ##: ########::::: ##::::: ##:: ##:::: ##: ## ## ##:");
	gotoxy(x, y + 4); printf(" ##:::: ##: ##.....:::::: ##::::: ##:: ##:::: ##: ##. ####:");
	gotoxy(x, y + 5); printf(" ##:::: ##: ##::::::::::: ##::::: ##:: ##:::: ##: ##:. ###:");
	gotoxy(x, y + 6); printf(". #######:: ##::::::::::: ##::::'####:. #######:: ##::. ##:");
	gotoxy(x, y + 7); printf(":.......:::..::::::::::::..:::::....:::.......:::..::::..::");

	// 네모 박스 안에 메뉴 옵션
//	setTextColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY); // pink
	//setTextColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); // white
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // green
	gotoxy(x, y + 9); printf("\t\t ┌─────────────────────────────┐");
	gotoxy(x, y + 10); printf("\t\t │ 1. Single Player Mode       │");
	gotoxy(x, y + 11); printf("\t\t │ 2. Two Player Mode          │");
	gotoxy(x, y + 12); printf("\t\t │ 3. View Rankings            │");
	gotoxy(x, y + 13); printf("\t\t │ 4. EXIT                     │");
	gotoxy(x, y + 14); printf("\t\t └─────────────────────────────┘");

	choice = _getch(); // 사용자의 입력을 받음				

	switch (choice) {
	case '1':
		login_window(); // 로그인 창 함수 호출				
		Sleep(300); // 씹히는거 방지				
		break;
	case '2':
		set_2P_mode(); // 2p 모드				
		login_window_2(); // 로그인 창 함수 호출				
		Sleep(300);
		break;
	case '3':
		ranking_window(); // 랭킹 창 함수 호출				
		break;
	case '4':
		system("cls"); // 화면을 지우고 
		exit(0); // 게임 종료 
		//while (_kbhit()) _getch(); // 버퍼에 기록된 키값을 버림
		//key = _getch();
		//start_window();
		return; // 루프를 빠져나가기 위해 return 사용
	default:
		setTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		gotoxy(x, y + 15); printf("Invalid choice! Please choose 1, 2, 3, or 4!");
		Sleep(1000);
		option_window(); // 잘못된 입력 시 다시 옵션 창으로 돌아가기				
		break;
	}
	while (_kbhit()) _getch(); // 버퍼에 기록된 키값을 버림
}

void login_window() {
	int x = 5; // 타이틀화면이 표시되는 x좌표				
	int y = 4; // 타이틀화면이 표시되는 y좌표				
	//int cnt; // 타이틀 프레임을 세는 변수				
	system("cls");
	// login display on the right side				
	setTextColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 0); printf("  _         ____      ___         __     _  _   "); Sleep(100);
	gotoxy(x, y + 1); printf(" FJ        F __ ]   , ___.       FJ    F L L]  "); Sleep(100);
	gotoxy(x, y + 2); printf("J |       J |--| L  FJ---L]      J  L  J   \| L "); Sleep(100);
	gotoxy(x, y + 3); printf("| |       | |  | | J |  [""L      |  |  | |\   | "); Sleep(100);
	gotoxy(x, y + 4); printf(" F L_____  F L__J J | \___] |     F  J  F L\\  J "); Sleep(100);
	gotoxy(x, y + 5); printf("J________LJ\______/FJ\_____/F     J____LJ__L \\__L"); Sleep(100);
	gotoxy(x, y + 6); printf("|________| J______FJ_____F      |____||__L  J__|"); Sleep(100);
	//setTextColor(FOREGROUND_BLUE);
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // green
	gotoxy(x, y + 9); printf("<1P> L O G I N"); Sleep(100);
	gotoxy(x, y + 10); printf("  If ID already exists, logged in automatically.\n            If not, a new ID created."); Sleep(100);
	gotoxy(x, y + 13); printf("    ! Enter your ID (up to 20 letters) !");
	gotoxy(x, y + 14); printf("    ------------------------------------");
	gotoxy(x, y + 15); printf("    >> ");
	gotoxy(x - 10, y + 15);

	set_current_user(&current_user_1);
	add_userInfo(current_user_1, 0);
	// snprintf(user_name_1, 100, "Player1");				
	gotoxy(x, y + 14); printf("    ------------------------------------");
	//if (!flg_2p) {				
	//    gotoxy(x + 15, y - 1); printf("   ★ Press the Any Key to start ! ☆");				
	//}				
}

void login_window_2() { // 2p 모드!!				
	login_window();
	int x = 5; // 타이틀화면이 표시되는 x좌표				
	int y = 4; // 타이틀화면이 표시되는 y좌표				
	//int cnt; // 타이틀 프레임을 세는 변수				
	system("cls");

	// login display on the right side				
	setTextColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 0); printf("  _         ____      ___         __     _  _   "); Sleep(100);
	gotoxy(x, y + 1); printf(" FJ        F __ ]   , ___.       FJ    F L L]  "); Sleep(100);
	gotoxy(x, y + 2); printf("J |       J |--| L  FJ---L]      J  L  J   \| L "); Sleep(100);
	gotoxy(x, y + 3); printf("| |       | |  | | J |  [""L      |  |  | |\   | "); Sleep(100);
	gotoxy(x, y + 4); printf(" F L_____  F L__J J | \___] |     F  J  F L\\  J "); Sleep(100);
	gotoxy(x, y + 5); printf("J________LJ\______/FJ\_____/F     J____LJ__L \\__L"); Sleep(100);
	gotoxy(x, y + 6); printf("|________| J______FJ_____F      |____||__L  J__|"); Sleep(100);
	//setTextColor(FOREGROUND_BLUE);
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // green
	gotoxy(x, y + 9); printf("<1P> L O G I N"); Sleep(100);
	gotoxy(x, y + 10); printf("  If ID already exists, logged in automatically.\n            If not, a new ID created."); Sleep(100);
	gotoxy(x, y + 13); printf("    ! Enter your ID (up to 20 letters) !");
	gotoxy(x, y + 14); printf("    ------------------------------------");
	gotoxy(x, y + 15); printf("    >> ");
	gotoxy(x - 10, y + 15);

	set_current_user(&current_user_2);
	add_userInfo(current_user_2, 0);
	gotoxy(x, y + 14); printf("    ------------------------------------");
	//gotoxy(x + 15, y - 1); printf("   ★ Press the Any Key to start ! ☆");				
}

void ranking_window() {
	int x = 10; // 랭킹 화면이 표시되는 x좌표				
	int y = 5;  // 랭킹 화면이 표시되는 y좌표				
	system("cls"); // 화면 초기화				

	// ASCII 아트 타이틀
	setTextColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 0); printf(".#####....####...##..##..##..##..######..##..##...####..");
	gotoxy(x, y + 1); printf(".##..##..##..##..###.##..##.##.....##....###.##..##.....");
	gotoxy(x, y + 2); printf(".#####...######..##.###..####......##....##.###..##.###.");
	gotoxy(x, y + 3); printf(".##..##..##..##..##..##..##.##.....##....##..##..##..##.");
	gotoxy(x, y + 4); printf(".##..##..##..##..##..##..##..##..######..##..##...####..");
	gotoxy(x, y + 5); printf(".......................................................");

	//setTextColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	//gotoxy(x, y + 7); printf("\t\tPress Any Button to Go Back");

	// 랭킹 출력 (예시 코드)
	// for (int i = 0; i < num_users; i++) {
	//     gotoxy(x, y + 9 + i); printf("%d. %s %d", i + 1, scores[i].name, scores[i].score); Sleep(100);
	// }

	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	//gotoxy(x, y + 8); printf("");
	print_scores();

	// 키 입력 대기
	while (_kbhit()) _getch();
	key = _getch();
	option_window();
}

void result_2p_window(void) {
	int x = 10; // 랭킹 화면이 표시되는 x좌표				
	int y = 5;  // 랭킹 화면이 표시되는 y좌표				
	system("cls"); // 화면 초기화				

	int user_1_score, user_2_score;

	//read_from_file();				
	setTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	gotoxy(x, y + 0); printf(" ____  _____ ____  _   _ _   _____  ");
	gotoxy(x, y + 1); printf("|  _ \| ____/ ___|| | | | | |_   _| ");
	gotoxy(x, y + 2); printf("| |_) |  _| \___ \| | | | |   | |   ");
	gotoxy(x, y + 3); printf("|  _ <| |___ ___) | |_| | |___| |   ");
	gotoxy(x, y + 4); printf("|_| \_\_____|____/ \___/|_____|_|  ");

	setTextColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 6); printf("-------------------------------------------------");
	gotoxy(x, y + 7); printf("\t      Press Any Key to Exit!");
	gotoxy(x, y + 8); printf("-------------------------------------------------");

	// 사용자 점수 읽기
	user_1_score = get_userInfo(current_user_1);
	user_2_score = get_userInfo(current_user_2);

	// 승자 표시
	setTextColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	if (user_1_score > user_2_score) {
		gotoxy(x, y + 10); printf(" WINNER IS %s(1P) !", current_user_1);
	}
	else if (user_1_score < user_2_score) {
		gotoxy(x, y + 10); printf(" WINNER IS %s(2P) !", current_user_2);
	}
	else {
		gotoxy(x, y + 10); printf(" It's a Draw! ");
	}

	// 점수 표시
	setTextColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	gotoxy(x, y + 12); printf("1P's Score: %d", user_1_score);
	gotoxy(x, y + 13); printf("2P's Score: %d", user_2_score);

	// 키 입력 대기
	while (_kbhit()) _getch(); // 버퍼에 기록된 키값을 버림
	int key = _getch();
	exit(0); // 프로그램 종료
}