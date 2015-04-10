from serial import Serial
from time import time, sleep
from rtmidi import MidiIn, MidiOut
from _thread import start_new_thread


open_ports = []
LEDs = []
PIRs = []

m_out = MidiOut().open_virtual_port("Python PIR Output")

midi_listen = False
melody_in = MidiIn().open_virtual_port("Python Melody Input")
drum_in = MidiIn().open_virtual_port("Python Drum Input")
last_drum = 0


stair_modes = [
    [48, 50, 52, 53, 55, 57, 59, 60],
    [
        [48, 52, 55],
        [50, 53, 57],
        [52, 55, 59],
        [53, 57, 60],
        [55, 59, 62],
        [57, 60, 64],
        [59, 62, 65],
        [60, 64, 67],
        [62, 65, 69],
        [64, 67, 71],
        [65, 69, 72],
        [67, 71, 74],
        [69, 72, 76],
        [71, 74, 77],
        [72, 76, 79]
    ],
    [48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78]
]


melody_data = [-1] * 255


light_data = []
for N in range(0, 21):
    light_data += [[0, False, False, False, False]]


def get_port(collection, n):
    return collection[n][0]


def get_id(collection, n):
    return collection[n][1]


def open_port(port):
    global open_ports

    open_ports += [Serial(port, 19200)]
    return open_ports[-1]


def call(port, data, rets=0):
    global open_ports

    if isinstance(port, (int, float)):
        port = open_ports[int(port)]

    port.flushInput()
    port.write(data)

    timeout = time() + 0.016 + 0.0005 * len(data)
    while time() < timeout and port.inWaiting() < rets:
        pass

    if port.inWaiting() < rets:
        return call(port, data, rets)

    ret = []
    while port.inWaiting():
        ret += port.read()

    return ret


def add_led(port, red, green, blue):
    global LEDs

    led = call(open_ports[port], [0, red, green, blue], 1)[0]

    LEDs += [(open_ports[port], led)]


def count_leds(port=-1):
    if port == -1:
        return len(LEDs)
    return call(port, [7], 1)


def count_pirs(port=-1):
    if port == -1:
        return len(PIRs)
    return call(port, [8], 1)


def set_led(led, red=1, green=1, blue=1):
    call(get_port(LEDs, led), [2, get_id(LEDs, led), (red << 2) | (green << 1) | blue])


def reset_led(led):
    call(get_port(LEDs, led), [3, get_id(LEDs, led)])


def add_pir(port, pin):
    global PIRs

    pir = call(open_ports[port], [1, pin], 1)[0]

    PIRs += [(open_ports[port], pir)]


def update_pirs():
    for n in open_ports:
        call(n, [4])


def check_pir(pir):
    global PIRs

    return call(get_port(PIRs, pir), [5, get_id(PIRs, pir)], 1)[0]


def check_pir_edge(pir, edge=-1):
    global PIRs

    if isinstance(edge, int) and edge == -1:
        pir_id = get_id(PIRs, pir)
        return call(get_port(PIRs, pir), [6, pir_id, 0, 6, pir_id, 1], 2)
    else:
        return call(get_port(PIRs, pir), [6, get_id(PIRs, pir), edge], 1)[0]


def midi_write_on(notes, velocity=127):
    midi_write_off(notes)
    if not hasattr(notes, "__iter__"):
        notes = [notes]

    for n in notes:
        m_out.send_message([144, n, velocity])


def midi_write_off(notes):
    if not hasattr(notes, "__iter__"):
        notes = [notes]

    for n in notes:
        m_out.send_message([128, n, 0])


def melody_callback(data, useless):
    global melody_data
    data = data[0]

    if data[0] == 144:
        melody_data[data[1]] = time()
    elif data[0] == 128:
        melody_data[data[1]] = -1


def drum_callback(data, useless):
    global last_drum
    data = data[0]

    if data[0] == 144:
        last_drum = time()


def set_led(n, colors, timeout=0):
    data = light_data[n]
    if data[0] != timeout or data[1] != colors[0] or data[2] != colors[1] or data[3] != colors[2]:
        data[0] = timeout
        data[1] = colors[0]
        data[2] = colors[1]
        data[3] = colors[2]
        data[4] = True


def get_color(n):
    if n == -1:
        return [False, False, False]
    return [True, True, True]


def input_runner():
    temp_melody_data = []
    for n in range(0, 12):
        temp_melody_data += [-1]

    while True:
        for i in range(0, 12):
            temp_melody_data[i] = -1
            for n in range(i, len(melody_data), 12):
                if melody_data[n] > melody_data[temp_melody_data[i]]:
                    temp_melody_data[i] = n

        for i in range(0, 12):
            to_set = get_color(temp_melody_data[i])
            timeout = 0

            if time() < last_drum + 0.2:
                if i > 0 and temp_melody_data[i - 1] != -1:
                    to_set = [True, False, False]
                    timeout = last_drum + 0.2
                elif i < 11 and temp_melody_data[i + 1] != -1:
                    to_set = [True, False, False]
                    timeout = last_drum + 0.2

            set_led(i + 4, to_set, timeout)

        sleep(0.01)


def light_runner():
    global light_data
    while True:
        for n in range(0, len(light_data)):
            data = light_data[n]
            if not data[0]:
                continue

            if time() > data[0]:
                light_data[n] = [0, False, False, False, False]
                # reset_led(n)
                print(str(n) + " ended")

        for n in range(0, len(light_data)):
            data = light_data[n]
            if data[4]:
                # set_led(n, data[1], data[2], data[3])
                print(str(n) + ": " + str(data[1]) + ", " + str(data[2]) + ", " + str(data[3]))
                data[4] = False

        sleep(0.01)


melody_in.set_callback(melody_callback)
drum_in.set_callback(drum_callback)
start_new_thread(light_runner, ())
start_new_thread(input_runner, ())