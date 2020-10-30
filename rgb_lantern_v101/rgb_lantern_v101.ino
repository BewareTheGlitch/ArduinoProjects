/*9.5v @ 1a
 * 
 * 
 */  
  //Assigns buttons and light colors to pins.
  const int btn1 = 7;
  const int btn2 = 8;
  const int white = 6;
  const int blue = 9;
  const int red = 5;
  const int off = 100;
  const int green = 10;
  const int pilotBlue = 2;
  const int pilotRed = 12;
  const int pilotGreen = 13;
  const int delay1 = 350;
  const int delay2 = 90;

  //Arrays that store the PWM values for each light color
  const int brightWhite[9] = {0, 2, 8, 16, 32, 64, 128, 192, 255};
  const int brightRed[9] = {0, 2, 8, 16, 32, 64, 128, 192, 255};
  const int brightBlue[9] = {0, 2, 8, 16, 32, 64, 128, 192, 255};
  const int brightGreen[9] = {0, 2, 8, 16, 32, 64, 128, 192, 255};

  //Array that maps pins to light colors
  const int color[5] = {6, 9, 5, 10, 100};

  //Vars that store the selected brightness of each color
  int brightChangeRed = 0;
  int brightChangeGrn = 0;
  int brightChangeBlu = 0;
  int brightChangeWht = 0;
  
  //Stores current selected color
  int currentColor = white;

  //Pointers for the PWM array values for each color
  int briPointerRed = 0;
  int briPointerGrn = 0;
  int briPointerBlu = 0;
  int briPointerWht = 0;

  //Pointer for the color select array.
  int colPointer = white;
  
  //Stores the buttons' states 
  int readBtn1 = HIGH;
  int readBtn2 = HIGH;
  
  
  
void setup() {
  // put your setup code here, to run once:
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(white, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(pilotBlue, OUTPUT);
  pinMode(pilotRed, OUTPUT);
  pinMode(pilotGreen, OUTPUT);
  digitalWrite(pilotBlue, LOW);
  digitalWrite(pilotRed, LOW);
  digitalWrite(pilotGreen, LOW);
  
  //serial console for debugging. Comment these lines out to disable.
  Serial.begin(9600); 
  Serial.println ("Startup.");
  Serial.print ("Selected Color: "); Serial.println (currentColor);
  Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.print(briPointerGrn); 
  
}

void loop() {
  // put your main code here, to run repeatedly:


  
  readBtn2 = digitalRead(btn2);       //Change the selected color when button 2 is pressed.
  if (readBtn2 == LOW){ 
    delay(50);
    colPointer++;
    
    if (colPointer >= 5){
      colPointer = 0;
    }
   
    currentColor = color[colPointer]; 
    
    if (currentColor == white){       //Set the pilot light to show the selected color.
      digitalWrite(pilotBlue, LOW);
      digitalWrite(pilotRed, LOW);
      digitalWrite(pilotGreen, LOW);   
      Serial.print ("Selected Color: "); Serial.println (currentColor);
      Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  
    }
    
    if (currentColor == red){
      digitalWrite(pilotBlue, HIGH);
      digitalWrite(pilotRed, LOW);
      digitalWrite(pilotGreen, HIGH);  
      Serial.print ("Selected Color: "); Serial.println (currentColor);
      Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);   
    }
    
    if (currentColor == blue){
      digitalWrite(pilotBlue, LOW);
      digitalWrite(pilotRed, HIGH);
      digitalWrite(pilotGreen, HIGH);   
      Serial.print ("Selected Color: "); Serial.println (currentColor);
      Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  
    }
    
    if (currentColor == green){
      digitalWrite(pilotBlue, HIGH);
      digitalWrite(pilotRed, HIGH);
      digitalWrite(pilotGreen, LOW);    
      Serial.print ("Selected Color: "); Serial.println (currentColor);
      Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn); 
    }
    
    if (currentColor == off){
      digitalWrite(pilotBlue, HIGH);
      digitalWrite(pilotRed, HIGH);
      digitalWrite(pilotGreen, HIGH); 
      Serial.print ("Selected Color: "); Serial.println (currentColor);
      Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);    
    }
      
    delay(delay1);
  }



  

  readBtn1 = digitalRead(btn1);      //Change the selected color's brightness when button 1 is pressed.
  if (readBtn1 == LOW){ 
    delay(50); 
    
    if (currentColor == white){
      briPointerWht++;
      if (briPointerWht == 8){
        digitalWrite(pilotBlue, HIGH);       //Blinks the pilot LED when brightness is 100% 
        digitalWrite(pilotRed, HIGH);
        digitalWrite(pilotGreen, HIGH);    
        delay(delay2);
        digitalWrite(pilotBlue, LOW);
        digitalWrite(pilotRed, LOW);
        digitalWrite(pilotGreen, LOW);        
      }
      if (briPointerWht >= 9){
        briPointerWht = 0;
      }
      
    brightChangeWht = brightWhite[briPointerWht]; 
    analogWrite (currentColor, brightChangeWht);
          Serial.print ("Selected Color: "); Serial.println (currentColor);
          Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  

    delay(delay1);   
    }
   
    if (currentColor == red){
      briPointerRed++;
      if (briPointerRed == 8){        
        digitalWrite(pilotRed, HIGH);        
        delay(delay2);        
        digitalWrite(pilotRed, LOW);  
      }      
      if (briPointerRed >= 9){
        briPointerRed = 0;
      }
      
    brightChangeRed = brightRed[briPointerRed]; 
    analogWrite (currentColor, brightChangeRed);
          Serial.print ("Selected Color: "); Serial.println (currentColor);
          Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  

    delay(delay1);   
    }
  
    if (currentColor == blue){
      briPointerBlu++;
      if (briPointerBlu == 8){        
        digitalWrite(pilotBlue, HIGH);       
        delay(delay2);        
        digitalWrite(pilotBlue, LOW);  
      }          
      if (briPointerBlu >= 9){
        briPointerBlu = 0;
      }
      
    brightChangeBlu = brightBlue[briPointerBlu]; 
    analogWrite (currentColor, brightChangeBlu);
          Serial.print ("Selected Color: "); Serial.println (currentColor);
          Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  

    delay(delay1);   
    }
   
    if (currentColor == green){
      briPointerGrn++;
      if (briPointerGrn == 8){        
        digitalWrite(pilotGreen, HIGH);        
        delay(delay2);        
        digitalWrite(pilotGreen, LOW);  
      }          
      if (briPointerGrn >= 9){
        briPointerGrn = 0;
      }
      
    brightChangeGrn = brightGreen[briPointerGrn]; 
    analogWrite (currentColor, brightChangeGrn);
          Serial.print ("Selected Color: "); Serial.println (currentColor);
          Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  

    delay(delay1);   
    }
    if (currentColor == off){ //Turn everything off and set color pointers back to 0
      analogWrite (white, 0);
      analogWrite (red, 0);
      analogWrite (green, 0);
      analogWrite (blue, 0);
      briPointerWht = 0;
      briPointerRed = 0;
      briPointerGrn = 0;
      briPointerBlu = 0;
            Serial.print ("Selected Color: "); Serial.println (currentColor);
            Serial.print ("Brightness W:"); Serial.print(briPointerWht); Serial.print (" B:"); Serial.print(briPointerBlu); Serial.print (" R:"); Serial.print(briPointerRed); Serial.print (" G:"); Serial.println(brightChangeGrn);  
    }
       
  }
    
}  
