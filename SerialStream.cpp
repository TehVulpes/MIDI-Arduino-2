/*
 * Created by Keegan Dahm on 4/9/15.
 */

#define MIN_SIZE 8

#include "SerialStream.h"

#include <stdint.h>

SerialStream::SerialStream(unsigned int length) {
    q = new Queue<uint8_t>(length);
}

SerialStream::~SerialStream() {
    delete q;
}

bool SerialStream::buffer() {
    while (Serial.available())
        if (!q->enqueue(Serial.read()))
            return false;
    return true;
}

unsigned int SerialStream::available() {
    return q->space_occupied();
}

uint8_t SerialStream::read() {
    return q->dequeue();
}

uint8_t SerialStream::peek() {
    return q->peek();
}

void SerialStream::flush() {
    q->clear();
}
