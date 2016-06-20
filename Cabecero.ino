// David Ramos                                                              26 diciembre 2014
//Analog 3: microphone 
//PWM 3: light regulation

unsigned long t1,t2,t3,t4,t5;  //time flags
unsigned int f1,f2;            //flags
unsigned int pulsoDetectado,i;
unsigned int sensorValue, activated;
unsigned int analog_channel = 3;
unsigned int pwm_channel = 3;
unsigned int state;  // 1-->LIGHT ON; 0-->LIGHT OFF; 

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  f1=0;
  f2=0;
  pulsoDetectado=0;
  state=0;  // estado: luz apagada
  analogWrite(pwm_channel, 0);  //luz apagada
}

// the loop routine runs over and over again forever:
void loop() {
  
  algoritmo_encendido();
  
  if(activated==1){
    activated=0;     //reset flag
    if(state==0){           //encender luz
       for(i=0;i<256;i++){
         analogWrite(pwm_channel, i);
         delay(15);
         //Serial.println(i); //debug
       }
       state=1; //luz encendida
    }else{                  //apagar luz
       for(i=256;i>0;i--){
         analogWrite(pwm_channel, i-1);
         delay(10);
         //Serial.println(i-1);  //debug
       }
       state=0; //luz apagada 
    }
    
  }

  delay(1);
}

void algoritmo_encendido() {

  if(pulsoDetectado==0){
    sensorValue = analogRead(analog_channel);
    if(sensorValue<700){
      if(f1==0){ t1 = millis(); f1 = 1; }
      t2 = millis();
      if((t2-t1)>10){ pulsoDetectado=1; blink_(); Serial.println("Pulso detectado!!"); }
    }else if(sensorValue>700){ f1 = 0; }
  
  }else if(pulsoDetectado==1){
    t3 = millis();
    if((t3-t1)>200){
      sensorValue = analogRead(analog_channel);
      if(sensorValue<700){
        if(f2==0){ t4 = millis(); f2 = 1; }
          t5 = millis();
        if((t5-t4)>10){ activated=1; pulsoDetectado=0; f1=0; f2=0; blink_(); Serial.println("ACTIVATION!!");}
      }
    }
    if((t3-t1)>400){pulsoDetectado=0; f1=0; f2=0;}
  }
  
  // print out the value you read:
  //Serial.println(sensorValue);  //debug

}

void blink_() {
  if (state==0){
    digitalWrite(pwm_channel, HIGH);
    delay(3);
    digitalWrite(pwm_channel, LOW);
  }else{
    digitalWrite(pwm_channel, LOW);
    delay(3);
    digitalWrite(pwm_channel, HIGH);
  }
}
