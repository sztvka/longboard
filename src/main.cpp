#include <Arduino.h>
#include <PWM.hpp> // biblioteka do czytania pwm 
#include <Servo.h> //biblioteka do wypisywania pwm. uzywana dla serw ale tutaj tez dziala
PWM my_pwm(2); // Setup pin 2 for PWM
Servo ESC; //deklaracja naszego ESC przy uzyciu biblioteki 
unsigned int power = 0; //deklaracja zmiennej przechowujacej wartosc podawanej mocy
boolean failsafe = false; //deklaracja zmiennej failsafe

void setup() {
   // Serial.begin(115200); // Serial for debug
    my_pwm.begin(true); // PWM on pin 2 reading PWM HIGH duration
    ESC.attach(3,1000,2000); //zalaczenie pinu D3 jako wyjscie dla esc
    pinMode(4, OUTPUT); //ustawienie pinu D4 jako wyjsciowy, jest pod mosfety
    digitalWrite(4, LOW); //upewnienie sie, ze nie podaje napiecia

}

void loop() {
  if(float(my_pwm.getAge()/1000) > 500){ //jesli ostatni sygnal starszy niz 500ms
    failsafe = true; 
    ESC.write(0); //ustawianie mocy na esc
  }
  else { //jesli jest mlodszy niz to
    failsafe = false;
  }
 if(!failsafe){ //wykonuj tylko jesli failsafe = false
  delay(40); //delay zeby przekazniki nie dostaly pierdolca, bo przy jakis oscylacjach radiowych moglyby przelaczac stan kilka razy przed pelnym przesterowaniem
     if(my_pwm.getValue()<1150) { //jesli pwm < 1150
      //Serial.println("dobrze brek");
      ESC.write(0);
      digitalWrite(4, HIGH); //brek on
    }
    else if (my_pwm.getValue()<1470) { //jesli pwm < 1470
     // Serial.println("wsteczny");
      ESC.write(0);
      digitalWrite(4, LOW); //brek off
    }
    else { //reszta przypadkow, czyli gaz ponad ten jalowy
    digitalWrite(4, LOW);
    power = (my_pwm.getValue()-1470)/2.2 ; //przeskalowanie zakresu pwm na liczby 0-180
      if(power<5){ //deadzone
        ESC.write(0);
      }
      else{ //jesli wiekszy niz deadzone
        ESC.write(power);
      }
    
    
 /* tutaj byl debug wyjebane w to
    Serial.print("Value: ");
    Serial.print(power);
    Serial.print("\tAGE: ");
    Serial.println(float(my_pwm.getAge()/1000)); //przeskalowane na ms, ustaw failsafe względem tego
 */


        }
}
 }
 
