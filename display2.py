import os
import time
import serial
from serial.tools import list_ports
from subprocess import Popen, PIPE

tests = [
    {'command': 'gir' , 'description': 'Seta para direita'},
    {'command': 'lefr' , 'description': 'Seta para esquerda'},
    {'command': 'idr', 'description': 'Mostar ID no display'},
    {'command': 'lef2r', 'description': 'Seta para esquerda'},
    {'command': 'gi2r', 'description': 'Seta para direita'},
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

# hex_file_path = os.path.join(os.path.dirname(_file_), 'bin/Display5621BS.ino.hex')

def send_command(ser, index):
    test = tests[index]
    ser.write(test['command'].encode('ascii'))
    print(f"Enviado comando: {test['description']}")
    time.sleep(3)

def main():

    ser = serial.Serial(port="/dev/ttyUSB0", baudrate=9600, timeout=1)
    time.sleep(3)
    i=0
    while i< (len(tests)):
        send_command(ser, i)
        i=i+1
          
        
            

    ser.close()

if __name__ == "__main__":
    main()