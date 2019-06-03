/*
 * Neighing mouse - v1 - 02/06/2019
 * Arduino Mini 16mhz
 * 
 * http://www.gianlucagallo.com/portfolio/201906/heigh-mouse
 * http://www.gianlucagallo.com/portfolio/201802/moo-mouse
 * 
 */
#define MOUSE_PIN 9
#define OUT1_PIN 10
#define OUT2_PIN 11
#define OUT3_PIN 12
unsigned int buttonState, val;

unsigned long counter1, counter2;
unsigned long counter1state, counter2state, counter3state;
#define OUT1_THRESHOLD 1000
#define OUT2_THRESHOLD 500
#define OUT3_PROBABILITY 1000
#define OUT3_PROBABILITY_CHECK (int)(OUT3_PROBABILITY/2)
#define OUT_TIME_MS 2400 //time for three neigh


void setup() {
   //Serial.begin(9600);
   pinMode(MOUSE_PIN, INPUT);
   pinMode(OUT1_PIN, OUTPUT);
   pinMode(OUT2_PIN, OUTPUT);
   pinMode(OUT3_PIN, OUTPUT);
   buttonState =  HIGH;
   counter1 = 0;
   counter2 = 0;
   counter1state = 0;
   counter2state = 0;
   counter3state = 0;
   randomSeed(analogRead(0)); //init rand
}

void loop() {
  unsigned long currentTime = millis();  
  //count button press
  val = digitalRead(MOUSE_PIN);
  if (val != buttonState) {         
     if (val == LOW) {  
       while (digitalRead(MOUSE_PIN) == LOW){
          delay(80); //waiting finger up, skip double click
       }
       //Serial.println("click");             
       ++counter1;
       ++counter2;

       //check for probability
       if (random(OUT3_PROBABILITY) == OUT3_PROBABILITY_CHECK && counter3state == 0) {
          digitalWrite(OUT3_PIN,HIGH);
          counter3state = currentTime;
          //Serial.println("OUT3 HIGH");    
       }
     }
  }
  buttonState = val; 

  //check if some out needs counter exeded threshold
  if (counter1 > OUT1_THRESHOLD && counter1state == 0) {
    digitalWrite(OUT1_PIN,HIGH);
    counter1state = currentTime;
    //Serial.println("OUT1 HIGH");   
  }  
  if (counter2 > OUT2_THRESHOLD && counter2state == 0) {
    digitalWrite(OUT2_PIN,HIGH);
    counter2state = currentTime;
    //Serial.println("OUT2 HIGH");   
  }

  //shutdown high output
  if (counter1state > 0 && currentTime - counter1state > OUT_TIME_MS) {
    digitalWrite(OUT1_PIN,LOW);
    counter1state = 0;
    counter1 = 0;
    //Serial.println("OUT1 LOW");
  }
  if (counter2state > 0 && currentTime - counter2state > OUT_TIME_MS) {
    digitalWrite(OUT2_PIN,LOW);
    counter2state = 0;
    counter2 = 0;
    //Serial.println("OUT2 LOW");
  }
  if (counter3state > 0 && currentTime - counter3state > OUT_TIME_MS) {
    digitalWrite(OUT3_PIN,LOW);
    counter3state = 0;
    //Serial.println("OUT3 LOW");
  }
}
