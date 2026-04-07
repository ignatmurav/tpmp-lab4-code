#include <stddef.h>
#include <assert.h>
#include "driver.h"

void test_driver_create() {
    Driver d = {0};
    assert(driver_create(&d) == 1);
}

void test_driver_read() {
    assert(driver_read(1) == NULL);
}

void test_driver_delete() {
    assert(driver_delete(1) == 1);
}

int main() {
    test_driver_create();
    test_driver_read();
    test_driver_delete();
    return 0;
}