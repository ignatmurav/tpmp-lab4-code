#include "auth.h"
#include "db.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static User current_user = {0};

int login(const char *username, const char *password) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT id, username, role, driver_id FROM users WHERE username = ? AND password_hash = ?";
    
    if (!db_query(sql, &stmt)) return 0;
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    
    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        current_user.id = sqlite3_column_int(stmt, 0);
        strcpy(current_user.username, (const char*)sqlite3_column_text(stmt, 1));
        strcpy(current_user.role, (const char*)sqlite3_column_text(stmt, 2));
        current_user.driver_id = sqlite3_column_int(stmt, 3);
        found = 1;
    }
    sqlite3_finalize(stmt);
    return found;
}

void logout(void) {
    memset(&current_user, 0, sizeof(current_user));
}

User* get_current_user(void) {
    if (current_user.id == 0) return NULL;
    return &current_user;
}