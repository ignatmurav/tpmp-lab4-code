#ifndef AUTH_H
#define AUTH_H

typedef struct {
    int id;
    char username[50];
    char role[20];
    int driver_id;
} User;

int login(const char *username, const char *password);
void logout(void);
User* get_current_user(void);

#endif