#include "FileManager.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int num_users = 0;
UserScore userInfo[MAX_USERS];
int best_score = 0;
char best_name[MAX_NAME_LENGTH];

// 파일에서 점수를 읽어오는 함수
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

// 파일에 점수를 쓰는 함수
void write_to_file() {
    FILE* file = fopen("userScores.dat", "w");
    if (file != NULL) {
        for (int i = 0; i < num_users; i++) {
            fprintf(file, "%s %d\n", userInfo[i].name, userInfo[i].score);
        }
        fclose(file);
    }
}

// 점수를 최고 점수 순으로 정렬하는 함수
void sort_scores() {
    UserScore temp;
    for (int i = 0; i < num_users; i++) {
        for (int j = i + 1; j < num_users; j++) {
            if (userInfo[i].score < userInfo[j].score ||
                (userInfo[i].score == userInfo[j].score && strcmp(userInfo[i].name, userInfo[j].name) > 0)) {
                // 점수를 내림차순으로 정렬, 점수가 같을 경우 이름순으로 정렬
                temp = userInfo[i];
                userInfo[i] = userInfo[j];
                userInfo[j] = temp;
            }
        }
    }
}

// 사용자가 이미 존재하는지 확인하는 함수
int find_user(const char* name) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(userInfo[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// 사용자 점수 얻는 함수
int get_userInfo(const char* name) {
    int existing_index = find_user(name);
    return userInfo[existing_index].score;
}

void add_userInfo(const char* name, int score) {
    // 사용자 이름이 이미 존재하는지 확인
    int existing_index = find_user(name);
    // 이미 존재하는 경우 점수 업데이트
    if (existing_index != -1) {
        userInfo[existing_index].score = score;
    }
    else {
        // 새로운 사용자 추가
        if (num_users < MAX_USERS) {
            strcpy(userInfo[num_users].name, name);
            userInfo[num_users].score = score;
            num_users++; // user 수 추가됨
        }
        else { // 최대 사용자 수 다 찼으면, 점수 제일 낮은애 버리고 거기에 덮어씀
            strcpy(userInfo[MAX_USERS - 1].name, name);
            userInfo[MAX_USERS - 1].score = score;
        }
    }
    sort_scores(); // 점수 정렬
    write_to_file(); // 파일에 저장

    // 최고 점수와 최고 점수를 기록한 사용자 이름 업데이트
    if (num_users > 0) {
        best_score = userInfo[0].score;
        strcpy(best_name, userInfo[0].name);
    }
    else {
        best_score = 0;
        best_name[0] = '\0';
    }
}

// 모든 점수를 출력하는 함수
void print_scores() {
    printf("\n\n");
    for (int i = 0; i < num_users; i++) {
        printf("\t\t%d. %s -> %d\n", i + 1, userInfo[i].name, userInfo[i].score);
    }
}
