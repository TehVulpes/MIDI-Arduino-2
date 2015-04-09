/*
 * Created by Keegan Dahm on 4/8/15.
 */

#ifndef MIDI_ARDUINO_2_LED_H
#define MIDI_ARDUINO_2_LED_H

#include <Arduino.h>


class LED {
private:
    unsigned int red, green, blue;
    bool initialized = false;
    
public:
    LED();
    LED(unsigned int red);
    LED(unsigned int red, unsigned int green, unsigned int blue);

    void set(bool red, bool green, bool blue);
    void reset();
};


#endif /* MIDI_ARDUINO_2_LED_H */
