#define RELAY_PIN (9)
#define ADC_PIN (0)
#define _debug
#define DEFAULT_TARGET_TEMPE (60.0)
#define CONTROL_IGNORE_REGION (2.0)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
}


String command;
float current_temperature = 0;
float target_temperature = DEFAULT_TARGET_TEMPE;
void loop() {
  // put your main code here, to run repeatedly:
  static long tm;
  static int init_flag = 1;

  if (init_flag == 1)
  {
    init_flag = 0;
    tm = millis();
  }
  if (Serial.available())
  {
    char c = Serial.read();
    if (c == '\n')
    {
      parseCommand(command);
      command = "";
    }
    else
    {
      command += c;
    }
  }

  //LOOP control in 0.1S period
  if ((millis() - tm) > 100)
  {
    tm += 100;
    //get temperature
    current_temperature = getTemperature(analogRead(ADC_PIN));
    //PID calculate
    //Output
    if (current_temperature >= target_temperature)
    {
      digitalWrite(RELAY_PIN, LOW);
    }
    else if (current_temperature < (target_temperature - CONTROL_IGNORE_REGION))
    {
      digitalWrite(RELAY_PIN, HIGH);
    }
  }
}
void parseCommand(String com)
{
  String part1 , part2;
  char op_char = ' ';
  com.toLowerCase();

  if (com.indexOf('=') != -1)
    op_char = '=';
  else   if (com.indexOf('?') != -1)
    op_char = '?';
  if (op_char != ' ')
  {
    part1 = com.substring(0, com.indexOf(op_char));
    part2 = com.substring(com.indexOf(op_char) + 1);
    part1.trim();
    part2.trim();

    if (op_char == '?') //query data
    {
      if (part1 == "t")
      {
        Serial.println(current_temperature);
        Serial.println("ok");
      }
      else if (part1 == "s")
      {
        Serial.println(target_temperature);
        Serial.println("ok");
      }
    }
    else if ((op_char == '=') && (part2.length() != 0)) // set data
    {
      if (part1 == "s")
      {
        target_temperature = part2.toFloat();
        Serial.println("ok");
      }
    }
  }
}
/*
           3.3V
            |
            R1 (250 ohm)
            +------Analog Out to arduino AD pin
           RTD
            |
           GND

    R to temperature formula
    T = (Rrtd - 100)/ 0.3851
*/

#define R1 (250.0)
#define ADC_MAX (1023.0)
float getRTD_Resister( float adc)
{
  float RTD;
  /*
      adc / ADC_MAX = RTD / (R1 + RTD)
      adc * (R1 + RTD) = R_adc * ADC_MAX
      adc * R1 + adc * R_adc = R_adc * ADC_MAX
      adc * R1 = RTD * ADC_MAX - adc * RTD
      adc * R1 = (ADC_MAX - adc) * RTD
      RTD = (adc * R1) / (ADC_MAX - adc)

  */
  RTD = (adc * R1) / (ADC_MAX - adc);
  return RTD;
}

float getTemperature(int adc)
{
  float RTD, tempe;
  RTD = getRTD_Resister(adc);
  tempe = (RTD - 100) / 0.3851;
#if 1
  Serial.print("Temperature = ");
  Serial.println(tempe);
#endif
  return tempe;
}

