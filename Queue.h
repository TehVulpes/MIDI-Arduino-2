/*
 * Created by Keegan Dahm on 4/9/15.
 */

#ifndef MIDI_ARDUINO_2_QUEUE_H
#define MIDI_ARDUINO_2_QUEUE_H


#include <stdlib.h>

template <typename T> class Queue {
private:
    T *data;
    unsigned int length;
    unsigned int front, back;

public:
    Queue(unsigned int length) {
        length;
        data = (T *) malloc((length + 1) * sizeof(T));

        this->length = length;
        front = 0;
        back = 0;
    }

    ~Queue() {
        free((void *) data);
    }

    bool enqueue(T data) {
        if (full())
            return false;

        this->data[back++] = data;

        if (back > length)
            back = 0;

        return true;
    }

    T peek() {
        if (empty())
            return 0xFF;

        return this->data[front];
    }

    T dequeue() {
        if (empty())
            return 0xFF;

        T data = this->data[front++];
        if (front > length)
            front = 0;

        return data;
    }

    void clear() {
        front = back;
    }

    bool full() {
        if (front > back)
            return front == back + 1;
        else if (front < back)
            return front == 0 && back == length;
        else
            return false;
    }

    bool empty() {
        return front == back;
    }

    unsigned int space_occupied() {
        if (front > back)
            return length - (front - back) + 1;
        else if (front == back)
            return 0;
        else
            return back - front;
    }

    unsigned int space_free() {
        return length - space_occupied();
    }

    unsigned int space() {
        return length;
    }
};


#endif /* MIDI_ARDUINO_2_QUEUE_H */
