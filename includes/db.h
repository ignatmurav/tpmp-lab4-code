#ifndef DB_H
#define DB_H

int db_connect(const char* path);
void db_disconnect(void);
int db_execute(const char* sql);

#endif