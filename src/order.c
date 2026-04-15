#include "order.h"
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int order_create(Order *order) {
    // Проверка грузоподъёмности
    if (!check_load_capacity(order->car_id, order->cargo_weight)) {
        fprintf(stderr, "Ошибка: масса груза превышает грузоподъёмность автомобиля\n");
        return 0;
    }
    const char *sql = "INSERT INTO orders (order_date, driver_id, car_id, mileage, cargo_weight, cost) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    
    sqlite3_bind_text(stmt, 1, order->order_date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, order->driver_id);
    sqlite3_bind_int(stmt, 3, order->car_id);
    sqlite3_bind_int(stmt, 4, order->mileage);
    sqlite3_bind_double(stmt, 5, order->cargo_weight);
    sqlite3_bind_double(stmt, 6, order->cost);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

Order* order_read(int id) {
    const char *sql = "SELECT id, order_date, driver_id, car_id, mileage, cargo_weight, cost FROM orders WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return NULL;
    sqlite3_bind_int(stmt, 1, id);
    Order *order = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        order = (Order*)malloc(sizeof(Order));
        order->id = sqlite3_column_int(stmt, 0);
        strcpy(order->order_date, (const char*)sqlite3_column_text(stmt, 1));
        order->driver_id = sqlite3_column_int(stmt, 2);
        order->car_id = sqlite3_column_int(stmt, 3);
        order->mileage = sqlite3_column_int(stmt, 4);
        order->cargo_weight = (float)sqlite3_column_double(stmt, 5);
        order->cost = (float)sqlite3_column_double(stmt, 6);
    }
    sqlite3_finalize(stmt);
    return order;
}

int order_update(Order *order) {
    if (!check_load_capacity(order->car_id, order->cargo_weight)) return 0;
    const char *sql = "UPDATE orders SET order_date=?, driver_id=?, car_id=?, mileage=?, cargo_weight=?, cost=? WHERE id=?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_text(stmt, 1, order->order_date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, order->driver_id);
    sqlite3_bind_int(stmt, 3, order->car_id);
    sqlite3_bind_int(stmt, 4, order->mileage);
    sqlite3_bind_double(stmt, 5, order->cargo_weight);
    sqlite3_bind_double(stmt, 6, order->cost);
    sqlite3_bind_int(stmt, 7, order->id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

int order_delete(int id) {
    const char *sql = "DELETE FROM orders WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

void order_list(void) {
    const char *sql = "SELECT id, order_date, driver_id, car_id, mileage, cargo_weight, cost FROM orders";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    printf("\n--- Список заказов ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *date = (const char*)sqlite3_column_text(stmt, 1);
        int driver = sqlite3_column_int(stmt, 2);
        int car = sqlite3_column_int(stmt, 3);
        int mileage = sqlite3_column_int(stmt, 4);
        float weight = (float)sqlite3_column_double(stmt, 5);
        float cost = (float)sqlite3_column_double(stmt, 6);
        printf("%d. %s | Вод.%d | Авто.%d | %d км | %.1f т | %.2f руб\n", id, date, driver, car, mileage, weight, cost);
    }
    sqlite3_finalize(stmt);
}

int check_load_capacity(int car_id, float cargo_weight) {
    const char *sql = "SELECT load_capacity FROM cars WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_int(stmt, 1, car_id);
    int ok = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        float capacity = (float)sqlite3_column_double(stmt, 0);
        if (cargo_weight <= capacity) ok = 1;
    }
    sqlite3_finalize(stmt);
    return ok;
}

float calculate_all_earnings(const char *start, const char *end) {
    const char *sql = "SELECT SUM(cost) * 0.2 FROM orders WHERE order_date BETWEEN ? AND ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_text(stmt, 1, start, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, end, -1, SQLITE_STATIC);
    float total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = (float)sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return total;
}

float calculate_driver_earnings(const char *start, const char *end, const char *last_name) {
    const char *sql = "SELECT SUM(o.cost) * 0.2 FROM orders o JOIN drivers d ON o.driver_id = d.id WHERE d.last_name = ? AND o.order_date BETWEEN ? AND ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_text(stmt, 1, last_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, start, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, end, -1, SQLITE_STATIC);
    float total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = (float)sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return total;
}