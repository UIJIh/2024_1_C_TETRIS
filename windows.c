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
	int x = 5; // Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ				
	int y = 4; // Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ				
	system("cls");

	// �ܼ� ��� ���ڵ� ���� (UTF-8)				
	//SetConsoleOutputCP(65001);				

	// ASCII ��Ʈ ���				
	gotoxy(x, y + 0); printf(" /$$$$$$$$ /$$$$$$$$ /$$$$$$$$ /$$$$$$$  /$$$$$$  /$$$$$$ "); Sleep(100);
	gotoxy(x, y + 1); printf("|__  $$__/| $$_____/|__  $$__/| $$__  $$|_  $$_/ /$$__  $$"); Sleep(100);
	gotoxy(x, y + 2); printf("   | $$   | $$         | $$   | $$  \\ $$  | $$  | $$  \\__/"); Sleep(100);
	gotoxy(x, y + 3); printf("   | $$   | $$$$$      | $$   | $$$$$$$/  | $$  |  $$$$$$ "); Sleep(100);
	gotoxy(x, y + 4); printf("   | $$   | $$__/      | $$   | $$__  $$  | $$   \\____  $$"); Sleep(100);
	gotoxy(x, y + 5); printf("   | $$   | $$         | $$   | $$  \\ $$  | $$   /$$  \\ $$"); Sleep(100);
	gotoxy(x, y + 6); printf("   | $$   | $$$$$$$$   | $$   | $$  | $$ /$$$$$$|  $$$$$$/"); Sleep(100);
	gotoxy(x, y + 7); printf("   |__/   |________/   |__/   |__/  |__/|______/ \\______/ "); Sleep(100);

	gotoxy(x, y + 9); printf("Please Login/Register to Start!");
	gotoxy(x, y + 11); printf("  ��   : Up");
	gotoxy(x, y + 12); printf("��   �� : Shift / Soft Drop");
	gotoxy(x, y + 13); printf("  ��   : Down");
	gotoxy(x, y + 14); printf(" SPACE : Hard Drop");
	gotoxy(x, y + 15); printf("   P   : Pause");
	gotoxy(x, y + 16); printf("  ESC  : Quit");
	gotoxy(x, y + 18); printf("BONUS FOR HARD DROPS");

	// print_scores();				
	for (cnt = 0;; cnt++) {
		// �ܼ� �ڵ� ���				
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Ű�Է��� ������ ���ѷ��� ����				
		if (_kbhit()) break;

		// �����̴� �ִϸ��̼� ȿ��				
		if (cnt % 200 == 0) {
			gotoxy(x + 4, y + 1);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("��");
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		if ((cnt % 200 - 100) == 0) {
			gotoxy(x + 4, y + 1);
			printf("  ");
		}
		if (cnt % 350 == 0) {
			gotoxy(x + 13, y + 2);
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("��");
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		if ((cnt % 350) - 100 == 0) {
			gotoxy(x + 13, y + 2);
			printf("  ");
		}
		Sleep(10); // 0.1�� ������				
	}
	while (_kbhit()) _getch(); // ���ۿ� ��ϵ� Ű���� ����				
}

void login_window() {
	int x = 5; // Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ				
	int y = 4; // Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ				
	//int cnt; // Ÿ��Ʋ �������� ���� ����				
	system("cls");
	// login display on the right side				
	gotoxy(x, y + 0); printf("        ����������������"); Sleep(100);
	gotoxy(x, y + 1); printf("      �����  ����    ������"); Sleep(100);
	gotoxy(x, y + 2); printf(" �����              ���  ��"); Sleep(100);
	gotoxy(x, y + 4); printf("        ���  �������������"); Sleep(100);
	gotoxy(x + 5, y + 2); printf("<1P> L O G I N"); Sleep(100);
	gotoxy(x, y + 5); printf("    If ID already exists, logged in automatically\n");
	gotoxy(x, y + 6); printf("    If not, a new ID created."); Sleep(100);
	gotoxy(x, y + 9); printf("    ! Enter your ID (up to 20 letters) !");
	gotoxy(x, y + 10); printf("    ------------------------------------");
	gotoxy(x, y + 11); printf("    >> ");
	gotoxy(x - 10, y + 11);

	set_current_user(&current_user_1);
	add_userInfo(current_user_1, 0);
	// snprintf(user_name_1, 100, "Player1");				
	gotoxy(x, y + 12); printf("    ------------------------------------");
	//if (!flg_2p) {				
	//    gotoxy(x + 15, y - 1); printf("   �� Press the Any Key to start ! ��");				
	//}				
}

void login_window_2() { // 2p ���!!				
	login_window();
	int x = 5; // Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ				
	int y = 4; // Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ				
	//int cnt; // Ÿ��Ʋ �������� ���� ����				
	system("cls");

	// login display on the right side				
	gotoxy(x, y + 0); printf("        ����������������"); Sleep(100);
	gotoxy(x, y + 1); printf("      �����  ����    ������"); Sleep(100);
	gotoxy(x, y + 2); printf(" �����              ���  ��"); Sleep(100);
	gotoxy(x, y + 4); printf("        ���  �������������"); Sleep(100);
	gotoxy(x + 5, y + 2); printf("<2P> L O G I N"); Sleep(100);
	gotoxy(x, y + 5); printf("  If ID already exists, logged in automatically.\n        If not, a new ID created."); Sleep(100);

	gotoxy(x, y + 9); printf("    ! Enter your ID (up to 20 letters) !");
	gotoxy(x, y + 10); printf("    ------------------------------------");
	gotoxy(x, y + 11); printf("    >> ");
	gotoxy(x - 10, y + 11);

	set_current_user(&current_user_2);
	add_userInfo(current_user_2, 0);
	gotoxy(x, y + 12); printf("    ------------------------------------");
	//gotoxy(x + 15, y - 1); printf("   �� Press the Any Key to start ! ��");				
}

void ranking_window() {
	int x = 10; // ��ŷ ȭ���� ǥ�õǴ� x��ǥ				
	int y = 5;  // ��ŷ ȭ���� ǥ�õǴ� y��ǥ				
	system("cls"); // ȭ�� �ʱ�ȭ				

	//read_from_file();				

	gotoxy(x, y + 0); printf("RANKING");
	gotoxy(x, y + 0); printf("\t\t\t\tPress the Any Key to go back !\n");
	/*   for (int i = 0; i < num_users; i++) {
	gotoxy(x, y + 2 + i); printf("%d. %s %d", i + 1, scores[i].name, scores[i].score); Sleep(100);
	}*/
	print_scores();

	while (_kbhit()) _getch();
	key = _getch();
	option_window();
}

void option_window() {
	int x = 10; // �ɼ� ȭ���� ǥ�õǴ� x��ǥ				
	int y = 5;  // �ɼ� ȭ���� ǥ�õǴ� y��ǥ				
	char choice; // ������� ������ ������ ����				

	system("cls"); // ȭ�� �ʱ�ȭ				

	gotoxy(x, y + 0); printf("1. 1P MODE"); Sleep(100);
	gotoxy(x, y + 1); printf("2. 2P MODE"); Sleep(100);
	gotoxy(x, y + 2); printf("3. RANKING"); Sleep(100);
	gotoxy(x, y + 3); printf("4. BACK"); Sleep(100);

	choice = _getch(); // ������� �Է��� ����				

	switch (choice) {
	case '1':
		login_window(); // �α��� â �Լ� ȣ��				
		Sleep(300); // �����°� ����				
		break;
	case '2':
		set_2P_mode(); // 2p ���				
		login_window_2(); // �α��� â �Լ� ȣ��				
		Sleep(300);
		break;
	case '3':
		ranking_window(); // ��ŷ â �Լ� ȣ��				
		break;
	case '4':
		start_window();
		break;
	default:
		gotoxy(x, y + 6); printf("Invalid choice! Choose 1 or 2 or 3 or 4!"); Sleep(1000);
		option_window(); // �߸��� �Է� �� �ٽ� �ɼ� â���� ���ư���				
		break;
	}
}

void result_2p_window(void) {
	int x = 10; // ��ŷ ȭ���� ǥ�õǴ� x��ǥ				
	int y = 5;  // ��ŷ ȭ���� ǥ�õǴ� y��ǥ				
	system("cls"); // ȭ�� �ʱ�ȭ				

	int user_1_score, user_2_score;

	//read_from_file();				

	gotoxy(x, y + 0); printf("RESULT : 1P VS 2P");
	gotoxy(x, y + 0); printf("\t\t\t\tPress the Any Key to exit !\n");

	user_1_score = get_userInfo(current_user_1);
	user_2_score = get_userInfo(current_user_2);

	if (user_1_score > user_2_score) {
		gotoxy(x, y + 2); printf("WINNER IS (1P) %s!\n", current_user_1);
	}
	else if (user_1_score < user_2_score) {
		gotoxy(x, y + 2); printf("WINNER IS (2P) %s!\n", current_user_2);
	}
	else {
		gotoxy(x, y + 2); printf("Draw!");
	}

	gotoxy(x, y + 4); printf("1P's score : %d\n", user_1_score);
	gotoxy(x, y + 5); printf("2P's score : %d\n", user_2_score);

	while (_kbhit()) _getch();
	key = _getch();
	start_window();		
}