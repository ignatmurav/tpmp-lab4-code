#include "order.h"
#include <stdio.h>

int order_create(Order* order) {
    printf("Order created\n");
    return 1;
}

Order* order_read(int id) {
    printf("Order read\n");
    return NULL;
}

int order_update(Order* order) {
    printf("Order updated\n");
    return 1;
}

int order_delete(int id) {
    printf("Order deleted\n");
    return 1;
}

void order_list(void) {
    printf("Listing orders\n");
}

int check_load_capacity(int car_id, float cargo_weight) {
    return 1;
}

float calculate_all_earnings(const char* start, const char* end) {
    return 10000.0;
}

float calculate_driver_earnings(const char* start, const char* end, const char* last_name) {
    return 5000.0;
}