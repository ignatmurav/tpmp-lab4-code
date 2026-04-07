#include "car.h"
#include <stdio.h>

int car_create(Car* car) {
    printf("Car created\n");
    return 1;
}

Car* car_read(int id) {
    printf("Car read\n");
    return NULL;
}

int car_update(Car* car) {
    printf("Car updated\n");
    return 1;
}

int car_delete(int id) {
    printf("Car deleted\n");
    return 1;
}

void car_list(void) {
    printf("Listing cars\n");
}