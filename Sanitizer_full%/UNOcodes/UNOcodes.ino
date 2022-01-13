#include <LiquidCrystal.h>;
#include <Servo.h>

//::::::::: Variable Declaration :::::::://
long duration;
int distance_us1;
int distance_us2;
const int minHandDistance=12;
const int unitDividor=10;                       //if used in real project, keep unitDividor = 1

//::::::::::::: 16*2 LCD :::::::::::::::://
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//::::::::::: UltraSonic 1 :::::::::::::://
const int us1_triggerPin = 9;                   // Trigger Pin of Ultrasonic Sensor
const int us1_echoPin = 8;                      // Echo Pin of Ultrasonic Sensor

//::::::::::: UltraSonic 2 :::::::::::::://
const int us2_triggerPin=11;                    // defines the pin numbers
const int us2_echoPin=10;

//::::::::::: Servo Motor :::::::::::::://
const int servoPin = 12;

//:::::::::::::  Buzzer :::::::::::::::://
const int buzzerPin = 13;

////////////////////////////////////////////////////////////////



Servo Myservo;


void setup()
{
    Serial.begin(9600);                         // Starting Serial Terminal
    lcd.begin (16,2);                           // Initializes the interface to the LCD screen and specifies the dimensions of the display

    //::::::::::: UltraSonic 1 :::::::::::::://
    pinMode(us1_triggerPin, OUTPUT);
    pinMode(us1_echoPin, INPUT);

    //::::::::::: UltraSonic 2 :::::::::::::://
    pinMode (us2_triggerPin, OUTPUT);
    pinMode (us2_echoPin, INPUT);

    //::::::::::: Servo Motor :::::::::::::://
    Myservo.attach(servoPin);



    //Serial.println("Automated Sanitizer Dispenser.");
    //delay(500);
    //Serial.println("Made By:");
    //delay(200);
    //Serial.println("Mainul Islam");
    //delay(200);
    //Serial.println("Adnan Saky");
    //delay(200);
    //Serial.println("Julfikar Haque");
    //Serial.println("-------------------");
    //Serial.println("-------------------");
    //delay(1000);
}

void loop()
{
    //::::::::::::::::::::::::::::::::::::::://
    //::::::::::: UltraSonic 2 :::::::::::::://
    //For Measuring Liquid distance from the top of the container
    digitalWrite(us2_triggerPin,LOW);           // Clears the trigger Pin
    delayMicroseconds(2);
    digitalWrite(us2_triggerPin,HIGH);
    delayMicroseconds(10);                      // Sets the trigger Pin on HIGH state for 10 micro seconds
    digitalWrite(us2_triggerPin,LOW);

    duration = pulseIn(us2_echoPin, HIGH);      //To receive the reflected signal.

    //distanceInch = duration*0.0133/2;
    distance_us2= duration*0.0340/2;            // Calculating the distance
    distance_us2 = distance_us2/unitDividor;

    Serial.print("Liquid: ");
    Serial.print(distance_us2);
    Serial.print("Cm");
    Serial.println();
    delay(100);
    
    if(distance_us2 <=99 )
    {
        lcd.clear();
        lcd.setCursor(0,0);                   // Sets the location where text written to the LCD will be displayed
        lcd.print("Liquid:");                 // Prints string "Distance" on the LCD
        lcd.print(100 - distance_us2);        // Prints the distance value from the sensor
        lcd.print("%");
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("BOTTLE EMPTY");
        delay(300);
        lcd.setCursor(0,1);
        lcd.print("PLEASE REFILL");
    }

    //::::::::::::::::::::::::::::::::::::::://
    //::::::::::: UltraSonic 1 :::::::::::::://
    //For Measuring Hand Distance
    pinMode(us1_triggerPin, OUTPUT);
    digitalWrite(us1_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(us1_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(us1_triggerPin, LOW);

    pinMode(us1_echoPin, INPUT);
    duration = pulseIn(us1_echoPin, HIGH);
    distance_us1 = duration*0.0340/2;           // Calculating the distance
    distance_us1 = distance_us1/unitDividor;

    Serial.print("Hand: ");
    Serial.print(distance_us1);
    Serial.print("Cm");

    if(distance_us1 < minHandDistance && distance_us2 <=99)
    {
        Myservo.write(180);
    }
    else if (distance_us1 < minHandDistance && distance_us2 > 99)
    {
        Myservo.write(90);
        tone(buzzerPin, 200);
        delay(500);
        noTone(buzzerPin);
        delay(500);
    }
    else
    {
        Myservo.write(90);
    }

    Serial.println();
    delay(100);
}
