#include <stddef.h>
#include <assert.h>
#include "car.h"

void test_car_create() {
    Car c = {0};
    assert(car_create(&c) == 1);
}

void test_car_read() {
    assert(car_read(1) == NULL);
}

void test_car_delete() {
    assert(car_delete(1) == 1);
}

int main() {
    test_car_create();
    test_car_read();
    test_car_delete();
    return 0;
}