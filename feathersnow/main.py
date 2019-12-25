import board
import digitalio
import time

led1 = digitalio.DigitalInOut(board.D1)
led1.direction = digitalio.Direction.OUTPUT

led2 = digitalio.DigitalInOut(board.D2)
led2.direction = digitalio.Direction.OUTPUT

led3 = digitalio.DigitalInOut(board.D3)
led3.direction = digitalio.Direction.OUTPUT

led4 = digitalio.DigitalInOut(board.D4)
led4.direction = digitalio.Direction.OUTPUT

led5 = digitalio.DigitalInOut(board.D5)
led5.direction = digitalio.Direction.OUTPUT

led6 = digitalio.DigitalInOut(board.D6)
led6.direction = digitalio.Direction.OUTPUT

led7 = digitalio.DigitalInOut(board.D7)
led7.direction = digitalio.Direction.OUTPUT

led8 = digitalio.DigitalInOut(board.D8)
led8.direction = digitalio.Direction.OUTPUT

led9 = digitalio.DigitalInOut(board.D9)
led9.direction = digitalio.Direction.OUTPUT

led10 = digitalio.DigitalInOut(board.D10)
led10.direction = digitalio.Direction.OUTPUT

led11 = digitalio.DigitalInOut(board.D11)
led11.direction = digitalio.Direction.OUTPUT

led12 = digitalio.DigitalInOut(board.D12)
led12.direction = digitalio.Direction.OUTPUT

led13 = digitalio.DigitalInOut(board.D13)
led13.direction = digitalio.Direction.OUTPUT

led14 = digitalio.DigitalInOut(board.D14)
led14.direction = digitalio.Direction.OUTPUT

led15 = digitalio.DigitalInOut(board.D15)
led15.direction = digitalio.Direction.OUTPUT

led16 = digitalio.DigitalInOut(board.D16)
led16.direction = digitalio.Direction.OUTPUT

led17 = digitalio.DigitalInOut(board.D17)
led17.direction = digitalio.Direction.OUTPUT

led18 = digitalio.DigitalInOut(board.D18)
led18.direction = digitalio.Direction.OUTPUT

leds = [led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12, led13, led14, led15, led16, led17, led18]

# turn off all
for l in leds:
    l.value = True

# shift
a = []
b = []
c = []
for i in range(6):
    a.append(leds[3*i])
    b.append(leds[3*i+1])
    c.append(leds[3*i+2])

for i in range(6):
    for l in a:
        l.value = False
    time.sleep(0.2)
    for l in a:
        l.value = True
    time.sleep(0.2)
    for l in b:
        l.value = False
    time.sleep(0.2)
    for l in b:
        l.value = True
    time.sleep(0.2)
    for l in c:
        l.value = False
    time.sleep(0.2)
    for l in c:
        l.value = True
    time.sleep(0.2)

# turn on one by one
while True:
    for l in leds:
        l.value = False
        time.sleep(0.1)
        l.value = True
        time.sleep(0.1)

