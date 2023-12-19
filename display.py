import os
import time
import serial
from serial.tools import list_ports
from subprocess import Popen, PIPE

tests = [
    {'command': 'idr', 'description': 'Mostar ID no display'},
    {'command': 'gor', 'description': 'Somente buzzer'},
    {'command': 'n0.0r', 'description': 'Mostra 0.0 no display'},
    {'command': 'n2.5.r', 'description': 'Mostra 2.5 no display'},
    {'command': 'n8.8r', 'description': 'Mostra 8.8 no display'},
    {'command': 'lpr', 'description': 'Buzzer e led lane 1/lane 2 (amarelo)'},
    {'command': 'gr', 'description': 'Mostra luz azul permanente'},
    {'command': 'gfr', 'description': 'Apaga luz azul'},
    {'command': 'c1r', 'description': 'Pisca luz vermelha'},
    {'command': 'sr', 'description': 'Liga a outra luz azul e buzzer'},
]

# avrdude_path = os.path.expanduser('~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/bin/avrdude')
# avrdude_conf_path = os.path.expanduser('~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino17/etc/avrdude.conf')
# hex_file_path = os.path.join(os.path.dirname(_file_), 'bin/Display5621BS.ino.hex')

# def avrdude(args):
#     process = Popen([avrdude_path] + args, stdout=PIPE, stderr=PIPE)
#     stdout, stderr = process.communicate()
#     return stdout.decode() if stdout else stderr.decode()

def send_command(ser, index):
    test = tests[index]
    ser.write(test['command'].encode('ascii'))
    print(f"Enviado comando: {test['description']}")
    time.sleep(3)

def main():
    # avrdude_args = [
    #     f'-C{avrdude_conf_path}', '-v', '-patmega328p', '-carduino',
    #     f'-P{list_ports.comports()[0].device}', '-b115200', '-D',
    #     f'-Uflash:w:{hex_file_path}:i'
    # ]
    # avrdude_result = avrdude(avrdude_args)
    # print(avrdude_result)

    ser = serial.Serial(port= '/dev/ttyUSB0', baudrate=9600, timeout=1)
    time.sleep(3)

    while True:
        for i in range(len(tests)):
            send_command(ser, i)
            userInput = input('Press Enter for next test, R to repeat: ')
            # userInput = ''
            if userInput.upper() != 'R':
                break

    ser.close()

if __name__ == "_main_":
    main()