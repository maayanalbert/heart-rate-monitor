

//  Variables
int PulseSensorPurplePin = A5;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int LED13 = D7;   //  The on-board Arduion LED
const int numSpotsForEllapsedTimes = 100;

int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
// int Threshold = 1050; // Determine which Signal to "count as a beat", and which to ingore. 
const int numSpotsForAvg = 100;
int dataForAvg[numSpotsForAvg];
float avg;
float sum;
int ellapsedTime;
int startTime;
int lastSignal;
float dataForEllapsedTimes[numSpotsForEllapsedTimes];
float ellapsedTimeSum;
int BPM;
int heartBeats;
unsigned long nowTime;


// The SetUp Function:
void setup() {
  pinMode(LED13,OUTPUT);         // pin that will blink to your heartbeat!
   Serial.begin(9600);         // Set's up Serial Communication at certain speed.
   
  Signal = 0;
  ellapsedTime = 400;
  startTime = 0;
  heartBeats = 0;

   
}

void loop() {
 
 ////GET AVERAGE
    
  lastSignal = Signal;

  Signal = analogRead(PulseSensorPurplePin);  // Read the PulseSensor's value. 
                                              // Assign this value to the "Signal" variable.
   
  for(int i = 0; i < numSpotsForAvg; i++){
      dataForAvg[i] = dataForAvg[i + 1];
  }
  dataForAvg[numSpotsForAvg] = Signal;
   
  sum = 0;
  for(int i = 0; i < numSpotsForAvg; i++){
      sum += dataForAvg[i];
  }
  

  delay(10);


  
  
  ////TURN ON LIGHT
  if(Signal > sum/numSpotsForAvg){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.  
     digitalWrite(LED13,HIGH);          
  } else {
     digitalWrite(LED13,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
  }
  
  ////COUNT HEART BEATS
  if(lastSignal < 200+(sum/numSpotsForAvg) && Signal > 200+(sum/numSpotsForAvg)){
    //   Serial.println(1);
     unsigned long nowTime= millis();
      ellapsedTime = nowTime - startTime;
      startTime = nowTime;
      heartBeats += 1;
  }

  
  ///CALCULATE BPM
    for(int i = 0; i < numSpotsForEllapsedTimes; i++){
      dataForEllapsedTimes[i] = dataForEllapsedTimes[i + 1];
  }
  dataForEllapsedTimes[numSpotsForEllapsedTimes-1] = ellapsedTime;
  
   
  ellapsedTimeSum = 0;
  for(int i = 0; i < numSpotsForEllapsedTimes; i++){
      ellapsedTimeSum += dataForEllapsedTimes[i];
  }


   BPM = 60000/(ellapsedTimeSum/numSpotsForEllapsedTimes);
   
       Serial.println(BPM); 
   


    Particle.variable("bpm", BPM);
   
}




