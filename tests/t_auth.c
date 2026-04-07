#include <stddef.h>
#include <assert.h>
#include "auth.h"

void test_login() {
    assert(login("user", "pass") == 1);
}

void test_logout() {
    assert(logout() == 1);
}

void test_get_user_id() {
    assert(get_current_user_id() == 1);
}

int main() {
    test_login();
    test_logout();
    test_get_user_id();
    return 0;
}