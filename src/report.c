#include "report.h"
#include <stdio.h>

void report_by_driver(const char* last_name, const char* start, const char* end) {
    printf("Report by driver %s from %s to %s\n", last_name, start, end);
}

void report_by_car(const char* plate_number) {
    printf("Report by car %s\n", plate_number);
}

void report_all_drivers_stats(void) {
    printf("All drivers stats\n");
}

void report_driver_min_trips(void) {
    printf("Driver with min trips\n");
}

void report_car_max_mileage(void) {
    printf("Car with max mileage\n");
}