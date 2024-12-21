//Throttle Duty Cycle minimum 0.8 Volts, Max 4.2 Volts
int throttlePin = 5;





void setup() {
  Serial.begin(9600);
  pinMode(throttlePin, OUTPUT);
  TCCR0B = (TCCR0B & 0b11111000) | 0x02; // Prescaler = 8

}


void loop() {

  if (Serial.available()){

    String input = Serial.readStringUntil("\n");
    int userValue = input.toInt();

    userValue = constrain(userValue, 0, 255);
    analogWrite(throttlePin,userValue);

  }
  




  // for (float dutyCycle = 30; dutyCycle <= 230; dutyCycle += 1){
  //   analogWrite(throttlePin, dutyCycle);
  //   Serial.println(dutyCycle);
  //   delay(1000);
  // }
  }
 


