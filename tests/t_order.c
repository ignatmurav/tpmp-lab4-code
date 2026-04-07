#include <stddef.h>
#include <assert.h>
#include "order.h"

void test_order_create() {
    Order o = {0};
    assert(order_create(&o) == 1);
}

void test_check_load_capacity() {
    assert(check_load_capacity(1, 10.0) == 1);
}

void test_calculate_earnings() {
    assert(calculate_all_earnings("2025-01-01", "2025-12-31") > 0);
}

int main() {
    test_order_create();
    test_check_load_capacity();
    test_calculate_earnings();
    return 0;
}