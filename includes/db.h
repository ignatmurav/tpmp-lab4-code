#ifndef DB_H
#define DB_H

#include <sqlite3.h>

extern sqlite3 *g_db;

int db_connect(const char *path);
void db_disconnect(void);
int db_execute(const char *sql);
int db_query(const char *sql, sqlite3_stmt **stmt);

#endif