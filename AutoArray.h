/*
 * Created by Keegan Dahm on 4/9/15.
 */

#ifndef MIDI_ARDUINO_2_AUTOARRAY_H
#define MIDI_ARDUINO_2_AUTOARRAY_H


#include <stdlib.h>

template <typename T> class AutoArray {
private:
    T *data;
    unsigned int allocated;

public:
    AutoArray() {
        allocated = 0;
        data = (T *) malloc(allocated * sizeof(T));
    }

    ~AutoArray() {
        free((void *) data);
    }

    bool add(T addition) {
        T *new_data = (T *) realloc(data, (allocated + 1) * sizeof(T));

        if (!new_data)
            return false;

        new_data[allocated++] = addition;
        data = new_data;

        return true;
    }

    inline unsigned int length() {
        return allocated;
    }

    inline T &operator[](unsigned int n) {
        return data[n];
    }
};


#endif /* MIDI_ARDUINO_2_AUTOARRAY_H */
