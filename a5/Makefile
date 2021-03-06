objects = satelliteComs.o powerSubsystem.o consoleDisplay.o \
          thrusterSubsystem.o warningAlarm.o rand2.o \
          terminalComs.o keyboardConsole.o solarPanelControl.o \
          vehicleComms.o nonBlockingKeys.o startup.o \
	  scheduler.o pwm_utils.o adc_utils.o \
	  transportDistance.o imageCapture.o batteryTemp.o \
	  optfft.o commandParser.o pirateDetection.o \
	  pirateManagement.o fft.o \

headers = dataStructs.h TCB.h startup.h scheduler.h

all: main

main: main.c $(objects) $(headers)
	gcc -Wall -g -o main main.c $(objects) -lm

pirateManagement.o: pirateManagement.c
	gcc -Wall -g -c -o pirateManagement.o pirateManagement.c

pirateDetection.o: pirateDetection.c
	gcc -Wall -g -c -o pirateDetection.o pirateDetection.c

commandParser.o: commandParser.c satComsParse.h
	gcc -Wall -g -c -o commandParser.o commandParser.c

optfft.o: optfft.c optfft.h tables.c
	gcc -Wall -g -c -o optfft.o optfft.c
	
fft.o: fft.c fft.h
	gcc -Wall -g -c -o fft.o fft.c -lm

transportDistance.o: transportDistance.c
	gcc -Wall -g -c -o transportDistance.o transportDistance.c

imageCapture.o: imageCapture.c optfft.h fft.h
	gcc -Wall -g -c -o imageCapture.o imageCapture.c

batteryTemp.o: batteryTemp.c
	gcc -Wall -g -c -o batteryTemp.o batteryTemp.c

adc_utils.o: adc_utils.c adc_utils.h
	gcc -Wall -g -c -o adc_utils.o adc_utils.c

pwm_utils.o: pwm_utils.c pwm_utils.h
	gcc -Wall -g -c -o pwm_utils.o pwm_utils.c

scheduler.o: scheduler.c
	gcc -Wall -g -c -o scheduler.o scheduler.c

startup.o: startup.c startup.h
	gcc -Wall -g -c -o startup.o startup.c

nonBlockingKeys.o: nonBlockingKeys.c nonBlockingKeys.h
	gcc -Wall -g -c -o nonBlockingKeys.o nonBlockingKeys.c

keyboardConsole.o: keyboardConsole.c
	gcc -Wall -g -c -o keyboardConsole.o keyboardConsole.c

solarPanelControl.o: solarPanelControl.c pwm_utils.h
	gcc -Wall -g -c -o solarPanelControl.o solarPanelControl.c

vehicleComms.o: vehicleComms.c satelliteVehicle.h
	gcc -Wall -g -c -o vehicleComms.o vehicleComms.c

warningAlarm.o: warningAlarm.c warningAlarm.h
	gcc -Wall -g -c -o warningAlarm.o warningAlarm.c

thrusterSubsystem.o: thrusterSubsystem.c thrusterSubsystem.h
	gcc -Wall -g -c -o thrusterSubsystem.o thrusterSubsystem.c

consoleDisplay.o: consoleDisplay.c nonBlockingKeys.h adc_utils.h
	gcc -Wall -g -c -o consoleDisplay.o consoleDisplay.c

powerSubsystem.o: powerSubsystem.c powerSubsystem.h
	gcc -Wall -g -c -o powerSubsystem.o powerSubsystem.c

satelliteComs.o: satelliteComs.c satelliteComs.h nonBlockingKeys.h satelliteVehicle.h satComsParse.h
	gcc -Wall -g -c -o satelliteComs.o satelliteComs.c

terminalComs.o: terminalComs.c TCB.h
	gcc -Wall -g -c -o terminalComs.o terminalComs.c

rand2.o: rand2.c
	gcc -Wall -g -c -o rand2.o rand2.c

copy:
	scp -r ../a5 root@192.168.7.2:/dev

clean:
	rm -rf main *.o *.dSYM
