from serial import Serial
from time import sleep


open_ports = []
LEDs = []
PIRs = []


def get_port(collection, n):
    return collection[n][0]


def get_id(collection, n):
    return collection[n][1]


def open_port(port):
    global open_ports

    open_ports += [Serial(port, 19200)]
    return open_ports[-1]


def call(port, data):
    global open_ports

    if isinstance(port, (int, float)):
        port = open_ports[int(port)]

    port.write(data)
    sleep(0.007 + 0.008 * len(data))

    ret = []
    while port.inWaiting():
        ret += port.read()

    return ret


def add_led(port, red, green, blue):
    global LEDs

    led = call(open_ports[port], [0, red, green, blue])[0]

    LEDs += [(open_ports[port], led)]


def count_leds(port=-1):
    if port == -1:
        return len(LEDs)
    return call(port, [7])


def count_pirs(port=-1):
    if port == -1:
        return len(PIRs)
    return call(port, [8])


def set_led(led, red, green, blue):
    call(get_port(LEDs, led), [2, get_id(LEDs, led), (red << 2) | (green << 1) | blue])


def reset_led(led):
    call(get_port(LEDs, led), [3, get_id(LEDs, led)])


def add_pir(port, pin):
    global PIRs

    pir = call(open_ports[port], [1, pin])[0]

    PIRs += [(open_ports[port], pir)]


def update_pirs():
    for n in open_ports:
        call(n, [4])


def check_pir(pir):
    global PIRs

    return call(get_port(PIRs, pir), [5, get_id(PIRs, pir)])[0]


def check_pir_edge(pir, edge=-1):
    global PIRs

    if isinstance(edge, int) and edge == -1:
        pir_id = get_id(PIRs, pir)
        return call(get_port(PIRs, pir), [6, pir_id, 0, 6, pir_id, 1])
    else:
        return call(get_port(PIRs, pir), [6, get_id(PIRs, pir), edge])[0]