#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "driver.h"
#include "db.h"

void setup_driver_test() {
    db_connect("test.db");
    db_execute("DROP TABLE IF EXISTS drivers");
    db_execute("CREATE TABLE drivers ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "personnel_number TEXT NOT NULL UNIQUE,"
               "last_name TEXT NOT NULL,"
               "category TEXT NOT NULL,"
               "experience INTEGER NOT NULL,"
               "birth_year INTEGER NOT NULL,"
               "address TEXT NOT NULL)");
}

void test_driver_create() {
    setup_driver_test();
    Driver d = {0};
    strcpy(d.personnel_number, "D001");
    strcpy(d.last_name, "Testov");
    strcpy(d.category, "C");
    d.experience = 5;
    d.birth_year = 1990;
    strcpy(d.address, "Street 1");
    int rc = driver_create(&d);
    assert(rc == 1);
    assert(d.id > 0);
}

void test_driver_read() {
    setup_driver_test();
    Driver d = {0};
    strcpy(d.personnel_number, "D002");
    strcpy(d.last_name, "Readov");
    driver_create(&d);
    Driver *read = driver_read(d.id);
    assert(read != NULL);
    assert(strcmp(read->last_name, "Readov") == 0);
    free(read);
}

void test_driver_update() {
    setup_driver_test();
    Driver d = {0};
    strcpy(d.personnel_number, "D003");
    strcpy(d.last_name, "Old");
    driver_create(&d);
    strcpy(d.last_name, "New");
    int up = driver_update(&d);
    assert(up == 1);
    Driver *updated = driver_read(d.id);
    assert(strcmp(updated->last_name, "New") == 0);
    free(updated);
}

void test_driver_delete() {
    setup_driver_test();
    Driver d = {0};
    strcpy(d.personnel_number, "D004");
    driver_create(&d);
    int del = driver_delete(d.id);
    assert(del == 1);
    Driver *gone = driver_read(d.id);
    assert(gone == NULL);
}

void test_driver_list() {
    setup_driver_test();
    Driver d1 = {0}, d2 = {0};
    strcpy(d1.personnel_number, "L1"); strcpy(d1.last_name, "List1");
    strcpy(d2.personnel_number, "L2"); strcpy(d2.last_name, "List2");
    driver_create(&d1);
    driver_create(&d2);
    driver_list();
}

int main() {
    test_driver_create();
    test_driver_read();
    test_driver_update();
    test_driver_delete();
    test_driver_list();
    db_disconnect();
    printf("All driver tests passed!\n");
    return 0;
}