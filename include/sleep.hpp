#pragma once



#include <chrono>
#include <thread>



// Pauses code for a set amount of time.
void sleep(float seconds) {
    std::chrono::milliseconds ms{static_cast<long int>(seconds * 1000)};
    std::this_thread::sleep_for(ms);
}