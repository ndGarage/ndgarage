import board
import busio
import digitalio
import time

i2c = busio.I2C(board.SCL, board.SDA)
while not i2c.try_lock():
    pass


# Build the led array
count = 18
pins = []
leds = []
dirs = []
for i in range(count):
    n = i + 1 # starting from 1
    p = 'board.D%d'%n
    l = 'led%d'%n
    d = 'led%d.direction'%n
    pins.append(p)
    leds.append(l)
    dirs.append(d)

# configure the pins as output

for i in range(count):
    leds[i] = digitalio.DigitalInOut(pins[i])
    dirs[i] = digitalio.Direction.OUTPUT

# test each LED fast
for l in leds:
    l.value = True
    time.sleep(0.3)
    l.value = False
    time.sleep(0.2)

