#ifndef ORDER_H
#define ORDER_H

typedef struct {
    int id;
    char order_date[11];
    int driver_id;
    int car_id;
    int mileage;
    float cargo_weight;
    float cost;
} Order;

int order_create(Order *order);
Order* order_read(int id);
int order_update(Order *order);
int order_delete(int id);
void order_list(void);
int check_load_capacity(int car_id, float cargo_weight);
float calculate_all_earnings(const char *start, const char *end);
float calculate_driver_earnings(const char *start, const char *end, const char *last_name);

#endif