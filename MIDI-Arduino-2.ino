/*
 * Created by Keegan Dahm on 4/8/15.
 */

#include "LED.h"
#include "PIR.h"
#include "SerialStream.h"
#include "AutoArray.h"

#include <stdlib.h>

AutoArray<LED *> leds;
AutoArray<PIR *> pirs;
SerialStream stream(128);


void add_LED() {
    unsigned int red = stream.read();
    unsigned int green = stream.read();
    unsigned int blue = stream.read();

    if (!leds.add(new LED(red, green, blue))) {
        Serial.write(0xFF);
        return;
    }

    leds[0]->set();

    Serial.write(leds.length() - 1);
}

void add_PIR() {
    unsigned int pin = stream.read();

    if (!pirs.add(new PIR(pin))) {
        Serial.write(0xFF);
        return;
    }

    Serial.write(pirs.length() - 1);
}

void set_LED() {
    unsigned int led = stream.read();
    unsigned int values = stream.read();

    leds[led]->set((values >> 2) & 1, (values >> 1) & 1, values & 1);
}

void reset_LED() {
    unsigned int led = stream.read();

    leds[led]->reset();
}

struct {
    void (*handler)();
    unsigned int args;
} commands [] = {
        { add_LED, 3 },     // 0
        { add_PIR, 1 },     // 1
        { set_LED, 2 },     // 2
        { reset_LED, 1 },   // 3
        { 0, 0 }
};
unsigned int num_commands;

void reply() {
    unsigned int bytes = stream.available();

    while (stream.peek() < num_commands && commands[stream.peek()].args < stream.available()) {
        commands[stream.read()].handler();
    }
}


void setup() {
    Serial.begin(19200);

    for(num_commands = 0; commands[num_commands].handler; num_commands++);
}

void loop() {
    if (Serial.available())
        stream.buffer();

    if (stream.available())
        reply();

    delay(10);
}