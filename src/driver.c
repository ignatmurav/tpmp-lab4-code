#include "driver.h"
#include <stdio.h>

int driver_create(Driver* driver) {
    printf("Driver created\n");
    return 1;
}

Driver* driver_read(int id) {
    printf("Driver read\n");
    return NULL;
}

int driver_update(Driver* driver) {
    printf("Driver updated\n");
    return 1;
}

int driver_delete(int id) {
    printf("Driver deleted\n");
    return 1;
}

void driver_list(void) {
    printf("Listing drivers\n");
}