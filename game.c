#include <windows.h>
#include <mmsystem.h>
#include "Game.h"
#include "UserManager.h"
#include "FileManager.h"
#include "WindowManager.h"
#include "Windows.h"

#pragma comment(lib, "winmm.lib")

int STATUS_Y_GOAL = 0; //GOAL 정보표시위치Y 좌표 저장 
int STATUS_Y_LEVEL = 0; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE = 0; //SCORE 정보표시위치Y 좌표 저장

int main_color[MAIN_Y][MAIN_X] = { 0 };
int main_org[MAIN_Y][MAIN_X] = { 0 };
int main_cpy[MAIN_Y][MAIN_X] = { 0 };
int bx = 0;
int by = 0; //이동중인 블록의 게임판상의 x,y좌표를 저장 
int key = 0; //키보드로 입력받은 키값을 저장 
int b_type = 0;
int b_rotation = 0;
int b_type_next = 0; // block
int level = 0;
int score = 0;
int level_goal = 0;
int cnt = 0;
int speed = 0;
int blocks[7][4][4][4] = {
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
 {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 
int last_score = 0;
int new_block_on = false; //새로운 블럭이 필요함을 알리는 flag 
int crush_on = 0; //현재 이동중인 블록이 충돌상태인지 알려주는 flag 
int space_key_on = 0; //hard drop상태임을 알려주는 flag 
int random_color[4]; // 랜덤한 색깔 저장
int combo_cnt = 0; // 두 줄 이상 한꺼번에 없애면 콤보

int level_up_on = 0; //다음레벨로 진행(현재 레벨목표가 완료되었음을) 알리는 flag 
int turn_2p = 0; // 2번째 플레이어 차례인지

void reset(void) {

    level = 1; //각종변수 초기화 
    score = 0;
    level_goal = 2;
    key = 0;
    crush_on = 0;
    cnt = 0;
    speed = 100;

    int i, j;
    random_color[2] = rand() % 15 + 1;
    random_color[1] = random_color[2];
    random_color[0] = random_color[1];
    for (i = 0; i < MAIN_Y; i++)
        for (j = 0; j < MAIN_X; j++)
            main_color[i][j] = 7;

    system("cls"); //화면지움 
    reset_main(); // main_org를 초기화 
    draw_map(); // 게임화면을 그림
    draw_main(); // 게임판을 그림 

    b_type_next = rand() % 7; //다음번에 나올 블록 종류를 랜덤하게 생성 
    new_block(); //새로운 블록을 하나 만듦  


    while (true) {
        for (int i = 0; i < 5; i++) { //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 
            check_key(); //키입력확인 
            if (combo_cnt == 1) {
                draw_main_color();
                combo_cnt = 0;
            }
            else
                draw_main(); //화면을 그림 
            Sleep(speed); //게임속도조절 
            if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
            //블록이 충돌중인경우 추가로 이동및 회전할 시간을 갖음 
            if (space_key_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
                space_key_on = 0;
                break;
            }
        }
        drop_block(); // 블록을 한칸 내림 
        check_level_up(); // 레벨업을 체크 
        check_game_over(); //게임오버를 체크 
        if (new_block_on == true) new_block(); // 뉴 블럭 flag가 있는 경우 새로운 블럭 생성 
    }
}

void reset_main(void) {
    int i, j;

    for (i = 0; i < MAIN_Y; i++) { // 게임판을 0으로 초기화  
        for (j = 0; j < MAIN_X; j++) {
            main_org[i][j] = 0;
            main_cpy[i][j] = 100;
        }
    }

    // 천장
    for (i = 1; i < MAIN_Y - 1; i++) {
        main_org[i][0] = CEILLING; // y값이 0인 위치에 천장을 만듦 (왼쪽 벽으로 변경)
    }

    // 위, 아래 벽을 만듦
    for (j = 0; j < MAIN_X; j++) {
        main_org[0][j] = WALL; // 위쪽 벽
        main_org[MAIN_Y - 1][j] = WALL; // 아래쪽 벽
    }

    // 바닥
    for (i = 0; i < MAIN_Y; i++) {
        main_org[i][MAIN_X - 1] = WALL; // 오른쪽 벽으로 변경
    }
}

void reset_main_cpy(void) { //main_cpy를 초기화 
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {         //게임판에 게임에 사용되지 않는 숫자를 넣음 
        for (j = 0; j < MAIN_X; j++) {  //이는 main_org와 같은 숫자가 없게 하기 위함 
            main_cpy[i][j] = 100;
        }
    }
}

void new_block(void) { //새로운 블록 생성  
    int i, j;

    bx = 1; //블록 생성 위치x좌표f를 게임판의 왼쪽 가장자리로 설정
    by = 5;  //블록 생성위치 y좌표(제일 위) 
    b_type = b_type_next; // 다음 타입 블럭을 현재 블록 타입으로 설정
    b_type_next = rand() % 7; //다음 블럭 타입을 랜덤하게 잡기
    b_rotation = 0;  // 초기 회전 상태 == 상태 0 

    new_block_on = false; //new_block flag를 끔  

    for (i = 0; i < 4; i++) { //게임판 bx, by위치에 블럭생성  
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) {
                main_org[by + i][bx + j] = ACTIVE_BLOCK;
                random_color[3] = rand() % 15 + 1;
            }
        }
    }
    for (i = 1; i < 3; i++) { //게임상태표시에 다음에 나올블럭을 그림 
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                textcolor(random_color[3]);
                printf("■");
                textcolor(7);
            }
            else {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                printf("  ");
            }
        }
    }
}

void check_key(void) {
    key = 0; // 키값 초기화  

    if (_kbhit()) { // 키입력이 있는 경우  
        key = _getch(); // 키값을 받음
        if (key == 224) { // 방향키인 경우 
            do { key = _getch(); } while (key == 224); // 방향키 지시값을 버림 
            switch (key) {
            case UP: // 위쪽키 눌렀을 때 위로 이동
                if (check_crush(bx, by, b_rotation % 4) == true) move_block(UP); // 위로 이동
                break;
            case RIGHT: // 오른쪽 키를 눌렀을 때 오른쪽으로 이동
                if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT); // 오른쪽으로 이동
                break;
            case DOWN: // 아래쪽 키를 눌렀을 때 아래로 이동
                if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); // 아래로 이동
                break;
            case LEFT: // 왼쪽 키 눌렀을 때 회전
                if (check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100); // 회전 동작 (특수 동작)
                break;
            }
        }
        else { // 방향키가 아닌 경우 
            switch (key) {
            case SPACE: // 스페이스 키를 눌렀을 때 
                space_key_on = 1; // 스페이스 키 플래그를 띄움 
                while (crush_on == 0) { // 바닥에 닿을 때까지 이동시킴 
                    drop_block();
                    score += level; // hard drop 보너스
                    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); // 점수 표시  
                }
                break;
            case P: // P(대문자) 눌렀을 때 
            case p: // p(소문자) 눌렀을 때 
                pause(); // 일시정지 
                break;
            case ESC: // ESC 눌렀을 때 
                system("cls"); // 화면을 지우고 
                exit(0); // 게임 종료 
            }
        }
    }
    while (_kbhit()) _getch(); // 키버퍼를 비움 
}

void drop_block(void) {
    int i, j;

    // 블록이 오른쪽 벽에 닿았는지 확인
    if (crush_on && check_crush(bx + 1, by, b_rotation) == false) {
        for (i = 0; i < MAIN_Y; i++) {
            for (j = 0; j < MAIN_X; j++) {
                if (main_org[i][j] == ACTIVE_BLOCK) {
                    main_org[i][j] = INACTIVE_BLOCK;
                    main_color[i][j] = random_color[0];
                }
                random_color[1] = random_color[2];
                random_color[2] = random_color[3];
            }
        }
        crush_on = false;
        check_line(); // 라인 체크
        new_block_on = 1; //새로운 블럭생성 flag를 켬  
        check_game_over(); // 게임 오버 검사
        return;
    }

    // 블록이 아래에 닿았는지 확인
    if (crush_on && check_crush(bx, by + 1, b_rotation) == true) {
        crush_on = false;
    }
    if (crush_on && check_crush(bx, by + 1, b_rotation) == false) {
        for (i = 0; i < MAIN_Y; i++) {
            for (j = 0; j < MAIN_X; j++) {
                if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = INACTIVE_BLOCK;
            }
        }
        crush_on = true;
        check_line();
        new_block_on = 1;
        check_game_over(); // 게임 오버 검사
        return;
    }

    if (check_crush(bx + 1, by, b_rotation) == true) {
        move_block(RIGHT); // 블록을 오른쪽으로 이동
    }
    if (check_crush(bx + 1, by, b_rotation) == false) {
        crush_on++; //밑으로 이동이 안되면  crush flag를 켬
        random_color[0] = random_color[1];
    }
}

int check_crush(int bx, int by, int b_rotation) {
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) {
                // 벽이나 다른 고정된 블록과 충돌하는지 확인
                if (main_org[by + i][bx + j] == WALL || main_org[by + i][bx + j] == INACTIVE_BLOCK) {
                    return false;
                }
            }
        }
    }
    return true;
}

void move_block(int dir) {
    int i, j;

    switch (dir) {
    case LEFT: // 제자리 회전
        if (check_crush(bx, by, (b_rotation + 1) % 4)) {
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = EMPTY;
                    }
                }
            }

            b_rotation = (b_rotation + 1) % 4;

            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        if (by + i >= 0) { // 위쪽 벽을 넘어서지 않도록
                            main_org[by + i][bx + j] = ACTIVE_BLOCK;
                        }
                    }
                }
            }
        }
        break;

    case RIGHT: // 오른쪽 방향
        if (check_crush(bx + 1, by, b_rotation)) {
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = EMPTY;
                    }
                }
            }
            bx++;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = ACTIVE_BLOCK;
                    }
                }
            }
        }
        break;

    case DOWN: // 아래 방향
        if (check_crush(bx, by + 1, b_rotation)) {
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = EMPTY;
                    }
                }
            }
            by++;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = ACTIVE_BLOCK;
                    }
                }
            }
        }
        break;

    case UP: // 위쪽 방향
        if (check_crush(bx, by - 1, b_rotation)) {
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = EMPTY;
                    }
                }
            }
            by--;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = ACTIVE_BLOCK;
                    }
                }
            }
        }
        break;

    case 100: // 블록이 바닥, 혹은 다른 블록과 닿은 상태에서 한 칸 위로 올려 회전이 가능한 경우 
        // 이를 동작시키는 특수동작 
        if (check_crush(bx + 1, by, (b_rotation + 1) % 4)) { // 회전 시킨 블록 불러와서 회전 가능 여부 확인
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = EMPTY;
                    }
                }
            }
            b_rotation = (b_rotation + 1) % 4; // 회전
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = ACTIVE_BLOCK; // 한 칸 회전된 b_lotation 반영
                    }
                }
            }
        }
        break;
    }
}

void check_line(void) {
    int i, j, k;
    int lines_cleared = 0;

    for (j = 1; j < MAIN_X - 1; j++) { // 각 열을 검사
        int filled = 1; // 가득 찬 열을 확인하기 위한 플래그

        for (i = 1; i < MAIN_Y - 1; i++) { // 위와 아래 행은 벽이므로 제외
            if (main_org[i][j] == EMPTY) { // 빈칸이 있으면
                filled = 0; // 가득 찬 열이 아님
                break;
            }
        }

        if (filled) { // 한 열이 가득 찬 경우
            lines_cleared++; // 지운 열 수 증가
            for (k = j; k > 1; k--) { // 현재 열부터 왼쪽으로 모든 열을 한 칸씩 이동
                for (i = 1; i < MAIN_Y - 1; i++) {
                    main_org[i][k] = main_org[i][k - 1];
                    main_color[i][k] = main_color[k - 1][i];
                }
            }
            for (i = 1; i < MAIN_Y - 1; i++) { // 맨 왼쪽 열을 빈칸으로 만듦
                main_org[i][1] = EMPTY;
                main_color[k][1] = 7;

            }
            j--; // 같은 열을 다시 검사 (한 열이 지워졌으므로)
        }
    }

    // 점수와 레벨 목표 업데이트
    if (lines_cleared > 0) {
        score += lines_cleared * 100; // 지운 열 수에 따른 점수 추가
        cnt += lines_cleared; // 현재 레벨에서 지운 열 수 증가
        Sleep(500);
        reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.
        // 레벨 목표 달성 여부 확인
        if (cnt >= level_goal) {
            level++; // 레벨 업
            cnt = 0; // 지운 열 수 초기화
            level_goal += 10; // 다음 레벨 목표 증가
            speed = (speed > 1) ? speed - 1 : 1; // 속도 증가 (속도 감소)

            // 레벨 업 메시지 출력
            gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level);
            //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", levekl_goal);
            gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
        }
        else {
            // 점수와 레벨 목표 표시 업데이트
            gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
            //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", level_goal - cnt);
        }
    }
}

void check_level_up(void) {
    int i, j;

    if (cnt >= 1) { //레벨별로 2줄씩 없애야함.
        draw_main();
        level_up_on = 1; //레벨업 flag를 띄움 
        level += 1; //레벨을 1 올림 
        cnt = 0; //지운 줄수 초기화   

        for (i = 0; i < 2; i++) {
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("             ");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("             ");
            Sleep(200);

            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("☆LEVEL UP!☆");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("☆SPEED UP!☆");
            Sleep(200);
        }
        reset_main_cpy(); //텍스트를 지우기 위해 main_cpy을 초기화.
        //(main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 됨) 

        // 레벨업 보상으로 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌
        for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) {
            for (j = 5; j < MAIN_X - 1; j++) {

                main_org[i][j] = INACTIVE_BLOCK; // 줄을 블록으로 모두 채우고 
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // 별을 찍어줌.. 이뻐보이게 
                printf("★");

                Sleep(10); // Sleep 시간을 줄임
            }
        }
        Sleep(100); //별찍은거 보여주기 위해 delay 
        check_line(); //블록으로 모두 채운것 지우기
        //.check_line()함수 내부에서 level up flag가 켜져있는 경우 점수는 없음.         
        switch (level) { //레벨별로 속도를 조절해줌. 
        case 2:
            speed = 50;
            break;
        case 3:
            speed = 25;
            break;
        case 4:
            speed = 15;
            break;
        case 5:
            speed = 10;
            break;
        case 6:
            speed = 5;
            break;
        case 7:
            speed = 4;
            break;
        case 8:
            speed = 3;
            break;
        case 9:
            speed = 2;
            break;
        case 10:
            speed = 1;
            break;
        }
        level_up_on = 0; //레벨업 flag꺼줌

        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨표시 
        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // 레벨목표 표시 

    }
}

void check_game_over(void) {
    int i;
    int x = 5;
    int y = 5;

    // 1p 모드
    if (!flg_2p) {
        // 왼쪽 첫 번째 열에 블록이 쌓여 있는지 확인
        for (i = 0; i < MAIN_Y - 1; i++) {
            if (main_org[i][1] == INACTIVE_BLOCK) { // 왼쪽 첫 번째 열에 굳혀진 블록이 있는지 확인
                gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); // 게임 오버 메시지
                gotoxy(x, y + 1); printf("▤                              ▤");
                gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
                gotoxy(x, y + 3); printf("▤  |  G A M E  O V E R..   |   ▤");
                gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
                gotoxy(x, y + 5); printf("▤   YOUR SCORE: %6d         ▤", score);
                gotoxy(x, y + 6); printf("▤                              ▤");
                gotoxy(x, y + 7); printf("▤  Press any key to restart..  ▤");
                gotoxy(x, y + 8); printf("▤                              ▤");
                gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
                last_score = score; // 게임 점수를 저장

                add_userInfo(current_user_1, score); // 랭킹 업데이트 (파일에 저장)

                if (score > best_score) { //최고기록 갱신시                     
                    gotoxy(x, y + 6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");
                }
                Sleep(1000);
                while (_kbhit()) _getch();
                key = _getch();
                reset();
            }
        }
    }

    // 2p 모드
    else {
        if (!turn_2p) {
            for (i = 0; i < MAIN_Y - 1; i++) {
                if (main_org[i][1] == INACTIVE_BLOCK) { // 왼쪽 첫 번째 열에 굳혀진 블록이 있는지 확인
                    gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); // 게임 오버 메시지
                    gotoxy(x, y + 1); printf("▤                                       ▤");
                    gotoxy(x, y + 2); printf("▤       +-----------------------+       ▤");
                    gotoxy(x, y + 3); printf("▤       |  G A M E  O V E R..   |       ▤");
                    gotoxy(x, y + 4); printf("▤       +-----------------------+       ▤");
                    gotoxy(x, y + 5); printf("▤        YOUR SCORE: %6d             ▤", score);
                    gotoxy(x, y + 6); printf("▤                                       ▤");
                    gotoxy(x, y + 7); printf("▤  Press any key for 2.Player to start  ▤");
                    gotoxy(x, y + 8); printf("▤                                       ▤");
                    gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
                    last_score = score; // 게임 점수를 저장

                    add_userInfo(current_user_1, score); // 랭킹 업데이트 (파일에 저장)
                    if (score > best_score) { //최고기록 갱신시                     
                        gotoxy(x, y + 6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");
                    }
                    Sleep(1000);
                    while (_kbhit()) _getch();
                    key = _getch();
                    // 플레이어 2차례
                    turn_2p = 1;
                    reset();
                }
            }
        }
        // turn_2p
        else {
            for (i = 0; i < MAIN_Y - 1; i++) {
                if (main_org[i][1] == INACTIVE_BLOCK) { // 왼쪽 첫 번째 열에 굳혀진 블록이 있는지 확인
                    gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); // 게임 오버 메시지
                    gotoxy(x, y + 1); printf("▤                                      ▤");
                    gotoxy(x, y + 2); printf("▤       +-----------------------+      ▤");
                    gotoxy(x, y + 3); printf("▤       |  G A M E  O V E R..   |      ▤");
                    gotoxy(x, y + 4); printf("▤       +-----------------------+      ▤");
                    gotoxy(x, y + 5); printf("▤        YOUR SCORE: %6d            ▤", score);
                    gotoxy(x, y + 6); printf("▤                                      ▤");
                    gotoxy(x, y + 7); printf("▤   Press any key to view the result   ▤");
                    gotoxy(x, y + 8); printf("▤                                      ▤");
                    gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
                    last_score = score; // 게임 점수를 저장

                    add_userInfo(current_user_2, score); // 랭킹 업데이트 (파일에 저장)
                    if (score > best_score) { //최고기록 갱신시                     
                        gotoxy(x, y + 6); printf("▤  ★★★ BEST SCORE! ★★★   ▤  ");
                    }
                    Sleep(1000);
                    while (_kbhit()) _getch();
                    key = _getch();
                    // 플레이어 차례 reset
                    flg_2p = 0;
                    // 1p, 2p 점수 비교 화면 (누가 이겼는지)
                    result_2p_window();
                }
            }
        }
    }
}

void pause(void) { //게임 일시정지 함수 
    int i, j;

    int x = 5;
    int y = 5;

    for (i = 1; i < MAIN_X - 2; i++) { //게임 일시정지 메세지 
        gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
        gotoxy(x, y + 1); printf("▤                              ▤");
        gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
        gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
        gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
        gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
        gotoxy(x, y + 6); printf("▤                              ▤");
        gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
    }
    _getch(); //키입력시까지 대기 

    system("cls"); //화면 지우고 새로 그림 
    reset_main_cpy();
    draw_main();
    draw_map();

    for (i = 1; i < 3; i++) { // 다음블록 그림 
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("■");
            }
            else {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("  ");
            }
        }
    }
} //끝! 

void play_music() {
    char c[256] = "";
    PlaySound(NULL, 0, 0);
    Sleep(10); // 0.01초 대기				
    PlaySound(TEXT("tetris-bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void stop_music() {
    PlaySound(NULL, 0, 0);
}

// game-window
void draw_map(void) { //게임 상태 표시를 나타내는 함수  
    int y = 3;             // level, goal, score만 게임중에 값이 바뀔수 도 있음 그 y값을 따로 저장해둠 
    // 그래서 혹시 게임 상태 표시 위치가 바뀌어도 그 함수에서 안바꿔도 되게.. 
    gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y); printf(" LEVEL : %5d", level);
    //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y + 1); printf(" GOAL  : %5d", 10 - cnt);
    gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
    gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
    gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
    gotoxy(STATUS_X_ADJ, y + 8); printf(" YOUR SCORE :");
    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", score);
    gotoxy(STATUS_X_ADJ, y + 10); printf(" LAST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 11); printf("        %6d", last_score);
    gotoxy(STATUS_X_ADJ, y + 12); printf(" BEST SCORE :");
    gotoxy(STATUS_X_ADJ, y + 13); printf("        %6d", best_score);
    gotoxy(STATUS_X_ADJ, y + 15); printf("  △   : Up                  SPACE : Hard Drop");
    gotoxy(STATUS_X_ADJ, y + 16); printf("◁   ▷ : Shift / Soft Drop   P   : Pause");
    gotoxy(STATUS_X_ADJ, y + 17); printf("  ▽   : Down                ESC  : Quit");
}

void draw_main(void) { // 게임판 그리는 함수 
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { // cpy랑 비교해서 값이 달라진 부분만 새로 그려줌
                // 이게 없으면 게임판 전체를 계속 그려서 느려지고 반짝거림
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                case EMPTY: // 빈칸 모양 
                    printf("  ");
                    break;
                case CEILLING: // 천장 모양 
                    printf(". ");
                    break;
                case WALL: // 벽 모양 
                    printf("▩");
                    break;
                case INACTIVE_BLOCK: // 굳은 블럭 모양  
                    textcolor(random_color[0]);
                    printf("□");
                    textcolor(7);
                    break;
                case ACTIVE_BLOCK: // 움직이고 있는 블럭 모양  
                    textcolor(random_color[2]);
                    printf("■");
                    textcolor(7);
                    break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { // 게임판을 그린 후 main_cpy에 복사  
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}

void draw_main_color(void) { //게임판 그리는 함수 
    int i, j;
    gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("        ");
    for (j = 1; j < MAIN_X - 1; j++) { //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 
        if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
    }
    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.
                //이게 없으면 게임판전체를 계속 그려서 느려지고 반짝거림
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                case EMPTY: //빈칸모양 
                    printf("  ");
                    break;
                case CEILLING: //천장모양 
                    printf(". ");
                    break;
                case WALL: //벽모양 
                    printf("▩");
                    break;
                case INACTIVE_BLOCK: //굳은 블럭 모양  
                    textcolor(main_color[i][j]);
                    printf("□");
                    textcolor(7);
                    break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { //게임판을 그린 후 main_cpy에 복사  
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}