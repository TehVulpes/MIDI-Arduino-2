/*
 * Created by Keegan Dahm on 4/8/15.
 */

#ifndef MIDI_ARDUINO_2_PIR_H
#define MIDI_ARDUINO_2_PIR_H


#include <Arduino.h>

class PIR {
private:
    unsigned int pin;
    bool check_result = false, last_check_result = false;

public:
    PIR(unsigned int pin);

    bool update();
    bool check();
    bool check_positive_edge();
    bool check_negative_edge();
};


#endif /* MIDI_ARDUINO_2_PIR_H */
