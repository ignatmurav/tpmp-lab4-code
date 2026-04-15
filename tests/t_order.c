#include <stddef.h>
#include <assert.h>
#include <string.h>
#include "order.h"
#include "car.h"
#include "driver.h"
#include "db.h"

void setup_order_test() {
    db_connect("test.db");
    db_execute("DROP TABLE IF EXISTS orders");
    db_execute("DROP TABLE IF EXISTS cars");
    db_execute("DROP TABLE IF EXISTS drivers");
    db_execute("CREATE TABLE cars (id INTEGER PRIMARY KEY AUTOINCREMENT, plate_number TEXT, brand TEXT, initial_mileage INTEGER, load_capacity REAL)");
    db_execute("CREATE TABLE drivers (id INTEGER PRIMARY KEY AUTOINCREMENT, personnel_number TEXT, last_name TEXT, category TEXT, experience INTEGER, birth_year INTEGER, address TEXT)");
    db_execute("CREATE TABLE orders (id INTEGER PRIMARY KEY AUTOINCREMENT, order_date TEXT, driver_id INTEGER, car_id INTEGER, mileage INTEGER, cargo_weight REAL, cost REAL, FOREIGN KEY(driver_id) REFERENCES drivers(id), FOREIGN KEY(car_id) REFERENCES cars(id))");
    // Добавляем тестовые автомобиль и водителя
    db_execute("INSERT INTO cars (id, plate_number, brand, initial_mileage, load_capacity) VALUES (1, 'CAR1', 'TestCar', 0, 10.0)");
    db_execute("INSERT INTO drivers (id, personnel_number, last_name) VALUES (1, 'DRV1', 'OrderTest')");
}

void test_order_create_ok() {
    setup_order_test();
    Order o = {0};
    strcpy(o.order_date, "2025-01-01");
    o.driver_id = 1;
    o.car_id = 1;
    o.mileage = 100;
    o.cargo_weight = 5.0;
    o.cost = 5000;
    int rc = order_create(&o);
    assert(rc == 1);
    assert(o.id > 0);
}


void test_order_create_fail_load_capacity() {
    setup_order_test();
    Order o = {0};
    strcpy(o.order_date, "2025-01-02");
    o.driver_id = 1;
    o.car_id = 1;
    o.cargo_weight = 15.0; // превышает грузоподъемность 10
    o.cost = 5000;
    int rc = order_create(&o);
    assert(rc == 0);
}

void test_order_read() {
    setup_order_test();
    Order o = {0};
    strcpy(o.order_date, "2025-01-03");
    o.driver_id = 1;
    o.car_id = 1;
    o.mileage = 50;
    o.cargo_weight = 3.0;
    o.cost = 3000;
    order_create(&o);
    Order *read = order_read(o.id);
    assert(read != NULL);
    assert(read->mileage == 50);
    free(read);
}

void test_order_update() {
    setup_order_test();
    Order o = {0};
    strcpy(o.order_date, "2025-01-04");
    o.driver_id = 1;
    o.car_id = 1;
    order_create(&o);
    o.mileage = 200;
    int up = order_update(&o);
    assert(up == 1);
    Order *updated = order_read(o.id);
    assert(updated->mileage == 200);
    free(updated);
}

void test_order_delete() {
    setup_order_test();
    Order o = {0};
    strcpy(o.order_date, "2025-01-05");
    o.driver_id = 1;
    o.car_id = 1;
    order_create(&o);
    int del = order_delete(o.id);
    assert(del == 1);
    Order *gone = order_read(o.id);
    assert(gone == NULL);
}

void test_check_load_capacity() {
    setup_order_test();
    int ok = check_load_capacity(1, 9.0);
    assert(ok == 1);
    int fail = check_load_capacity(1, 12.0);
    assert(fail == 0);
}

void test_calculate_all_earnings() {
    setup_order_test();
    // Добавим несколько заказов
    db_execute("DELETE FROM orders");
    db_execute("INSERT INTO orders (order_date, driver_id, car_id, mileage, cargo_weight, cost) VALUES "
               "('2025-01-01',1,1,10,1,1000),"
               "('2025-01-02',1,1,20,2,2000)");
    float total = calculate_all_earnings("2025-01-01", "2025-01-31");
    // 20% от (1000+2000)=600
    assert(total == 600.0);
}

void test_calculate_driver_earnings() {
    setup_order_test();
    db_execute("DELETE FROM orders");
    db_execute("INSERT INTO orders (order_date, driver_id, car_id, cost) VALUES "
               "('2025-01-01',1,1,1000), ('2025-01-02',1,1,2000)");
    float earned = calculate_driver_earnings("2025-01-01", "2025-01-31", "OrderTest");
    assert(earned == 600.0);
}

int main() {
    test_order_create_ok();
    test_order_create_fail_load_capacity();
    test_order_read();
    test_order_update();
    test_order_delete();
    test_check_load_capacity();
    test_calculate_all_earnings();
    test_calculate_driver_earnings();
    db_disconnect();
    printf("All order tests passed!\n");
    return 0;
}