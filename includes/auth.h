#ifndef AUTH_H
#define AUTH_H

int login(const char* username, const char* password);
int logout(void);
int get_current_user_id(void);

#endif