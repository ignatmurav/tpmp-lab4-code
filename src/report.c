#include "report.h"
#include "db.h"
#include <stdio.h>

void report_by_driver(const char *last_name, const char *start, const char *end) {
    const char *sql = "SELECT o.order_date, o.mileage, o.cargo_weight, o.cost FROM orders o JOIN drivers d ON o.driver_id = d.id WHERE d.last_name = ? AND o.order_date BETWEEN ? AND ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    sqlite3_bind_text(stmt, 1, last_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, start, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, end, -1, SQLITE_STATIC);
    
    printf("\n--- Заказы водителя %s за период %s - %s ---\n", last_name, start, end);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *date = (const char*)sqlite3_column_text(stmt, 0);
        int mileage = sqlite3_column_int(stmt, 1);
        float weight = (float)sqlite3_column_double(stmt, 2);
        float cost = (float)sqlite3_column_double(stmt, 3);
        printf("%s | %d км | %.1f т | %.2f руб\n", date, mileage, weight, cost);
    }
    sqlite3_finalize(stmt);
}

void report_by_car(const char *plate_number) {
    const char *sql = "SELECT SUM(o.mileage), SUM(o.cargo_weight) FROM orders o JOIN cars c ON o.car_id = c.id WHERE c.plate_number = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    sqlite3_bind_text(stmt, 1, plate_number, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int total_mileage = sqlite3_column_int(stmt, 0);
        float total_weight = (float)sqlite3_column_double(stmt, 1);
        printf("\n--- Машина %s: общий пробег %d км, перевезено %.1f т груза ---\n", plate_number, total_mileage, total_weight);
    }
    sqlite3_finalize(stmt);
}

void report_all_drivers_stats(void) {
    const char *sql = "SELECT d.last_name, COUNT(o.id), SUM(o.cargo_weight), SUM(o.cost)*0.2 FROM drivers d LEFT JOIN orders o ON d.id = o.driver_id GROUP BY d.id";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    printf("\n--- Статистика по водителям (поездки, масса, заработано) ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *name = (const char*)sqlite3_column_text(stmt, 0);
        int trips = sqlite3_column_int(stmt, 1);
        float weight = (float)sqlite3_column_double(stmt, 2);
        float earned = (float)sqlite3_column_double(stmt, 3);
        printf("%s: поездок %d, масса %.1f т, заработано %.2f руб\n", name, trips, weight, earned);
    }
    sqlite3_finalize(stmt);
}

void report_driver_min_trips(void) {
    const char *sql = "SELECT d.id, d.last_name, COUNT(o.id) as trips FROM drivers d LEFT JOIN orders o ON d.id = o.driver_id GROUP BY d.id ORDER BY trips ASC LIMIT 1";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        int trips = sqlite3_column_int(stmt, 2);
        printf("\n--- Водитель с наименьшим числом поездок: %s (%d поездок) ---\n", name, trips);
    }
    sqlite3_finalize(stmt);
}

void report_car_max_mileage(void) {
    const char *sql = "SELECT c.plate_number, c.brand, SUM(o.mileage) as total_mileage FROM cars c LEFT JOIN orders o ON c.id = o.car_id GROUP BY c.id ORDER BY total_mileage DESC LIMIT 1";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *plate = (const char*)sqlite3_column_text(stmt, 0);
        const char *brand = (const char*)sqlite3_column_text(stmt, 1);
        int mileage = sqlite3_column_int(stmt, 2);
        printf("\n--- Автомобиль с наибольшим пробегом: %s (%s) - %d км ---\n", plate, brand, mileage);
    }
    sqlite3_finalize(stmt);
}