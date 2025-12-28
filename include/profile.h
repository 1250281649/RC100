#include <esp_timer.h>
#ifndef PROFILE_H
#define PROFILE_H


inline unsigned long millis() {
    return esp_timer_get_time() / 1000;
}

inline unsigned long micros() {
    return esp_timer_get_time();
}

#define TIME_FUNCTION(func_call)                     \
    {                                               \
        int64_t start_time = esp_timer_get_time(); \
        func_call;                                 \
        int64_t end_time = esp_timer_get_time();   \
        printf("Function %s took %lld microseconds\n", #func_call, end_time - start_time); \
    }

#endif // PROFILE_H
