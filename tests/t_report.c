#include <stddef.h>
#include <assert.h>
#include "report.h"

void test_report_by_driver() {
    report_by_driver("Ivanov", "2025-01-01", "2025-12-31");
}

void test_report_by_car() {
    report_by_car("A123BC");
}

void test_report_all_drivers() {
    report_all_drivers_stats();
}

int main() {
    test_report_by_driver();
    test_report_by_car();
    test_report_all_drivers();
    return 0;
}