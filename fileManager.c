#include "FileManager.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_users = 0;
UserScore userInfo[MAX_USERS];
int best_score = 0;
char best_name[MAX_NAME_LENGTH];

// ���Ͽ��� ������ �о���� �Լ�
void read_from_file() {
    FILE* file = fopen("userScores.dat", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %d\n", userInfo[num_users].name, &userInfo[num_users].score) == 2) {
            num_users++;
            if (num_users >= MAX_USERS) break;
        }
        fclose(file);
    }
}

// ���Ͽ� ������ ���� �Լ�
void write_to_file() {
    FILE* file = fopen("userScores.dat", "w");
    if (file != NULL) {
        for (int i = 0; i < num_users; i++) {
            fprintf(file, "%s %d\n", userInfo[i].name, userInfo[i].score);
        }
        fclose(file);
    }
}

// ������ �ְ� ���� ������ �����ϴ� �Լ�
void sort_scores() {
    UserScore temp;
    for (int i = 0; i < num_users; i++) {
        for (int j = i + 1; j < num_users; j++) {
            if (userInfo[i].score < userInfo[j].score ||
                (userInfo[i].score == userInfo[j].score && strcmp(userInfo[i].name, userInfo[j].name) > 0)) {
                // ������ ������������ ����, ������ ���� ��� �̸������� ����
                temp = userInfo[i];
                userInfo[i] = userInfo[j];
                userInfo[j] = temp;
            }
        }
    }
}

// ����ڰ� �̹� �����ϴ��� Ȯ���ϴ� �Լ�
int find_user(const char* name) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(userInfo[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// ����� ���� ��� �Լ�
int get_userInfo(const char* name) {
    int existing_index = find_user(name);
    return userInfo[existing_index].score;
}

void add_userInfo(const char* name, int score) {
    // ����� �̸��� �̹� �����ϴ��� Ȯ��
    int existing_index = find_user(name);
    // �̹� �����ϴ� ��� ���� ������Ʈ
    if (existing_index != -1) {
        userInfo[existing_index].score = score;
    }
    else {
        // ���ο� ����� �߰�
        if (num_users < MAX_USERS) {
            strcpy(userInfo[num_users].name, name);
            userInfo[num_users].score = score;
            num_users++; // user �� �߰���
        }
        else { // �ִ� ����� �� �� á����, ���� ���� ������ ������ �ű⿡ ���
            strcpy(userInfo[MAX_USERS - 1].name, name);
            userInfo[MAX_USERS - 1].score = score;
        }
    }
    sort_scores(); // ���� ����
    write_to_file(); // ���Ͽ� ����

    // �ְ� ������ �ְ� ������ ����� ����� �̸� ������Ʈ
    if (num_users > 0) {
        best_score = userInfo[0].score;
        strcpy(best_name, userInfo[0].name);
    }
    else {
        best_score = 0;
        best_name[0] = '\0';
    }
}

// ��� ������ ����ϴ� �Լ�
void print_scores() {
    printf("\n\n");
    for (int i = 0; i < num_users; i++) {
        printf("\t\t%d. %s -> %d\n", i + 1, userInfo[i].name, userInfo[i].score);
    }
}
