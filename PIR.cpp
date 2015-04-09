/*
 * Created by Keegan Dahm on 4/8/15.
 */

#include "PIR.h"

PIR::PIR(unsigned int pin) {
    this->pin = pin;

    pinMode(pin, INPUT);
}

bool PIR::update() {
    last_check_result = check_result;
    check_result = digitalRead(pin);
    return check_result;
}

bool PIR::check() {
    return check_result;
}

bool PIR::check_positive_edge() {
    return check_result && !last_check_result;
}

bool PIR::check_negative_edge() {
    return !check_result && last_check_result;
}
