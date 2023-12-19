const path = require('path')
const rl = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
  })
const {SerialPort} = require('serialport');

const tests = [
    {
        command: 'idr',
        description: 'Mostar ID no display',
    },
    {
        command: 'gor',
        description: 'Somente buzzer',
    },
    {
        command: 'n0.0r',
        description: 'Mostra 0.0 no display',
    },
    {
        command: 'n2.5.r',
        description: 'Mostra 2.5 no display',
    },
    {
        command: 'n8.8r',
        description: 'Mostra 8.8 no display',
    },
    {
        command: 'lpr',
        description: 'Buzzer e led lane 1/lane 2 (amarelo)',
    },
    {
        command: 'gr',
        description: 'Mostra luz azul permanente',
    },
    {
        command: 'gfr',
        description: 'Apaga luz azul',
    },
    {
        command: 'c1r',
        description: 'Pisca luz vermelha',
    },
    {
        command: 'sr',
        description: 'Liga a outra luz azul e buzzer',
    }
];


// exec shell avrdude
const { exec } = require('child_process');
const { exit } = require('process');
const avrdude = (args) => {
    return new Promise((resolve, reject) => {
        exec(`${path.resolve(process.env.HOME + '/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/bin/avrdude')} ${args}`, (error, stdout, stderr) => {
            if (error) {
                reject(error)
            }
            resolve(stdout ? stdout : stderr)
        });
    })
}


const main = async () => {
    // const res = await avrdude(`-C${path.resolve(process.env.HOME + '/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf')} -v -patmega328p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:${path.resolve(__dirname + '/bin/Display5621BS.ino.hex')}:i`)
    // console.log(res)

    const serialport = new SerialPort({ path: '/dev/ttyUSB0', baudRate: 9600 })

    await new Promise((resolve) => setTimeout(resolve, 3000))

    serialport.on('data', (data) => {
        console.log('Data:', data.toString())
    })

    serialport.on('error', function(err) {
        console.log('Error: ', err.message);
      })

      let currentIndex = 0;
      while (currentIndex < tests.length) {
        await sendCommand(serialport, currentIndex);
  
          const userInput = await getUserInput('Press Enter for next test, R to repeat: ');
        // const userInput = ''
          if (userInput.toUpperCase() !== 'R') {
              currentIndex++;
          }
      }

    serialport.close()
    exit()
}


const getUserInput = (query) => {

    return new Promise((resolve) => {
        rl.question(query, (input) => {
            // rl.close();
            resolve(input);
        });
    });
};

const sendCommand = async (serial, index) => {
    const test = tests[index];
    serial.write(test.command, 'ascii', (err) => {
        if (err) {
            console.log('Error on write: ', err.message);
        } else {
            console.log(`Enviado comando: ${test.description}`);
        }
    });
    await new Promise((resolve) => setTimeout(resolve, 1000));
};

main()