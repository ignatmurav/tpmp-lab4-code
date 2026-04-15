#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "auth.h"
#include "db.h"

void setup_auth_test() {
    db_connect("test.db");
    db_execute("DROP TABLE IF EXISTS users");
    db_execute("CREATE TABLE users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "username TEXT NOT NULL UNIQUE,"
               "password_hash TEXT NOT NULL,"
               "role TEXT NOT NULL,"
               "driver_id INTEGER)");
    db_execute("INSERT INTO users (username, password_hash, role, driver_id) VALUES "
               "('manager', 'pass123', 'manager', NULL),"
               "('ivanov', 'pass123', 'driver', 1)");
}

void test_login_valid_manager() {
    setup_auth_test();
    int ok = login("manager", "pass123");
    assert(ok == 1);
    User *u = get_current_user();
    assert(u != NULL);
    assert(strcmp(u->role, "manager") == 0);
    logout();
}

void test_login_valid_driver() {
    setup_auth_test();
    int ok = login("ivanov", "pass123");
    assert(ok == 1);
    User *u = get_current_user();
    assert(u != NULL);
    assert(strcmp(u->role, "driver") == 0);
    assert(u->driver_id == 1);
    logout();
}

void test_login_invalid() {
    setup_auth_test();
    int ok = login("wrong", "wrong");
    assert(ok == 0);
    assert(get_current_user() == NULL);
}

int main() {
    test_login_valid_manager();
    test_login_valid_driver();
    test_login_invalid();
    db_disconnect();
    printf("All auth tests passed!\n");
    return 0;
}