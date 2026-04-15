#include "driver.h"
#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int driver_create(Driver *driver) {
    const char *sql = "INSERT INTO drivers (personnel_number, last_name, category, experience, birth_year, address) VALUES (?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    
    sqlite3_bind_text(stmt, 1, driver->personnel_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, driver->last_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, driver->category, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, driver->experience);
    sqlite3_bind_int(stmt, 5, driver->birth_year);
    sqlite3_bind_text(stmt, 6, driver->address, -1, SQLITE_STATIC);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) return 0;
    
    // Получаем последний ID
    driver->id = sqlite3_last_insert_rowid(g_db);
    return 1;
}

Driver* driver_read(int id) {
    const char *sql = "SELECT id, personnel_number, last_name, category, experience, birth_year, address FROM drivers WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return NULL;
    
    sqlite3_bind_int(stmt, 1, id);
    Driver *driver = NULL;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        driver = (Driver*)malloc(sizeof(Driver));
        driver->id = sqlite3_column_int(stmt, 0);
        strcpy(driver->personnel_number, (const char*)sqlite3_column_text(stmt, 1));
        strcpy(driver->last_name, (const char*)sqlite3_column_text(stmt, 2));
        strcpy(driver->category, (const char*)sqlite3_column_text(stmt, 3));
        driver->experience = sqlite3_column_int(stmt, 4);
        driver->birth_year = sqlite3_column_int(stmt, 5);
        strcpy(driver->address, (const char*)sqlite3_column_text(stmt, 6));
    }
    sqlite3_finalize(stmt);
    return driver;
}

int driver_update(Driver *driver) {
    const char *sql = "UPDATE drivers SET personnel_number = ?, last_name = ?, category = ?, experience = ?, birth_year = ?, address = ? WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    
    sqlite3_bind_text(stmt, 1, driver->personnel_number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, driver->last_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, driver->category, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, driver->experience);
    sqlite3_bind_int(stmt, 5, driver->birth_year);
    sqlite3_bind_text(stmt, 6, driver->address, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, driver->id);
    
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

int driver_delete(int id) {
    const char *sql = "DELETE FROM drivers WHERE id = ?";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return 0;
    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE);
}

void driver_list(void) {
    const char *sql = "SELECT id, personnel_number, last_name, category, experience, birth_year, address FROM drivers";
    sqlite3_stmt *stmt = NULL;
    if (!db_query(sql, &stmt)) return;
    
    printf("\n--- Список водителей ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *pn = (const char*)sqlite3_column_text(stmt, 1);
        const char *name = (const char*)sqlite3_column_text(stmt, 2);
        const char *cat = (const char*)sqlite3_column_text(stmt, 3);
        int exp = sqlite3_column_int(stmt, 4);
        int year = sqlite3_column_int(stmt, 5);
        const char *addr = (const char*)sqlite3_column_text(stmt, 6);
        printf("%d. %s | %s | %s | стаж %d | %d г.р. | %s\n", id, pn, name, cat, exp, year, addr);
    }
    sqlite3_finalize(stmt);
}