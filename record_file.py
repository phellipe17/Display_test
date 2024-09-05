import time
import serial
import subprocess
import os

tests = [
    {
        "command": 'idr',
        "description": 'Mostrar ID no display',
    },
    {
        "command": 'gor',
        "description": 'Somente buzzer',
    },
    {
        "command": 'n0.0r',
        "description": 'Mostra 0.0 no display',
    },
    {
        "command": 'n2.5.r',
        "description": 'Mostra 2.5 no display',
    },
    {
        "command": 'n8.8r',
        "description": 'Mostra 8.8 no display',
    },
    {
        "command": 'lpr',
        "description": 'Buzzer e led lane 1/lane 2 (amarelo)',
    },
    {
        "command": 'gr',
        "description": 'Mostra luz azul permanente',
    },
    {
        "command": 'gfr',
        "description": 'Apaga luz azul',
    },
    {
        "command": 'c1r',
        "description": 'Pisca luz vermelha',
    },
    {
        "command": 'sr',
        "description": 'Liga a outra luz azul e buzzer',
    }
]

def avrdude(args):
    try:
        result = subprocess.run([os.path.expanduser('~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/bin/avrdude')] + args, check=True, text=True, capture_output=True)
        return result.stdout
    except subprocess.CalledProcessError as e:
        return e.stderr

def main():
    res = avrdude([
        '-C{}'.format(os.path.expanduser('~/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf')),
        '-v', '-patmega328p', '-carduino', '-P/dev/ttyARD', '-b115200', '-D',
        '-Uflash:w:{}:i'.format(os.path.join(os.path.dirname(__file__), '/home/pi/bin_code/Display5621BS.ino.hex'))
    ])
    print(res)

    serial_port = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    
    time.sleep(3)

    try:
        while serial_port.is_open:
            currentIndex = 0
            while currentIndex < len(tests):
                send_command(serial_port, currentIndex)
                user_input = input('Press Enter for next test, R to repeat: ')
                if user_input.upper() != 'R':
                    currentIndex += 1
    except Exception as e:
        print('Error:', str(e))
    finally:
        serial_port.close()

def send_command(serial, index):
    test = tests[index]
    serial.write(test["command"].encode('ascii'))
    print(f'Enviado comando: {test["description"]}')
    time.sleep(1)

if __name__ == "__main__":
    main()
