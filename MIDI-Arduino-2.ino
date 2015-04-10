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

    leds[leds.length() - 1]->set();

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

void update_PIRs() {
    for (int i = 0; i < pirs.length(); i++)
        pirs[i]->update();
}

void check_PIR() {
    unsigned int pir = stream.read();

    bool value = pirs[pir]->check();

    if (value)
        Serial.write(1);
    else
        Serial.write(0);
}

void check_PIR_edge() {
    unsigned int pir = stream.read();
    unsigned int edge = stream.read();

    bool value = edge ? pirs[pir]->check_positive_edge() : pirs[pir]->check_negative_edge();

    if (value)
        Serial.write(1);
    else
        Serial.write(0);
}

void get_num_LEDs() {
    Serial.write(leds.length());
}

void get_num_PIRs() {
    Serial.write(pirs.length());
}

inline void reset() {
    asm("jmp 0");
}

struct {
    void (*handler)();
    unsigned int args;
} commands [] = {
        { add_LED, 3 },     // 0
        { add_PIR, 1 },     // 1
        { set_LED, 2 },     // 2
        { reset_LED, 1 },   // 3
        { update_PIRs, 0 }, // 4
        { check_PIR, 1 },   // 5
        { check_PIR_edge, 2 },  // 6
        { get_num_LEDs, 0 },//7
        { get_num_PIRs, 0 },//8
        { 0, 0 }
};
unsigned int num_commands;

void reply() {
    unsigned int bytes = stream.available();

    while (stream.peek() < num_commands && commands[stream.peek()].args < stream.available())
        commands[stream.read()].handler();

    if (stream.available()) {
        if(stream.peek() == 0xFF) {
            stream.flush();
            Serial.write(0xFF);
        }
        else if (stream.peek() == 0xFE)
            reset();
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

    delay(1);
}