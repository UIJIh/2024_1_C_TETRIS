#define _CRT_SECURE_NO_WARNINGS
#include "UserManager.h"
#include "FileManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool flg_2p = false;
char* current_user_1 = NULL;
char* current_user_2 = NULL;
int existing_index = -1;

void initialize_user_info() {
    flg_2p = false;
    current_user_1 = (char*)malloc(100 * sizeof(char));
    if (current_user_1 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

void set_2P_mode() {
    flg_2p = true;
    current_user_2 = (char*)malloc(100 * sizeof(char));
    if (current_user_2 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
}

void cleanup_user_info() {
    if (current_user_1 != NULL) {
        free(current_user_1);
        current_user_1 = NULL;
    }
    if (flg_2p) {
        if (current_user_2 != NULL) {
            free(current_user_2);
            current_user_2 = NULL;
        }
    }
}

//void clear_read_buffer()
//{
//    while (getchar() != '\n');//�Է¹��۸� ����
//}

void set_current_user(char** user) {
    char buffer[MAX_NAME_LENGTH];
    printf("");
    if (fgets(buffer, sizeof(buffer), stdin)) { // ������ ���� ũ�⺸�� ũ�� �ڵ����� �߶�
        buffer[strcspn(buffer, "\n")] = 0; // remove newline character if present(fgets�� ���� ������\n)
        *user = malloc(MAX_NAME_LENGTH * sizeof(char));
        if (*user == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        strncpy(*user, buffer, MAX_NAME_LENGTH); // ª���� null�� �� ä����
        (*user)[MAX_NAME_LENGTH - 1] = '\0'; // ensure null-terminated
    }
    else {
        fprintf(stderr, "Error reading user input.\n");
        exit(1);
    }
}