#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include<LiquidCrystal.h>
//Define LCD pins
LiquidCrystal lcd (7,6,5,4,3,2); //RS, E, D4, D5, D6, D7

SemaphoreHandle_t sem; // handler for semaphore

TaskHandle_t ADCtask;   // task handlers
void Task1 (void *pvParameters);

TaskHandle_t Ultrasonictask;
void Task2 (void *pvParameters); 

void setup() 
{
  Serial.begin(9600); // Enable serial communication library.

  lcd.begin(16, 2);  // Enable LCD library

 // Create tasks
   xTaskCreate(Task1, // Task function
              "ADC", // Task name
              128, // Stack size 
              NULL, 
              0, // Priority
              &ADCtask );
              
   xTaskCreate(Task2, // Task function
              "Ultrasonic", // Task name
              128, // Stack size 
              NULL, 
              0, // Priority
              &Ultrasonictask);   
               
 sem = xSemaphoreCreateCounting(1,1);
   vTaskStartScheduler();
}


void loop() {}

void Task1(void *pvParameters)
{ //adc task
  (void) pvParameters;
  for (;;) 
    { int adc; //variable to store adc value
       
      adc = analogRead(A0);  //Read adc value from A0 channel and store it in adc variable 
      xSemaphoreTake(sem, portMAX_DELAY);  // acquire the semaphore
      // dispay adc value 
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("ADC : ");
       lcd.setCursor(6, 0);
       lcd.print(adc);
       delay(800);
       lcd.clear();
       
      //print on the virtual terminal
      Serial.print("ADC : ");
      Serial.print(adc);    
      Serial.println();
  
      xSemaphoreGive(sem); // give the sem to Task2 which is the ultrasonic task
      vTaskDelay(1);
  }
}

void Task2(void *pvParameters)
{  //Ultrasonic
  (void) pvParameters;
  for (;;) 
    {
      
      long distance,duration; // variables for storing data
      pinMode(A1, OUTPUT);   //trigger pin
      pinMode(A2, INPUT);    //echo pin 

      digitalWrite(A1, LOW);
      delayMicroseconds(2);
      digitalWrite(A1, HIGH);
      delayMicroseconds(10);
      digitalWrite(A1, LOW);
  
  duration = pulseIn(A2, HIGH); // how long pin is high
  distance = (duration/2) /29.1; //the duration value is to be divided by 2, since the signal bounces back after sending it and multiplies by speed of sound
  xSemaphoreTake(sem, portMAX_DELAY); //acquire the semaphore
        lcd.setCursor(0,0);   //display it on the lcd
        lcd.print("Distance:");
        lcd.setCursor(10,0);
        lcd.print(distance); //output on lcd in centimeter
        lcd.println("cm");
        
        delay(800);
        lcd.clear();

        Serial.print("Distance:"); // display it on the virtual terminal
        Serial.print(distance);
        Serial.print("cm");
        Serial.println();

        xSemaphoreGive(sem); //release the semaphore
        vTaskDelay(1);
  }
}
