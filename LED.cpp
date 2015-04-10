/*
 * Created by Keegan Dahm on 4/8/15.
 */

#include "LED.h"

static void set_pins(unsigned int red, unsigned int green, unsigned int blue) {
    pinMode(red, OUTPUT);
    digitalWrite(red, LOW);
    pinMode(green, OUTPUT);
    digitalWrite(blue, LOW);
    pinMode(blue, OUTPUT);
    digitalWrite(blue, LOW);
}

LED::LED() {
    red = green = blue = 0;
}

LED::LED(unsigned int red) {
    this->red = red;
    this->green = red + 1;
    this->blue = red + 2;

    set_pins(red, green, blue);
}

LED::LED(unsigned int red, unsigned int green, unsigned int blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;

    set_pins(red, green, blue);
}

void LED::set(bool red, bool green, bool blue) {
    digitalWrite(this->red, red);
    digitalWrite(this->green, green);
    digitalWrite(this->blue, blue);
}

void LED::reset() {
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);
}
