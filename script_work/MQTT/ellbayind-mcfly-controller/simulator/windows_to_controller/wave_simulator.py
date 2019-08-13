import time
import RPi.GPIO as GPIO



def square_wave():
	GPIO.setmode(GPIO.BCM)

	GPIO.setup(7,GPIO.OUT)

	#p = GPIO.PWM( 7, 5)
	
	#p.start(50.0)

	GPIO.output( 7, 1 )

	raw_input("Press Enter key to Stop 1kHz PWM @ 50% duty cycle")

	GPIO.cleanup()

def main():

	square_wave()		

main()