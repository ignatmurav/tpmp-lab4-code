#include <stdio.h>
#include <string.h>
#include "db.h"
#include "auth.h"
#include "car.h"
#include "driver.h"
#include "order.h"
#include "report.h"

void init_database() {
    db_connect("autopark.db");
    // Таблицы уже созданы, но на всякий случай можно выполнить SQL-скрипт
}

void menu_manager() {
    int choice;
    do {
        printf("\n=== МЕНЮ МЕНЕДЖЕРА ===\n");
        printf("1. Управление автомобилями (CRUD)\n");
        printf("2. Управление водителями (CRUD)\n");
        printf("3. Управление заказами (CRUD)\n");
        printf("4. Отчёт: по водителю за период\n");
        printf("5. Отчёт: по машине (пробег и масса)\n");
        printf("6. Отчёт: статистика по всем водителям\n");
        printf("7. Отчёт: водитель с минимум поездок\n");
        printf("8. Отчёт: машина с максимум пробега\n");
        printf("9. Рассчитать заработок всех водителей за период\n");
        printf("10. Рассчитать заработок указанного водителя\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                int sub;
                printf("1. Добавить 2. Список 3. Найти 4. Обновить 5. Удалить: ");
                scanf("%d", &sub);
                if (sub == 1) {
                    Car c = {0};
                    printf("Номер машины: "); scanf("%s", c.plate_number);
                    printf("Марка: "); scanf("%s", c.brand);
                    printf("Пробег: "); scanf("%d", &c.initial_mileage);
                    printf("Грузоподъемность (т): "); scanf("%f", &c.load_capacity);
                    if (car_create(&c)) printf("Добавлено\n");
                } else if (sub == 2) car_list();
                else if (sub == 3) { int id; printf("ID: "); scanf("%d", &id); Car *c = car_read(id); if(c) { printf("%s %s\n", c->plate_number, c->brand); free(c); } }
                else if (sub == 4) { /* обновление аналогично */ }
                else if (sub == 5) { int id; printf("ID: "); scanf("%d", &id); car_delete(id); }
                break;
            }
            case 2: {
                // аналогично для водителей (можно доработать)
                printf("Управление водителями (заглушка, реализуйте по аналогии с авто)\n");
                break;
            }
            case 3: {
                // управление заказами
                printf("Управление заказами (заглушка)\n");
                break;
            }
            case 4: {
                char name[50], start[11], end[11];
                printf("Фамилия водителя: "); scanf("%s", name);
                printf("Начальная дата (ГГГГ-ММ-ДД): "); scanf("%s", start);
                printf("Конечная дата: "); scanf("%s", end);
                report_by_driver(name, start, end);
                break;
            }
            case 5: {
                char plate[20];
                printf("Номер машины: "); scanf("%s", plate);
                report_by_car(plate);
                break;
            }
            case 6: report_all_drivers_stats(); break;
            case 7: report_driver_min_trips(); break;
            case 8: report_car_max_mileage(); break;
            case 9: {
                char start[11], end[11];
                printf("Период (начало конец): "); scanf("%s %s", start, end);
                float total = calculate_all_earnings(start, end);
                printf("Общий заработок всех водителей за период: %.2f руб\n", total);
                break;
            }
            case 10: {
                char name[50], start[11], end[11];
                printf("Фамилия водителя: "); scanf("%s", name);
                printf("Период (начало конец): "); scanf("%s %s", start, end);
                float earned = calculate_driver_earnings(start, end, name);
                printf("Заработок водителя %s: %.2f руб\n", name, earned);
                break;
            }
        }
    } while (choice != 0);
}

void menu_driver() {
    User *u = get_current_user();
    if (!u || u->driver_id == 0) {
        printf("Ошибка: профиль водителя не привязан\n");
        return;
    }
    int choice;
    do {
        printf("\n=== МЕНЮ ВОДИТЕЛЯ ===\n");
        printf("1. Мои заказы за период\n");
        printf("2. Данные моей машины\n");
        printf("3. Моя статистика (поездки, масса, заработок)\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);
        getchar();
        if (choice == 1) {
            char start[11], end[11];
            printf("Начальная дата: "); scanf("%s", start);
            printf("Конечная дата: "); scanf("%s", end);
            // получить фамилию водителя по driver_id
            // упростим: используем report_by_driver с last_name, но нужно получить last_name из drivers
            // для демонстрации допустим, что у нас есть глобальная переменная или функция.
            // Здесь можно реализовать прямой SQL-запрос.
            printf("Реализация: см. report_by_driver\n");
        } else if (choice == 2) {
            // получить машину водителя (из заказов или из привязки)
            printf("Реализация: выборка авто по driver_id\n");
        } else if (choice == 3) {
            printf("Реализация: статистика водителя\n");
        }
    } while (choice != 0);
}

int main() {
    init_database();
    char username[50], password[50];
    printf("=== АВТОПАРК ===\n");
    printf("Логин: "); scanf("%s", username);
    printf("Пароль: "); scanf("%s", password);
    
    if (login(username, password)) {
        User *u = get_current_user();
        printf("Добро пожаловать, %s! Роль: %s\n", u->username, u->role);
        if (strcmp(u->role, "manager") == 0)
            menu_manager();
        else
            menu_driver();
    } else {
        printf("Неверный логин или пароль\n");
    }
    db_disconnect();
    return 0;
}