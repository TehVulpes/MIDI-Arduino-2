/*
 * Created by Keegan Dahm on 4/8/15.
 */

#include "LED.h"
#include "PIR.h"
#include "SerialStream.h"
#include "Queue.h"

#include <stdlib.h>

PIR p(10);
LED l(7);
SerialStream stream(128);

void setup() {
    Serial.begin(19200);
}

void loop() {
    /*
    if (stream.available())
        l.set();
    else
        l.reset();
        */

    if (Serial.available()) {
        stream.buffer();
    }
    else if (stream.available()) {
        Serial.print("Printing: ");
        while (stream.available())
            Serial.write(stream.read());

        Serial.write('\n');
    }

    delay(10);
}