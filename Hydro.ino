// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
float V,Va;                                                          // Voltage variable
float I;
float S;
int f=7, O=0;
Servo myservo; 

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int i=0,val;
void setup() {
  // set up the LCD's number of columns and rows:
    Serial.begin(9600);
  lcd.begin(20, 4);
  // Print a message to the LCD.

   myservo.attach(9);
   pinMode(A0, INPUT);
   pinMode(O, INPUT);
   pinMode(A2, INPUT);
   pinMode(A3, INPUT);
   pinMode(f, OUTPUT);

}

void loop() { 
  lcd.clear();
    lcd.setCursor(0, 0);
  lcd.print("  Water Turbine  ");
  V = analogRead(A2);                                              // Read the voltage on the A0 pin                                                         // Set R equal to the raw ADC value
  delay(10);
 Va = (V/1023);
 Va= Va*366;                                                  // Convert the digital ADC value to volts
 I= analogRead(A3);
 I= I/1023;
 S=(I*V);
 S= S/1000 ;
                                                
if(digitalRead(0)==HIGH)
  {
    digitalWrite(f,HIGH);
    lcd.clear();
    lcd.print(" Fire Alert !!");  
  }
  else
  {digitalWrite (f,LOW);}
  sensorValue = Va ; 
  val = map(sensorValue, 120 , 350, 0, 180);
  myservo.write(val);
 lcd.setCursor(0, 1);
  lcd.print("I = ");
  lcd.setCursor(5, 1);
  lcd.print(I);
  lcd.setCursor(9, 1);
  lcd.print("A");

   lcd.setCursor(12, 1);
  lcd.print("V = ");
  lcd.setCursor(16, 1);
  lcd.print(Va);
  lcd.setCursor(19, 1);
  lcd.print("V");

  delay(100);

   lcd.setCursor(12, 1);
  lcd.print("Va = ");
  lcd.setCursor(16, 1);
  lcd.print(220);
  lcd.setCursor(19, 1);
  lcd.print("V");
 lcd.setCursor(0, 2);
  lcd.print("S = ");
  lcd.setCursor(4, 2);
  lcd.print(S);
  lcd.setCursor(8, 2);
  lcd.print("KVA");

   lcd.setCursor(11, 2);
  lcd.print("f = ");
  lcd.setCursor(15, 2);
  lcd.print(50);
  lcd.setCursor(18, 2);
  lcd.print("Hz");

   lcd.setCursor(0, 3);
  lcd.print("Servo Angle ");
  lcd.setCursor(12, 3);
  lcd.print(val);
  lcd.setCursor(15, 3);
  lcd.print("deg");


    delay(500); 
      lcd.clear();
}
With Battery Backup and Stepper Motor as Generator
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
LiquidCrystal_I2C lcd(0x20, 20, 4);
int val;
int pos = 75;
float i = 0, watt,fire=0;
float voltage;

const int currentPin = A1;
int sensitivity = 66;
int adcValue = 0;
int offsetVoltage = 2472;
double adcVoltage = 0;
int currentValue = 0;
void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  lcd.backlight();
  lcd.init();

  myservo.attach(9);
  pinMode(A0, INPUT); // Generator Feedback
  pinMode(A1, INPUT); // CT

  pinMode(8, INPUT);   // smoke sensor
  pinMode(11, OUTPUT); // fire alarm buzzer
  pinMode(13, OUTPUT); // MOTOR relay
}

void loop() {
  if (digitalRead(8) == HIGH)   // if FIRE detected
  {
    fire++;
    digitalWrite(13, HIGH);
    digitalWrite(11, LOW);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    Alert....!!!!   ");
    digitalWrite(11, HIGH);
    delay(300);
  }

  if (fire == 0)
  {
    if (i == 0)                              // INITIALIZE
    { myservo.write(75);
      digitalWrite(13, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("        Smart       ");
      lcd.setCursor(0, 2);
      lcd.print("  Hydropower Plant  ");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" UET Taxila ");
      lcd.setCursor(0, 2);
      lcd.print("           ");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(" Starting Turbine...");
      myservo.write(75);
      digitalWrite(13, LOW);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Hydropower Plant  ");
      lcd.setCursor(0, 1);
      lcd.print("Frequency    =");
      lcd.setCursor(0, 2);
      lcd.print("Current      =");
      lcd.setCursor(0, 3);
      lcd.print("Power        =");
      i++;
    }
    int sensorValue = analogRead(A0);                            // Reading voltage from generator
    voltage = sensorValue * (5.0 / 1023.0) * 10;

    adcValue = analogRead(currentPin);                           // reading current sensor
    adcVoltage = (adcValue / 1024.0) * 5000;
    currentValue = ((adcVoltage - offsetVoltage) / sensitivity);
    watt = currentValue * 12;
    
    int  f = map(pos, 75, 160, 49.5, 50.5 );                      // frequency mapping with servo angle
    lcd.setCursor (16, 1); 
    lcd.print     (f);
    lcd.setCursor (16, 2);
    lcd.print  (currentValue, 1);
    lcd.setCursor (16, 3);
    lcd.print  (watt, 1);
    Serial.println(voltage);
    Serial.println(pos);
    Serial.println("-----------");
    Serial.print("Current = ");
    Serial.println(currentValue);
    Serial.println("-----------");
    if (pos < 160 && voltage < 12.5)                   
    { myservo.write(pos);
      delay(100);
      pos++;
    }

    if (pos > 70 && voltage > 12.5)
    { myservo.write(pos);
      delay(100);
      pos--;
    }

    delay(500)
  }
}

