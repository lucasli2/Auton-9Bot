//Defining Pin Constants
const int THROTTLE_PIN  = 5;
const int BRAKE_PIN = 6;
const int THROTTLE_VSENSE = A0;
const int BRAKE_VSENSE  = A1;
const int CRUISE_BUTTON = 7;

//Math COnstnats
const int THROTTLE_MIN = 60; //TODO
const int BRAKE_MIN = 60; //TODO
const int THROTTLE_MAX = 255;
const int BRAKE_MAX = 255;

const int THROTTLE_VSENSE_MIN = 161; //TODO
const int BRAKE_VSENSE_MIN = 161; //TODO

//Defining Global Variables
bool estop = false;
bool cc_button_read = false;
bool cc_button_last = false;
bool cruise_control = false;
bool cruise_control_last = false;
int throttle_position = 0; //TODO
int brake_position = 0; //TODO
int throttle_read = 0; //TODO
int brake_read = 0; //TODO
int throttle_cruise = 0;
String serial_return = "";


void setup() {
  Serial.begin(115200); //High Speed Baud Rate, may degrade with cable quality/

  pinMode(THROTTLE_PIN, OUTPUT);
  pinMode(BRAKE_PIN,  OUTPUT);
  pinMode(CRUISE_BUTTON,  INPUT);

  TCCR0B = (TCCR0B & 0b11111000) | 0x02; // Uses Prescaler of 8 for Pins 5/6 PWM Output
}


void loop() {
  throttle_read = map(analogRead(THROTTLE_VSENSE) - THROTTLE_VSENSE_MIN, 0, 1023, 0, 255);
  brake_read = map(analogRead(BRAKE_VSENSE) - BRAKE_VSENSE_MIN,0,1023,0,255);

  //Cruise Control Activation from Button
  cc_button_read = digitalRead(CRUISE_BUTTON);
  cruise_control = (cc_button_read && !cc_button_last);
  cc_button_last = cc_button_read;

  if (cruise_control && !cruise_control_last){ //Reads Throttle Voltage When Cruise Control is Triggered
    throttle_cruise = throttle_read;
  }

  if (cruise_control){
    if (brake_read+60>200){
      cruise_control = false; //Disables Cruise Control with the press of brake
    }
    else {
      throttle_position = throttle_cruise;
    }
  }

  if (Serial.available()){ //Serial Control Should only be used for controlling throttle inputs... for now.
    String throttle_wire = Serial.readStringUntil("\n");
    int throttle_input = throttle_wire.toInt();

    throttle_position = constrain(throttle_input, THROTTLE_MIN, THROTTLE_MAX);
  }

  if (estop){
    analogWrite(THROTTLE_PIN, THROTTLE_MIN);
    analogWrite(BRAKE_PIN, BRAKE_MAX);
  }
  else{
    analogWrite(THROTTLE_PIN, throttle_position);
    analogWrite(BRAKE_PIN, BRAKE_MIN);
  }
  serial_return = "<s>TR=" + String(throttle_read) + ",BR=" + String(brake_read)+",CC="+String(cruise_control) +"<e>";
  Serial.println(serial_return);
  }
 


