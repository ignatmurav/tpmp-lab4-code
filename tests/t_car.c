#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "car.h"
#include "db.h"

void setup_car_test() {
    db_connect("test.db");
    db_execute("DROP TABLE IF EXISTS cars");
    db_execute("CREATE TABLE cars ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "plate_number TEXT NOT NULL UNIQUE,"
               "brand TEXT NOT NULL,"
               "initial_mileage INTEGER NOT NULL,"
               "load_capacity REAL NOT NULL)");
}

void test_car_create() {
    setup_car_test();
    Car c = {0};
    strcpy(c.plate_number, "A001AA");
    strcpy(c.brand, "TestCar");
    c.initial_mileage = 1000;
    c.load_capacity = 5.5;
    int rc = car_create(&c);
    assert(rc == 1);
    assert(c.id > 0);
}

void test_car_read() {
    setup_car_test();
    Car c = {0};
    strcpy(c.plate_number, "A002BB");
    strcpy(c.brand, "ReadCar");
    c.initial_mileage = 2000;
    c.load_capacity = 8.0;
    car_create(&c);
    Car *read = car_read(c.id);
    assert(read != NULL);
    assert(strcmp(read->brand, "ReadCar") == 0);
    free(read);
}

void test_car_update() {
    setup_car_test();
    Car c = {0};
    strcpy(c.plate_number, "A003CC");
    strcpy(c.brand, "Old");
    car_create(&c);
    strcpy(c.brand, "New");
    int up = car_update(&c);
    assert(up == 1);
    Car *updated = car_read(c.id);
    assert(strcmp(updated->brand, "New") == 0);
    free(updated);
}

void test_car_delete() {
    setup_car_test();
    Car c = {0};
    strcpy(c.plate_number, "A004DD");
    car_create(&c);
    int del = car_delete(c.id);
    assert(del == 1);
    Car *gone = car_read(c.id);
    assert(gone == NULL);
}

void test_car_list() {
    setup_car_test();
    Car c1 = {0}, c2 = {0};
    strcpy(c1.plate_number, "L1"); strcpy(c1.brand, "List1");
    strcpy(c2.plate_number, "L2"); strcpy(c2.brand, "List2");
    car_create(&c1);
    car_create(&c2);
    car_list(); // просто не падает
}

int main() {
    test_car_create();
    test_car_read();
    test_car_update();
    test_car_delete();
    test_car_list();
    db_disconnect();
    printf("All car tests passed!\n");
    return 0;
}