#include <stddef.h>
#include <assert.h>
#include "report.h"
#include "db.h"

void setup_report_test() {
    db_connect("test.db");
    // Создаём все нужные таблицы и заполняем тестовыми данными
    db_execute("DROP TABLE IF EXISTS orders");
    db_execute("DROP TABLE IF EXISTS cars");
    db_execute("DROP TABLE IF EXISTS drivers");
    db_execute("CREATE TABLE cars (id INTEGER PRIMARY KEY, plate_number TEXT, brand TEXT, initial_mileage INTEGER, load_capacity REAL)");
    db_execute("CREATE TABLE drivers (id INTEGER PRIMARY KEY, last_name TEXT, personnel_number TEXT)");
    db_execute("CREATE TABLE orders (id INTEGER PRIMARY KEY, order_date TEXT, driver_id INTEGER, car_id INTEGER, mileage INTEGER, cargo_weight REAL, cost REAL)");
    db_execute("INSERT INTO cars VALUES (1, 'CAR1', 'Brand1', 0, 10)");
    db_execute("INSERT INTO cars VALUES (2, 'CAR2', 'Brand2', 0, 20)");
    db_execute("INSERT INTO drivers VALUES (1, 'DriverA', 'A01')");
    db_execute("INSERT INTO drivers VALUES (2, 'DriverB', 'A02')");
    db_execute("INSERT INTO orders (order_date, driver_id, car_id, mileage, cargo_weight, cost) VALUES "
               "('2025-01-10',1,1,100,5,5000),"
               "('2025-01-15',1,1,150,6,6000),"
               "('2025-01-20',2,2,200,10,10000)");
}

void test_report_by_driver() {
    setup_report_test();
    // просто проверяем, что не падает
    report_by_driver("DriverA", "2025-01-01", "2025-01-31");
}

void test_report_by_car() {
    setup_report_test();
    report_by_car("CAR1");
}

void test_report_all_drivers_stats() {
    setup_report_test();
    report_all_drivers_stats();
}

void test_report_driver_min_trips() {
    setup_report_test();
    report_driver_min_trips();
}

void test_report_car_max_mileage() {
    setup_report_test();
    report_car_max_mileage();
}

int main() {
    test_report_by_driver();
    test_report_by_car();
    test_report_all_drivers_stats();
    test_report_driver_min_trips();
    test_report_car_max_mileage();
    db_disconnect();
    printf("All report tests passed!\n");
    return 0;
}