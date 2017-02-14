# Sous-vide-cooker
A temperature controller for Sous vide method


- You can change RTD input analog and releay control output pin by change define macros below

	```
 	#define RELAY_PIN (9)
 	#define ADC_PIN (0)
	```
- Change target temperature by change define macro below 

	```
	#define DEFAULT_TARGET_TEMPE (60.0)
	```
- The RDT convert principle and formula

	```
 	#define R1 (250.0) <-- the shunt resister
 	#define ADC_MAX (1023.0) <-- ADC resolution
	
	3.3V
	|
	R1 (250 ohm)
	+------Analog Out to arduino AD pin
	RTD
	|
	GND

	adc / ADC_MAX = RTD / (R1 + RTD)
	adc * (R1 + RTD) = R_adc * ADC_MAX
	adc * R1 + adc * R_adc = R_adc * ADC_MAX
	adc * R1 = RTD * ADC_MAX - adc * RTD
	adc * R1 = (ADC_MAX - adc) * RTD
	RTD = (adc * R1) / (ADC_MAX - adc)

	R to temperature formula
	T = (Rrtd - 100)/ 0.3851
	```
- You can change target temperature and read current temperature by using serial console

	Serial console set to 115200,n,8,1
	
	```
	s=? query target temperature
	s=70 set target temperature to 70 degree C	
	t=? query current temperature
	```
