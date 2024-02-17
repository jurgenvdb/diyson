/*
 * LED breathing light sample
 */
const int ledPin = 10;  // Actually output pin after PWM generation

//Set PWM parameter
const int freq = 5000;//PWM frequency
const int ledChannel = 0;//GPIO for signal generation
const int resolution = 8;//8-bit resolution

void setup(){
  //PWM parameter setting
  ledcSetup(ledChannel, freq, resolution);

  //Attach the signal generation channel to the output channel
  ledcAttachPin(ledPin, ledChannel);
}

void loop(){
  //Start to brighten
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }

  //Start to dim
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);   
    delay(15);
  }
}