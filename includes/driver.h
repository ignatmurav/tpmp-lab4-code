#ifndef DRIVER_H
#define DRIVER_H

typedef struct {
    int id;
    char personnel_number[10];
    char last_name[50];
    char category[20];
    int experience;
    int birth_year;
    char address[200];
} Driver;

int driver_create(Driver *driver);
Driver* driver_read(int id);
int driver_update(Driver *driver);
int driver_delete(int id);
void driver_list(void);

#endif