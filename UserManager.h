#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <stdbool.h>


extern bool flg_2p;
extern char* current_user_1;
extern char* current_user_2;

void initialize_user_info();
void set_2P_mode();
void set_current_user(char** user);
void cleanup_user_info();
//void clear_read_buffer();

#endif