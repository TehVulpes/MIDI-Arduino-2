/*
 * Created by Keegan Dahm on 4/9/15.
 */

#ifndef MIDI_ARDUINO_2_SERIALSTREAM_H
#define MIDI_ARDUINO_2_SERIALSTREAM_H


#include "Queue.h"
#include <Arduino.h>
#include <stdint.h>

class SerialStream {
private:
    Queue<uint8_t> *q;

public:
    SerialStream(unsigned int length = 128);
    ~SerialStream();

    bool buffer();
    unsigned int available();
    uint8_t read();
    uint8_t peek();
    void flush();
};


#endif /* MIDI_ARDUINO_2_SERIALSTREAM_H */
