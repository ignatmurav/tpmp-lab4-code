#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include "db.h"

void test_connect() {
    int rc = db_connect("test.db");
    assert(rc == 1);
}

void test_disconnect() {
    db_disconnect();
    // повторное подключение для проверки
    assert(db_connect("test.db") == 1);
}

void test_execute() {
    db_connect("test.db");
    int rc = db_execute("CREATE TABLE IF NOT EXISTS test_table (id INTEGER)");
    assert(rc == 1);
    rc = db_execute("DROP TABLE test_table");
    assert(rc == 1);
    db_disconnect();
}

int main() {
    test_connect();
    test_disconnect();
    test_execute();
    printf("All db tests passed!\n");
    return 0;
}