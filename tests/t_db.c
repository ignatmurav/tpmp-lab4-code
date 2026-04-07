#include <stddef.h>
#include <assert.h>
#include "db.h"

void test_connect() {
    assert(db_connect("test.db") == 1);
}

void test_disconnect() {
    db_disconnect();
}

void test_execute() {
    assert(db_execute("SELECT 1") == 1);
}

int main() {
    test_connect();
    test_disconnect();
    test_execute();
    return 0;
}