from time import sleep
from random import randint

lightSequence = 0
lightDelay = 10

def lightning():
    if lightSequence>=0 and lightSequence<lightDelay*1:
        for strand in range(1):
            print(f"Strand {strand}: White")
            print(f"Strand {7-strand}: White")

    if lightSequence>=lightDelay*1 and lightSequence<lightDelay*2:
        for strand in range(2):
            print(f"Strand {strand}: White")
            print(f"Strand {7-strand}: White")

    if lightSequence>=lightDelay*2 and lightSequence<lightDelay*3:
        for strand in range(3):
            print(f"Strand {strand}: White")
            print(f"Strand {7-strand}: White")

    if lightSequence>=lightDelay*3 and lightSequence<lightDelay*4:
        for strand in range(4):
            print(f"Strand {strand}: White")
            print(f"Strand {7-strand}: White")
    if lightSequence>=lightDelay*4:
        if (lightSequence%lightDelay == 0):
            print(f"Strand {randint(0,7)}: White")

while True:
    lightSequence += 1
    lightning()
    print(f'Sequence: {lightSequence}')
    sleep(0.1)
