#include <windows.h>
#include <mmsystem.h>
#include "Game.h"
#include "UserManager.h"
#include "FileManager.h"
#include "WindowManager.h"
#include "Windows.h"

#pragma comment(lib, "winmm.lib")

int STATUS_Y_GOAL = 0; //GOAL ����ǥ����ġY ��ǥ ���� 
int STATUS_Y_LEVEL = 0; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE = 0; //SCORE ����ǥ����ġY ��ǥ ����

int main_color[MAIN_Y][MAIN_X] = { 0 };
int main_org[MAIN_Y][MAIN_X] = { 0 };
int main_cpy[MAIN_Y][MAIN_X] = { 0 };
int bx = 0;
int by = 0; //�̵����� ����� �����ǻ��� x,y��ǥ�� ���� 
int key = 0; //Ű����� �Է¹��� Ű���� ���� 
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
}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� 
int last_score = 0;
int new_block_on = false; //���ο� ���� �ʿ����� �˸��� flag 
int crush_on = 0; //���� �̵����� ����� �浹�������� �˷��ִ� flag 
int space_key_on = 0; //hard drop�������� �˷��ִ� flag 
int random_color[4]; // ������ ���� ����
int combo_cnt = 0; // �� �� �̻� �Ѳ����� ���ָ� �޺�

int level_up_on = 0; //���������� ����(���� ������ǥ�� �Ϸ�Ǿ�����) �˸��� flag 
int turn_2p = 0; // 2��° �÷��̾� ��������

void reset(void) {

    level = 1; //�������� �ʱ�ȭ 
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

    system("cls"); //ȭ������ 
    reset_main(); // main_org�� �ʱ�ȭ 
    draw_map(); // ����ȭ���� �׸�
    draw_main(); // �������� �׸� 

    b_type_next = rand() % 7; //�������� ���� ��� ������ �����ϰ� ���� 
    new_block(); //���ο� ����� �ϳ� ����  


    while (true) {
        for (int i = 0; i < 5; i++) { //����� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
            check_key(); //Ű�Է�Ȯ�� 
            if (combo_cnt == 1) {
                draw_main_color();
                combo_cnt = 0;
            }
            else
                draw_main(); //ȭ���� �׸� 
            Sleep(speed); //���Ӽӵ����� 
            if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
            //����� �浹���ΰ�� �߰��� �̵��� ȸ���� �ð��� ���� 
            if (space_key_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
                space_key_on = 0;
                break;
            }
        }
        drop_block(); // ����� ��ĭ ���� 
        check_level_up(); // �������� üũ 
        check_game_over(); //���ӿ����� üũ 
        if (new_block_on == true) new_block(); // �� �� flag�� �ִ� ��� ���ο� �� ���� 
    }
}

void reset_main(void) {
    int i, j;

    for (i = 0; i < MAIN_Y; i++) { // �������� 0���� �ʱ�ȭ  
        for (j = 0; j < MAIN_X; j++) {
            main_org[i][j] = 0;
            main_cpy[i][j] = 100;
        }
    }

    // õ��
    for (i = 1; i < MAIN_Y - 1; i++) {
        main_org[i][0] = CEILLING; // y���� 0�� ��ġ�� õ���� ���� (���� ������ ����)
    }

    // ��, �Ʒ� ���� ����
    for (j = 0; j < MAIN_X; j++) {
        main_org[0][j] = WALL; // ���� ��
        main_org[MAIN_Y - 1][j] = WALL; // �Ʒ��� ��
    }

    // �ٴ�
    for (i = 0; i < MAIN_Y; i++) {
        main_org[i][MAIN_X - 1] = WALL; // ������ ������ ����
    }
}

void reset_main_cpy(void) { //main_cpy�� �ʱ�ȭ 
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {         //�����ǿ� ���ӿ� ������ �ʴ� ���ڸ� ���� 
        for (j = 0; j < MAIN_X; j++) {  //�̴� main_org�� ���� ���ڰ� ���� �ϱ� ���� 
            main_cpy[i][j] = 100;
        }
    }
}

void new_block(void) { //���ο� ��� ����  
    int i, j;

    bx = 1; //��� ���� ��ġx��ǥf�� �������� ���� �����ڸ��� ����
    by = 5;  //��� ������ġ y��ǥ(���� ��) 
    b_type = b_type_next; // ���� Ÿ�� ���� ���� ��� Ÿ������ ����
    b_type_next = rand() % 7; //���� �� Ÿ���� �����ϰ� ���
    b_rotation = 0;  // �ʱ� ȸ�� ���� == ���� 0 

    new_block_on = false; //new_block flag�� ��  

    for (i = 0; i < 4; i++) { //������ bx, by��ġ�� ������  
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) {
                main_org[by + i][bx + j] = ACTIVE_BLOCK;
                random_color[3] = rand() % 15 + 1;
            }
        }
    }
    for (i = 1; i < 3; i++) { //���ӻ���ǥ�ÿ� ������ ���ú��� �׸� 
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
                textcolor(random_color[3]);
                printf("��");
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
    key = 0; // Ű�� �ʱ�ȭ  

    if (_kbhit()) { // Ű�Է��� �ִ� ���  
        key = _getch(); // Ű���� ����
        if (key == 224) { // ����Ű�� ��� 
            do { key = _getch(); } while (key == 224); // ����Ű ���ð��� ���� 
            switch (key) {
            case UP: // ����Ű ������ �� ���� �̵�
                if (check_crush(bx, by, b_rotation % 4) == true) move_block(UP); // ���� �̵�
                break;
            case RIGHT: // ������ Ű�� ������ �� ���������� �̵�
                if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT); // ���������� �̵�
                break;
            case DOWN: // �Ʒ��� Ű�� ������ �� �Ʒ��� �̵�
                if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); // �Ʒ��� �̵�
                break;
            case LEFT: // ���� Ű ������ �� ȸ��
                if (check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100); // ȸ�� ���� (Ư�� ����)
                break;
            }
        }
        else { // ����Ű�� �ƴ� ��� 
            switch (key) {
            case SPACE: // �����̽� Ű�� ������ �� 
                space_key_on = 1; // �����̽� Ű �÷��׸� ��� 
                while (crush_on == 0) { // �ٴڿ� ���� ������ �̵���Ŵ 
                    drop_block();
                    score += level; // hard drop ���ʽ�
                    gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); // ���� ǥ��  
                }
                break;
            case P: // P(�빮��) ������ �� 
            case p: // p(�ҹ���) ������ �� 
                pause(); // �Ͻ����� 
                break;
            case ESC: // ESC ������ �� 
                system("cls"); // ȭ���� ����� 
                exit(0); // ���� ���� 
            }
        }
    }
    while (_kbhit()) _getch(); // Ű���۸� ��� 
}

void drop_block(void) {
    int i, j;

    // ����� ������ ���� ��Ҵ��� Ȯ��
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
        check_line(); // ���� üũ
        new_block_on = 1; //���ο� ������ flag�� ��  
        check_game_over(); // ���� ���� �˻�
        return;
    }

    // ����� �Ʒ��� ��Ҵ��� Ȯ��
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
        check_game_over(); // ���� ���� �˻�
        return;
    }

    if (check_crush(bx + 1, by, b_rotation) == true) {
        move_block(RIGHT); // ����� ���������� �̵�
    }
    if (check_crush(bx + 1, by, b_rotation) == false) {
        crush_on++; //������ �̵��� �ȵǸ�  crush flag�� ��
        random_color[0] = random_color[1];
    }
}

int check_crush(int bx, int by, int b_rotation) {
    int i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) {
                // ���̳� �ٸ� ������ ��ϰ� �浹�ϴ��� Ȯ��
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
    case LEFT: // ���ڸ� ȸ��
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
                        if (by + i >= 0) { // ���� ���� �Ѿ�� �ʵ���
                            main_org[by + i][bx + j] = ACTIVE_BLOCK;
                        }
                    }
                }
            }
        }
        break;

    case RIGHT: // ������ ����
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

    case DOWN: // �Ʒ� ����
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

    case UP: // ���� ����
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

    case 100: // ����� �ٴ�, Ȥ�� �ٸ� ��ϰ� ���� ���¿��� �� ĭ ���� �÷� ȸ���� ������ ��� 
        // �̸� ���۽�Ű�� Ư������ 
        if (check_crush(bx + 1, by, (b_rotation + 1) % 4)) { // ȸ�� ��Ų ��� �ҷ��ͼ� ȸ�� ���� ���� Ȯ��
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = EMPTY;
                    }
                }
            }
            b_rotation = (b_rotation + 1) % 4; // ȸ��
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (blocks[b_type][b_rotation][i][j] == 1) {
                        main_org[by + i][bx + j] = ACTIVE_BLOCK; // �� ĭ ȸ���� b_lotation �ݿ�
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

    for (j = 1; j < MAIN_X - 1; j++) { // �� ���� �˻�
        int filled = 1; // ���� �� ���� Ȯ���ϱ� ���� �÷���

        for (i = 1; i < MAIN_Y - 1; i++) { // ���� �Ʒ� ���� ���̹Ƿ� ����
            if (main_org[i][j] == EMPTY) { // ��ĭ�� ������
                filled = 0; // ���� �� ���� �ƴ�
                break;
            }
        }

        if (filled) { // �� ���� ���� �� ���
            lines_cleared++; // ���� �� �� ����
            for (k = j; k > 1; k--) { // ���� ������ �������� ��� ���� �� ĭ�� �̵�
                for (i = 1; i < MAIN_Y - 1; i++) {
                    main_org[i][k] = main_org[i][k - 1];
                    main_color[i][k] = main_color[k - 1][i];
                }
            }
            for (i = 1; i < MAIN_Y - 1; i++) { // �� ���� ���� ��ĭ���� ����
                main_org[i][1] = EMPTY;
                main_color[k][1] = 7;

            }
            j--; // ���� ���� �ٽ� �˻� (�� ���� ���������Ƿ�)
        }
    }

    // ������ ���� ��ǥ ������Ʈ
    if (lines_cleared > 0) {
        score += lines_cleared * 100; // ���� �� ���� ���� ���� �߰�
        cnt += lines_cleared; // ���� �������� ���� �� �� ����
        Sleep(500);
        reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� main_cpy�� �ʱ�ȭ.
        // ���� ��ǥ �޼� ���� Ȯ��
        if (cnt >= level_goal) {
            level++; // ���� ��
            cnt = 0; // ���� �� �� �ʱ�ȭ
            level_goal += 10; // ���� ���� ��ǥ ����
            speed = (speed > 1) ? speed - 1 : 1; // �ӵ� ���� (�ӵ� ����)

            // ���� �� �޽��� ���
            gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level);
            //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", levekl_goal);
            gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
        }
        else {
            // ������ ���� ��ǥ ǥ�� ������Ʈ
            gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
            //gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", level_goal - cnt);
        }
    }
}

void check_level_up(void) {
    int i, j;

    if (cnt >= 1) { //�������� 2�پ� ���־���.
        draw_main();
        level_up_on = 1; //������ flag�� ��� 
        level += 1; //������ 1 �ø� 
        cnt = 0; //���� �ټ� �ʱ�ȭ   

        for (i = 0; i < 2; i++) {
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("             ");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("             ");
            Sleep(200);

            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
            printf("��LEVEL UP!��");
            gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
            printf("��SPEED UP!��");
            Sleep(200);
        }
        reset_main_cpy(); //�ؽ�Ʈ�� ����� ���� main_cpy�� �ʱ�ȭ.
        //(main_cpy�� main_org�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ������ ��ü�� ���� �׸��� ��) 

        // ������ �������� �� ����-1�� ����ŭ �Ʒ��� ���� ������
        for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) {
            for (j = 5; j < MAIN_X - 1; j++) {

                main_org[i][j] = INACTIVE_BLOCK; // ���� ������� ��� ä��� 
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // ���� �����.. �̻����̰� 
                printf("��");

                Sleep(10); // Sleep �ð��� ����
            }
        }
        Sleep(100); //�������� �����ֱ� ���� delay 
        check_line(); //������� ��� ä��� �����
        //.check_line()�Լ� ���ο��� level up flag�� �����ִ� ��� ������ ����.         
        switch (level) { //�������� �ӵ��� ��������. 
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
        level_up_on = 0; //������ flag����

        gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //����ǥ�� 
        gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // ������ǥ ǥ�� 

    }
}

void check_game_over(void) {
    int i;
    int x = 5;
    int y = 5;

    // 1p ���
    if (!flg_2p) {
        // ���� ù ��° ���� ����� �׿� �ִ��� Ȯ��
        for (i = 0; i < MAIN_Y - 1; i++) {
            if (main_org[i][1] == INACTIVE_BLOCK) { // ���� ù ��° ���� ������ ����� �ִ��� Ȯ��
                gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�"); // ���� ���� �޽���
                gotoxy(x, y + 1); printf("��                              ��");
                gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
                gotoxy(x, y + 3); printf("��  |  G A M E  O V E R..   |   ��");
                gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
                gotoxy(x, y + 5); printf("��   YOUR SCORE: %6d         ��", score);
                gotoxy(x, y + 6); printf("��                              ��");
                gotoxy(x, y + 7); printf("��  Press any key to restart..  ��");
                gotoxy(x, y + 8); printf("��                              ��");
                gotoxy(x, y + 9); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
                last_score = score; // ���� ������ ����

                add_userInfo(current_user_1, score); // ��ŷ ������Ʈ (���Ͽ� ����)

                if (score > best_score) { //�ְ��� ���Ž�                     
                    gotoxy(x, y + 6); printf("��  �ڡڡ� BEST SCORE! �ڡڡ�   ��  ");
                }
                Sleep(1000);
                while (_kbhit()) _getch();
                key = _getch();
                reset();
            }
        }
    }

    // 2p ���
    else {
        if (!turn_2p) {
            for (i = 0; i < MAIN_Y - 1; i++) {
                if (main_org[i][1] == INACTIVE_BLOCK) { // ���� ù ��° ���� ������ ����� �ִ��� Ȯ��
                    gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�"); // ���� ���� �޽���
                    gotoxy(x, y + 1); printf("��                                       ��");
                    gotoxy(x, y + 2); printf("��       +-----------------------+       ��");
                    gotoxy(x, y + 3); printf("��       |  G A M E  O V E R..   |       ��");
                    gotoxy(x, y + 4); printf("��       +-----------------------+       ��");
                    gotoxy(x, y + 5); printf("��        YOUR SCORE: %6d             ��", score);
                    gotoxy(x, y + 6); printf("��                                       ��");
                    gotoxy(x, y + 7); printf("��  Press any key for 2.Player to start  ��");
                    gotoxy(x, y + 8); printf("��                                       ��");
                    gotoxy(x, y + 9); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
                    last_score = score; // ���� ������ ����

                    add_userInfo(current_user_1, score); // ��ŷ ������Ʈ (���Ͽ� ����)
                    if (score > best_score) { //�ְ��� ���Ž�                     
                        gotoxy(x, y + 6); printf("��  �ڡڡ� BEST SCORE! �ڡڡ�   ��  ");
                    }
                    Sleep(1000);
                    while (_kbhit()) _getch();
                    key = _getch();
                    // �÷��̾� 2����
                    turn_2p = 1;
                    reset();
                }
            }
        }
        // turn_2p
        else {
            for (i = 0; i < MAIN_Y - 1; i++) {
                if (main_org[i][1] == INACTIVE_BLOCK) { // ���� ù ��° ���� ������ ����� �ִ��� Ȯ��
                    gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�"); // ���� ���� �޽���
                    gotoxy(x, y + 1); printf("��                                      ��");
                    gotoxy(x, y + 2); printf("��       +-----------------------+      ��");
                    gotoxy(x, y + 3); printf("��       |  G A M E  O V E R..   |      ��");
                    gotoxy(x, y + 4); printf("��       +-----------------------+      ��");
                    gotoxy(x, y + 5); printf("��        YOUR SCORE: %6d            ��", score);
                    gotoxy(x, y + 6); printf("��                                      ��");
                    gotoxy(x, y + 7); printf("��   Press any key to view the result   ��");
                    gotoxy(x, y + 8); printf("��                                      ��");
                    gotoxy(x, y + 9); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
                    last_score = score; // ���� ������ ����

                    add_userInfo(current_user_2, score); // ��ŷ ������Ʈ (���Ͽ� ����)
                    if (score > best_score) { //�ְ��� ���Ž�                     
                        gotoxy(x, y + 6); printf("��  �ڡڡ� BEST SCORE! �ڡڡ�   ��  ");
                    }
                    Sleep(1000);
                    while (_kbhit()) _getch();
                    key = _getch();
                    // �÷��̾� ���� reset
                    flg_2p = 0;
                    // 1p, 2p ���� �� ȭ�� (���� �̰����)
                    result_2p_window();
                }
            }
        }
    }
}

void pause(void) { //���� �Ͻ����� �Լ� 
    int i, j;

    int x = 5;
    int y = 5;

    for (i = 1; i < MAIN_X - 2; i++) { //���� �Ͻ����� �޼��� 
        gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
        gotoxy(x, y + 1); printf("��                              ��");
        gotoxy(x, y + 2); printf("��  +-----------------------+   ��");
        gotoxy(x, y + 3); printf("��  |       P A U S E       |   ��");
        gotoxy(x, y + 4); printf("��  +-----------------------+   ��");
        gotoxy(x, y + 5); printf("��  Press any key to resume..   ��");
        gotoxy(x, y + 6); printf("��                              ��");
        gotoxy(x, y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
    }
    _getch(); //Ű�Է½ñ��� ��� 

    system("cls"); //ȭ�� ����� ���� �׸� 
    reset_main_cpy();
    draw_main();
    draw_map();

    for (i = 1; i < 3; i++) { // ������� �׸� 
        for (j = 0; j < 4; j++) {
            if (blocks[b_type_next][0][i][j] == 1) {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("��");
            }
            else {
                gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
                printf("  ");
            }
        }
    }
} //��! 

void play_music() {
    char c[256] = "";
    PlaySound(NULL, 0, 0);
    Sleep(10); // 0.01�� ���				
    PlaySound(TEXT("tetris-bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void stop_music() {
    PlaySound(NULL, 0, 0);
}

// game-window
void draw_map(void) { //���� ���� ǥ�ø� ��Ÿ���� �Լ�  
    int y = 3;             // level, goal, score�� �����߿� ���� �ٲ�� �� ���� �� y���� ���� �����ص� 
    // �׷��� Ȥ�� ���� ���� ǥ�� ��ġ�� �ٲ� �� �Լ����� �ȹٲ㵵 �ǰ�.. 
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
    gotoxy(STATUS_X_ADJ, y + 15); printf("  ��   : Up                  SPACE : Hard Drop");
    gotoxy(STATUS_X_ADJ, y + 16); printf("��   �� : Shift / Soft Drop   P   : Pause");
    gotoxy(STATUS_X_ADJ, y + 17); printf("  ��   : Down                ESC  : Quit");
}

void draw_main(void) { // ������ �׸��� �Լ� 
    int i, j;

    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { // cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���
                // �̰� ������ ������ ��ü�� ��� �׷��� �������� ��¦�Ÿ�
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                case EMPTY: // ��ĭ ��� 
                    printf("  ");
                    break;
                case CEILLING: // õ�� ��� 
                    printf(". ");
                    break;
                case WALL: // �� ��� 
                    printf("��");
                    break;
                case INACTIVE_BLOCK: // ���� �� ���  
                    textcolor(random_color[0]);
                    printf("��");
                    textcolor(7);
                    break;
                case ACTIVE_BLOCK: // �����̰� �ִ� �� ���  
                    textcolor(random_color[2]);
                    printf("��");
                    textcolor(7);
                    break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { // �������� �׸� �� main_cpy�� ����  
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}

void draw_main_color(void) { //������ �׸��� �Լ� 
    int i, j;
    gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("        ");
    for (j = 1; j < MAIN_X - 1; j++) { //õ���� ��� ���ο���� �������� �������� ���� �׷��� 
        if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
    }
    for (i = 0; i < MAIN_Y; i++) {
        for (j = 0; j < MAIN_X; j++) {
            if (main_cpy[i][j] != main_org[i][j]) { //cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���.
                //�̰� ������ ��������ü�� ��� �׷��� �������� ��¦�Ÿ�
                gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
                switch (main_org[i][j]) {
                case EMPTY: //��ĭ��� 
                    printf("  ");
                    break;
                case CEILLING: //õ���� 
                    printf(". ");
                    break;
                case WALL: //����� 
                    printf("��");
                    break;
                case INACTIVE_BLOCK: //���� �� ���  
                    textcolor(main_color[i][j]);
                    printf("��");
                    textcolor(7);
                    break;
                }
            }
        }
    }
    for (i = 0; i < MAIN_Y; i++) { //�������� �׸� �� main_cpy�� ����  
        for (j = 0; j < MAIN_X; j++) {
            main_cpy[i][j] = main_org[i][j];
        }
    }
}