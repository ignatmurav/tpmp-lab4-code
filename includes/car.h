#ifndef CAR_H
#define CAR_H

typedef struct {
    int id;
    char plate_number[20];
    char brand[50];
    int initial_mileage;
    float load_capacity;
} Car;

int car_create(Car *car);
Car* car_read(int id);
int car_update(Car *car);
int car_delete(int id);
void car_list(void);

#endif