#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

#include <SPI.h>
#include <SD.h>

#include <DS1307.h>
#include <rgb_lcd.h>

/* Assign a unique ID to the sensors */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

const int chipSelectSD = 53;
int printCounter = 0;
const int printCountMax = 25;
long timebase = 0;

const int buttonPin = 2;    // the number of the pushbutton pin

// Variables will change:
int writeState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

DS1307 clock;//define a object of DS1307 class

/*Function: Display time on the serial monitor*/
void printTime()
{
  clock.getTime();
  Serial.print(clock.hour, DEC);
  Serial.print(":");
  Serial.print(clock.minute, DEC);
  Serial.print(":");
  Serial.print(clock.second, DEC);
  Serial.print("  ");
  Serial.print(clock.month, DEC);
  Serial.print("/");
  Serial.print(clock.dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(clock.year+2000, DEC);
  Serial.print(" ");
  Serial.print(clock.dayOfMonth);
  Serial.print("*");
  switch (clock.dayOfWeek)// Friendly printout the weekday
  {
    case MON:
      Serial.print("MON");
      break;
    case TUE:
      Serial.print("TUE");
      break;
    case WED:
      Serial.print("WED");
      break;
    case THU:
      Serial.print("THU");
      break;
    case FRI:
      Serial.print("FRI");
      break;
    case SAT:
      Serial.print("SAT");
      break;
    case SUN:
      Serial.print("SUN");
      break;
  }
  Serial.println(" ");
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  
  accel.getSensor(&sensor);
  Serial.println(F("----------- ACCELEROMETER ----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" m/s^2"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  gyro.getSensor(&sensor);
  Serial.println(F("------------- GYROSCOPE -----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" rad/s"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  
  mag.getSensor(&sensor);
  Serial.println(F("----------- MAGNETOMETER -----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" uT"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" uT"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" uT"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  bmp.getSensor(&sensor);
  Serial.println(F("-------- PRESSURE/ALTITUDE ---------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" hPa"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" hPa"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" hPa"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
     
  delay(500);
}

void setup(void)
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println(F("Adafruit 10DOF data logger")); Serial.println("");
  
  /* Initialise the sensors */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  // Setup the SD Card
  /* Display some basic information on this sensor */
  displaySensorDetails();

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelectSD)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");  

  if(SD.exists("datalog.txt")) {
    Serial.println("Removing existing old datalog file.");  
    SD.remove("datalog.txt");
  }

  //Setup trigger buttons
  pinMode(buttonPin, INPUT);

  clock.begin();
  printTime();
  
  Serial.println("Setup finished.");

  Serial.println("Waiting for write button trigger.");  
}

void printDataToSerial(sensors_event_t acc_event, 
                       sensors_event_t mg_event,
                       sensors_event_t gyro_event)
{
  printTime();
  
  Serial.print(F("ACCEL "));
  Serial.print("X: "); Serial.print(acc_event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(acc_event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(acc_event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  Serial.print(F("MAG   "));
  Serial.print("X: "); Serial.print(mg_event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(mg_event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(mg_event.magnetic.z); Serial.print("  ");Serial.println("uT");

  Serial.print(F("GYRO  "));
  Serial.print("X: "); Serial.print(gyro_event.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(gyro_event.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(gyro_event.gyro.z); Serial.print("  ");Serial.println("rad/s ");  

  Serial.println(F(""));
}

String GenerateDataString(sensors_event_t acc_event, 
                       sensors_event_t mg_event,
                       sensors_event_t gyro_event)
{
  String data = String(timebase) + ", ";

  clock.getTime();
  data += String((int) clock.month);
  data += "/";
  data += String((int) clock.dayOfMonth);
  data += "/";
  data += String((int) clock.year+2000);
  data += " ";
  data += String((int) clock.hour);
  data += ":";
  data += String((int) clock.minute);
  data += ":";
  data += String((int) clock.second);

  data += (", " + String(acc_event.acceleration.x));
  data += (", " + String(acc_event.acceleration.y));
  data += (", " + String(acc_event.acceleration.z));

  data += (", " + String(mg_event.acceleration.x));
  data += (", " + String(mg_event.acceleration.y));
  data += (", " + String(mg_event.acceleration.z));

  data += (", " + String(gyro_event.acceleration.x));
  data += (", " + String(gyro_event.acceleration.y));
  data += (", " + String(gyro_event.acceleration.z));

  return data;
}

void checkButton()
{
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        writeState = !writeState;
        if(writeState) {
          Serial.println("Resuming data writes.");        
        }
        else {
          Serial.println("Pausing data writes.");        
        }
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void GrabSensorState() {
  /* Get a new sensor event */
   
  /* Display the results (acceleration is measured in m/s^2) */
  sensors_event_t acc_event;
  accel.getEvent(&acc_event);

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  sensors_event_t mg_event;
  mag.getEvent(&mg_event);

  /* Display the results (gyrocope values in rad/s) */
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event);

  if(printCounter > printCountMax) {
    printDataToSerial(acc_event, mg_event, gyro_event);
    printCounter = 0;
  }
  printCounter++;

  String dataString = GenerateDataString(acc_event, mg_event, gyro_event);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }  
}

void loop(void)
{
  checkButton();

  if(writeState) {
    GrabSensorState();
  }
  
  delay(100);
  timebase++;
}
