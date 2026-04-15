#include "car.h"
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int car_create(Car *car) {
    const char *sql = "INSERT INTO cars (plate_number, brand, initial_mileage, load_capacity) VALUES (?, ?, ?, ?)";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    
    sqlite3_bind_text(stmt, 1, car->plate_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, car->brand, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, car->initial_mileage);
    sqlite3_bind_double(stmt, 4, car->load_capacity);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) return 0;
    
    // Получаем ID последней вставки
    car->id = sqlite3_last_insert_rowid(g_db);
    return 1;
}

Car* car_read(int id) {
    const char *sql = "SELECT id, plate_number, brand, initial_mileage, load_capacity FROM cars WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return NULL;
    sqlite3_bind_int(stmt, 1, id);
    Car *car = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        car = (Car*)malloc(sizeof(Car));
        car->id = sqlite3_column_int(stmt, 0);
        strcpy(car->plate_number, (const char*)sqlite3_column_text(stmt, 1));
        strcpy(car->brand, (const char*)sqlite3_column_text(stmt, 2));
        car->initial_mileage = sqlite3_column_int(stmt, 3);
        car->load_capacity = (float)sqlite3_column_double(stmt, 4);
    }
    sqlite3_finalize(stmt);
    return car;
}

int car_update(Car *car) {
    const char *sql = "UPDATE cars SET plate_number = ?, brand = ?, initial_mileage = ?, load_capacity = ? WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_text(stmt, 1, car->plate_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, car->brand, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, car->initial_mileage);
    sqlite3_bind_double(stmt, 4, car->load_capacity);
    sqlite3_bind_int(stmt, 5, car->id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

int car_delete(int id) {
    const char *sql = "DELETE FROM cars WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

void car_list(void) {
    const char *sql = "SELECT id, plate_number, brand, initial_mileage, load_capacity FROM cars";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    printf("\n--- Список автомобилей ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *plate = (const char*)sqlite3_column_text(stmt, 1);
        const char *brand = (const char*)sqlite3_column_text(stmt, 2);
        int mileage = sqlite3_column_int(stmt, 3);
        float capacity = (float)sqlite3_column_double(stmt, 4);
        printf("%d. %s | %s | Пробег: %d | Грузопод.: %.1f т\n", id, plate, brand, mileage, capacity);
    }
    sqlite3_finalize(stmt);
}