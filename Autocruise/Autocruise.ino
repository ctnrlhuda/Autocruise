#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

unsigned int counter = 0;   //set counter to 0
int potentiometer = A0;    // select the input pin for the potentiometer
int potentiometerValue = 0;  // variable to store the value coming from the sensor
int speedValue = 0;     // value of speed for motor moves and display from lcd
const int EN_A = 9;   // motor pin connection
int sensor = 2;      // speed sensor connection
unsigned long starttime = 0;//set startime to 0
unsigned long endtime = 0;//set endtime to 0
int steps = 0;//set steps to 0
int stepsold = 0;//set stepsold to 0
int temp = 0;//set temp to 0
int rps = 0;//set rps to 0
const int IR = 7;//IR sensor connection
const int buzzer = 12;//buzzer connection
const int buttonPin = 10 ; //pushbutton connection
int buttonState = 0; //set buttoState to 0
int i=0;//set i to 0

void forward(int pwmLeft)//function for motor to move forward
{   analogWrite (EN_A, pwmLeft);//send input from pwmLeft to EN_A
   delay(100);//delay time for 0.1 second
    
}


void displaylcd(int rotation)//Function to display lcd. receive rotation from rps
{ lcd.setCursor(0,1);//set position to start display
  lcd.print(rotation);// lcd display value rotation
  
}

void obstacle()//function for alarm 
{  int irState = digitalRead (IR);//IR value transferred to iRstate
   if (irState == 1) // If sensor detect obstacle
    digitalWrite (buzzer, LOW);//alarm activated
   else
   {
    digitalWrite (buzzer, HIGH);// alarm not activated
    
      int newspeed= speedValue-100;//speed decrease by 100 and send to newspeed
      forward(newspeed);//send the new speed to motor
      Serial.println("speed decreased...."); // print at serial monitor
      Serial.println(newspeed);//print the new speed
    }
}
void autocruise (int x)// function for auto cruise mode. receive x from speedvalue
{
  Serial.println(x);//print x at serial monitor
  forward(x);//send x to forward function 
  delay(100);// delay 0.1 sec  
}
void manual()// function for manual mode
{
   potentiometerValue = analogRead(potentiometer);//transfer data from potentiometer to potentiometerValue
   speedValue = map(potentiometerValue, 0, 1023, 0, 255);  //set min and max value for speedValue
   Serial.println(speedValue);//print speedValue at serial monitor
   delay(500);//delay 0.5 sec
   forward (speedValue);//send speedValue to forward function
}


void setup() 
{
  Serial.begin(9600);//set baudrate
  pinMode(buttonPin, INPUT);//set buttonPin as input 
  pinMode(potentiometer, INPUT);// setup potentiometer as an INPUT
  pinMode (EN_A, OUTPUT);//setup for motor as an OUTPUT
  lcd.init();  // initialize the lcd 
  lcd.backlight();// to turn on backlight
  pinMode (sensor, INPUT);// setup speed sensor as an INPUT
  pinMode(IR, INPUT);// setup IR as an INPUT
  pinMode(buzzer, OUTPUT);// setup buzzer as an OUTPUT
}

void loop() {
  // put your main code here, to run repeatedly:
 buttonState =digitalRead(buttonPin);//transfer data from buttonPin to buttonState
 if(buttonState == HIGH)//Check condition of buttonState if push button is pressed
 {
   Serial.println(buttonState); //print buttonState at serial monitor
   i=i+1;//increase i by 1
 }
    if((i%2)==1)//check condition if i is even
    {
      Serial.println("auto mode ..... ");//print at serial monitor
     autocruise(speedValue);//call autocruise function and send speedValue 
    }
 else
 {
  Serial.println("manual mode....");//print at serial monitor
  manual();//call function manual
  
 }
 
  while (millis()<endtime)//check if millisecond less than endtime
  { if(digitalRead(sensor))//check if input of speed sensor equal to 1
    { steps = steps+1;//steps increase by 1
      while(digitalRead(sensor));// continue run if input equal to 1
    }
  }
   temp = steps - stepsold;
   stepsold = steps;
   rps = (temp/2);//transfer current speed value to rps
   displaylcd(rps); //display rps at lcd
   delay(100);//delay 0.1 sec
   forward (speedValue);//call function forward and send speed value 
   obstacle();    // detect obstacle

   starttime=millis();// return the number of millisecond to startTime
   endtime = starttime + 1000;//return to endtime
   lcd.clear();// clear screen
  
}
