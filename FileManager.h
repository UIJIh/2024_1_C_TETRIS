#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#define MAX_NAME_LENGTH 21
#define MAX_USERS 20 // �ִ� ���尡���� ���� ����

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} UserScore;

extern int num_users;
extern UserScore userInfo[MAX_USERS];
extern int best_score;  
extern char best_name[MAX_NAME_LENGTH];
int existing_index;

void read_from_file();
void write_to_file();
void sort_scores();
int find_user(const char* name);
int get_userInfo(const char* name);
void add_userInfo(const char* name, int score);
void print_scores();

#endif // FILEMANAGER_H