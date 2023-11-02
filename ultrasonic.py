import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
GPIO_TRIG = 14
GPIO_ECHO = 18
GPIO.setup(GPIO_TRIG, GPIO.OUT)
GPIO.setup(GPIO_ECHO, GPIO.IN)
while True:
	GPIO.output(GPIO_TRIG, GPIO.LOW)
	time.sleep(0.1)
	GPIO.output(GPIO_TRIG, GPIO.HIGH)
	time.sleep(0.00001)
	GPIO.output(GPIO_TRIG, GPIO.LOW)
	start_time = 0
	Bounce_back_time = 0
	pulse_duration = 0
	while GPIO.input(GPIO_ECHO)==0:
		start_time = time.time()  
	while GPIO.input(GPIO_ECHO)==1:  
		Bounce_back_time = time.time()  
	pulse_duration = Bounce_back_time - start_time  
	distance = round(pulse_duration * 17150, 2)  
	print ("Distance: " + str(distance) + " cm")
GPIO.cleanup()
