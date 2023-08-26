#include "Adafruit_Fingerprint.h" //fingerprint library header file
#include "Adafruit_MLX90614.h" // Mlx90614 library header file
#include<EEPROM.h> //command for storing data
#include <Wire.h>// library for I2C communication
#include<LiquidCrystal_I2C.h> //lcd header file
#include <SoftwareSerial.h> // software serial header file
SoftwareSerial fingerPrint(10,11); //for tx/rx communication between arduino & r305 fingerprint sensor

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

double temp_amb; //ambient temperature should read as double
double temp_obj;// object temperature should read as double
 
#include <Wire.h> // wire library header file
#include "RTClib.h" //library file for DS3231 RTC Module
RTC_DS3231 rtc;
 
uint8_t id; //unsigned 8-bit integer type
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
 
#define register_back A0 // declearing pin 14 as register_back
#define delete_ok A1 // declearing pin 15 as delete_ok
#define forward A2 // declearing pin 16 as forward
#define reverse A3 // declearing pin 17 as reverse
#define match 5 // declearing pin 5 as match
#define indFinger 7 // declearing pin 7 as indfinger
#define buzzer 5 // declearing pin 5 as buzzer

#define records 25 // 25 for 10 user
 
int user1,user2,user3,user4,user5,user6,user7,user8,user9,user10,user11,user12,user13,user14,user15,user16,user17,user18,user19,user20,user21,user22,user23,user24;// list of users
 
DateTime now;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,20,4) for 16x2 LCD.
 
void setup()
{
delay(1000);// delay for 1000ms
// Initiate the LCD:
  lcd.init();
  lcd.backlight();
  
Serial.begin(9600);
pinMode(register_back, INPUT_PULLUP);//register_back as input pullup(as input using push button)
pinMode(forward, INPUT_PULLUP);//forward as input pullup(as input using push button)
pinMode(reverse, INPUT_PULLUP);
pinMode(delete_ok, INPUT_PULLUP);
pinMode(match, INPUT_PULLUP);
pinMode(buzzer, OUTPUT);
pinMode(12,OUTPUT);
pinMode(indFinger, OUTPUT);
digitalWrite(buzzer, LOW);
if(digitalRead(register_back) == 0)// if register_back is compare to zero
{
digitalWrite(buzzer, HIGH);//buzzer should be high
delay(500);//delay for 500ms
digitalWrite(buzzer, LOW);//buzzer should be low
lcd.clear();// lcd should clear
lcd.print("Please wait !");// lcd shold display please wait
lcd.setCursor(0,1);// lcd should put please wait up and downloading data down
lcd.print("Downloding Data");// lcd should display downloading data

 
Serial.println("Please wait");// serial monitor to display please wait on first line
Serial.println("Downloding Data..");//serial monitor to display download data on second line
Serial.println();
 
Serial.print("S.No. ");//serial monitor to display S.NO
for(int i=0;i<records;i++) // int i should read from zero with an increment by 1
{
digitalWrite(buzzer, HIGH);
delay(500);
digitalWrite(buzzer, LOW);
Serial.print(" User ID");// serial monitor should print User ID
Serial.print(i+1);
Serial.print(" ");
}
Serial.println();
int eepIndex=0;
for(int i=0;i<30;i++)
{
if(i+1<24)
Serial.print('0');
Serial.print(i+1);
Serial.print(" ");
eepIndex=(i*7);
download(eepIndex);
eepIndex=(i*7)+210;
download(eepIndex);
eepIndex=(i*7)+420;
download(eepIndex);
eepIndex=(i*7)+630;
download(eepIndex);
eepIndex=(i*7)+840;
download(eepIndex);
eepIndex=(i*7)+1050;
download(eepIndex);
eepIndex=(i*7)+1260;
download(eepIndex);
eepIndex=(i*7)+1470;
download(eepIndex);
eepIndex=(i*7)+1680;
download(eepIndex);
eepIndex=(i*7)+1890;
download(eepIndex);
eepIndex=(i*7)+2100;
download(eepIndex);
eepIndex=(i*7)+2310;
download(eepIndex);
eepIndex=(i*7)+2520;
download(eepIndex);
eepIndex=(i*7)+2730;
download(eepIndex);
eepIndex=(i*7)+2940;
download(eepIndex);
eepIndex=(i*7)+3150;
download(eepIndex);
eepIndex=(i*7)+3360;
download(eepIndex);
eepIndex=(i*7)+3570;
download(eepIndex);
eepIndex=(i*7)+3780;
download(eepIndex);
eepIndex=(i*7)+3990;
download(eepIndex);
eepIndex=(i*7)+4200;
download(eepIndex);
eepIndex=(i*7)+4410;
download(eepIndex);
eepIndex=(i*7)+4620;
download(eepIndex);
Serial.println();
}
}
if(digitalRead(delete_ok) == 0)
{
lcd.clear();
lcd.print("Please Wait");
lcd.setCursor(0,1);
lcd.print("Reseting.....");
for(int i=1000;i<1012;i++)
EEPROM.write(i,0);// EEprom is to keep data in the memory  when you reset or the board is turn off, EEprom write i is zero
for(int i=0;i<2311;i++)
EEPROM.write(i, 0xff);
lcd.clear();
lcd.print("System Reset"); // lcd to display System Reset
delay(1000);
}
 lcd.clear();//lcd should clear
lcd.print("WELCOME TUNMISE ");//lcd display welcome tunmise
lcd.setCursor(0,1);// lcd should display welcome tunmise above and 18CE024931
lcd.print("  18CE024931  ");// lcd display Matric no
delay(2000);
lcd.clear();//lcd should clear
lcd.print("  Fingerprint  ");//lcd display fingerprint
lcd.setCursor(0,1);// lcd should display fingerprint above and Biometric below
lcd.print("  Biometric & ");// lcd display Biometric
delay(2000);//delay (Fingerprint Biometric) for 2000ms
lcd.clear();
lcd.print("  Temperature  ");
lcd.setCursor(0,1);
lcd.print("  Scanner.  ");
delay(2000);
lcd.clear();
 
digitalWrite(buzzer, HIGH);
delay(500);
digitalWrite(buzzer, LOW);
for(int i=1000;i<1000+records;i++)
{
if(EEPROM.read(i) == 0xff)
EEPROM.write(i,0);
}
 
finger.begin(57600);
Serial.begin(9600);
lcd.clear();
lcd.print("Finding Module..");// lcd display Finding Module
lcd.setCursor(0,1);
delay(2000);
if (finger.verifyPassword())
{
Serial.println("Found fingerprint sensor!");
lcd.clear();
lcd.print(" Module Found");
delay(2000);
}
else
{
Serial.println("Did not find fingerprint sensor :(");
lcd.clear();
lcd.print("Module Not Found");// lcd display Module Not Found
lcd.setCursor(0,1);
lcd.print("Check Connections");//lcd display check connections
while (1);
}
 
if (! rtc.begin())
Serial.println("Couldn't find RTC");
 
// rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 
if (rtc.lostPower())
{
Serial.println("RTC is NOT running!");
// following line sets the RTC to the date & time this sketch was compiled
rtc.adjust(DateTime(2022, 3, 7, 11, 0, 0));
// This line sets the RTC with an explicit date & time, for example to set
// June 7, 2018 at 11am you would call:
// rtc.adjust(DateTime(2022, 3, 7, 11, 0, 0));
}
lcd.setCursor(0,0);
lcd.print(" Press Match to ");
lcd.setCursor(0,1);
lcd.print(" Start System");
delay(3000);
 
user1=EEPROM.read(1000);// user 1 shold read  value 1000 on EEPROM
user2=EEPROM.read(1001);//user2 should read value 1001 on EEPROM
user3=EEPROM.read(1002);
user4=EEPROM.read(1003);
user5=EEPROM.read(1004);
user6=EEPROM.read(1005);
user7=EEPROM.read(1006);
user8=EEPROM.read(1007);
user9=EEPROM.read(1008);
user10=EEPROM.read(1009);
user11=EEPROM.read(1010);
user12=EEPROM.read(1011);

lcd.clear();
digitalWrite(indFinger, HIGH);
 Serial.println("Temperature sensor MLX90614");//serial monitor to display (Temperature sensor MLX90614

  mlx. begin(); // mlx shold begin
 
  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}
 
void loop()
{
now = rtc.now();
lcd.setCursor(0,0);
lcd.print("Time: ");
lcd.print(now.hour(), DEC);
lcd.print(':');
lcd.print(now.minute(), DEC);
lcd.print(':');
lcd.print(now.second(), DEC);
lcd.print(" ");
lcd.setCursor(0,1);
lcd.print("Date: ");
lcd.print(now.day(), DEC);
lcd.print('/');
lcd.print(now.month(), DEC);
lcd.print('/');
lcd.print(now.year(), DEC);
lcd.print(" ");
delay(500);
int result=getFingerprintIDez();
if(result>0)
{
digitalWrite(indFinger, LOW);
digitalWrite(buzzer, HIGH);
delay(100);
digitalWrite(buzzer, LOW);
lcd.clear();
lcd.print("ID:");
lcd.print(result);
lcd.setCursor(0,1);
lcd.print("Please Wait....");
delay(1000);
biometric(result);
lcd.clear();
lcd.print("  Biometric  ");
lcd.setCursor(0,1);
lcd.print("  Registered &  ");
delay(1000);
lcd.clear();
lcd.print("  Temperature  ");
lcd.setCursor(0,1);
lcd.print("  Scanned.");
delay(1000);
digitalWrite(indFinger, HIGH);
return;
}
checkKeys();
delay(300);
}
 
// dmyyhms - 7 bytes (date,month,year,hours,minutes,seconds)
void biometric(int id)
{
int user=0,eepLoc=0;
if(id == 1)
{
eepLoc=0;
user=user1++;// user should add by increment of 1
}
else if(id == 2)
{
eepLoc=210;
user=user2++;
}
else if(id == 3)
{
eepLoc=420;
user=user3++;
}
else if(id == 4)
{
eepLoc=630;
user=user4++;
}
else if(id == 5)
{
eepLoc=840;
user=user5++;
}
else if(id == 6)
{
eepLoc=1050;
user=user6++;
}
else if(id == 7)
{
eepLoc=1260;
user=user7++;
}
else if(id == 8)
{
eepLoc=1470;
user=user8++;
}
else if(id == 9)
{
eepLoc=1680;
user=user9++;
}
else if(id == 10)
{
eepLoc=1890;
user=user10++;
}
else if(id == 11)
{
  eepLoc =2100;
  user=user11++;
}
else if(id == 12)
{
  eepLoc =0;
  user=user12++;
}
else if(id == 13)
{
  eepLoc =2310;
  user=user13++;
}
else if(id == 14)
{
  eepLoc =2520;
  user=user14++;
}
else if(id == 15)
{
  eepLoc =2730;
  user=user14++;
}
else if(id == 16)
{
  eepLoc =2940;
  user=user16++;
}
else if(id == 17)
{
  eepLoc =2940;
  user=user17++;
}
else if(id == 18)
{
  eepLoc =3150;
  user=user18++;
}
else if(id == 19)
{
  eepLoc =3360;
  user=user19++;
}
else if(id == 20)
{
  eepLoc =3570;
  user=user20++;
}
else if(id == 21)
{
  eepLoc =3780;
  user=user21++;
}
else if(id == 22)
{
  eepLoc =3990;
  user=user22++;
}
else if(id == 23)
{
  eepLoc =4200;
  user=user23++;
}
else if(id == 24)
{
  eepLoc =4410;
  user=user24++;
}

/*else if(id == 5) // fifth user
{
eepLoc=840;
user=user5++;
}*/
else
return;
 
int eepIndex=(user*7)+eepLoc;
EEPROM.write(eepIndex++, now.hour());
EEPROM.write(eepIndex++, now.minute());
EEPROM.write(eepIndex++, now.second());
EEPROM.write(eepIndex++, now.day());
EEPROM.write(eepIndex++, now.month());
EEPROM.write(eepIndex++, now.year()>>8 );
EEPROM.write(eepIndex++, now.year());
 
EEPROM.write(1000,user1);
EEPROM.write(1001,user2);
EEPROM.write(1002,user3);
EEPROM.write(1003,user4);
EEPROM.write(1003,user4);
EEPROM.write(1004,user5);
EEPROM.write(1005,user6);
EEPROM.write(1006,user7);
EEPROM.write(1007,user8);
EEPROM.write(1008,user9);
EEPROM.write(1009,user10);
EEPROM.write(1010,user11);
EEPROM.write(1011,user12);
EEPROM.write(1012,user13);

// EEPROM.write(4,user5); // figth user
}
 
void checkKeys()
{
if(digitalRead(register_back) == 0)
{
lcd.clear();
lcd.print("Please Wait");
delay(1000);
while(digitalRead(register_back) == 0);
Enroll();
}
 
else if(digitalRead(delete_ok) == 0)
{
lcd.clear();
lcd.print("Please Wait");
delay(1000);
delet();
}
}
 
void Enroll()
{
int count=1;
lcd.clear();
lcd.print("Enter Finger ID:");
 
while(1)
{
lcd.setCursor(0,1);
lcd.print(count);
if(digitalRead(forward) == 0)
{
count++;
if(count>records)
count=1;
delay(500);
}
 
else if(digitalRead(reverse) == 0)
{
count--;
if(count<1)
count=records;
delay(500);
}
else if(digitalRead(delete_ok) == 0)
{
id=count;
getFingerprintEnroll();
for(int i=0;i<records;i++)
{
if(EEPROM.read(i) != 0xff)
{
EEPROM.write(i, id);
break;
}
}
return;
}
 
else if(digitalRead(register_back) == 0)
{
return;
}
}
}
 
void delet()
{
int count=1;
lcd.clear();
lcd.print("Enter Finger ID");
 
while(1)
{
lcd.setCursor(0,1);
lcd.print(count);
if(digitalRead(forward) == 0)
{
count++;
if(count>records)
count=1;
delay(500);
}
 
else if(digitalRead(reverse) == 0)
{
count--;
if(count<1)
count=records;
delay(500);
}
else if(digitalRead(delete_ok) == 0)
{
id=count;
deleteFingerprint(id);
for(int i=0;i<records;i++)
{
if(EEPROM.read(i) == id)
{
EEPROM.write(i, 0xff);
break;
}
}
return;
}
 
else if(digitalRead(register_back) == 0)
{
return;
}
}
}
 
uint8_t getFingerprintEnroll()
{
int p = -1;
lcd.clear();
lcd.print("finger ID:");
lcd.print(id);
lcd.setCursor(0,1);
lcd.print("Place Finger");
delay(2000);
while (p != FINGERPRINT_OK)
{
p = finger.getImage();
switch (p)
{
case FINGERPRINT_OK:
Serial.println("Image taken");
lcd.clear();
lcd.print("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.println("No Finger");
lcd.clear();
lcd.print("No Finger Found");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Comm Error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
lcd.clear();
lcd.print("Imaging Error");
break;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown Error");
break;
}
}
 
// OK success!
 
p = finger.image2Tz(1);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
lcd.clear();
lcd.print("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
lcd.clear();
lcd.print("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
lcd.clear();
lcd.print("Comm Error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Feature Not Found");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
lcd.clear();
lcd.print("Feature Not Found");
return p;
default:
Serial.println("Unknown error");
lcd.clear();
lcd.print("Unknown Error");
return p;
}
 
Serial.println("Remove finger");
lcd.clear();
lcd.print("Remove Finger");
delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {
p = finger.getImage();
}
Serial.print("ID "); Serial.println(id);
p = -1;
Serial.println("Place same finger again");
lcd.clear();
lcd.print("Place Finger");
lcd.setCursor(0,1);
lcd.print(" Again");
while (p != FINGERPRINT_OK) {
p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.print(".");
break;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
break;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
break;
default:
Serial.println("Unknown error");
return;
}
}
 
// OK success!
 
p = finger.image2Tz(2);
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
return p;
}
 
// OK converted!
Serial.print("Creating model for #"); Serial.println(id);
 
p = finger.createModel();
if (p == FINGERPRINT_OK) {
Serial.println("Prints matched!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_ENROLLMISMATCH) {
Serial.println("Fingerprints did not match");
return p;
} else {
Serial.println("Unknown error");
return p;
}
 
Serial.print("ID "); Serial.println(id);
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
Serial.println("Stored!");
lcd.clear();
lcd.print(" Finger Stored!");
delay(2000);
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_BADLOCATION) {
Serial.println("Could not store in that location");
return p;
} else if (p == FINGERPRINT_FLASHERR) {
Serial.println("Error writing to flash");
return p;
}
else {
Serial.println("Unknown error");
return p;
}
}
 
int getFingerprintIDez()
{
uint8_t p = finger.getImage();
 
if (p != FINGERPRINT_OK)
return -1;
 
p = finger.image2Tz();
if (p != FINGERPRINT_OK)
return -1;
 
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK)
{
lcd.clear();
lcd.print("Finger Not Found");
lcd.setCursor(0,1);
lcd.print("Try Later");
delay(2000);
return -1;
}
// found a match!
Serial.print("Found ID #");
Serial.println(finger.fingerID);
 temp_amb = mlx.readAmbientTempC();// temp_amb should read mlx as ambient temperature
 temp_obj = mlx.readObjectTempC();// temp_obj should read mlx as object temperature

 //lcd display
 lcd.setCursor(0,0);
 lcd.print("Room Temp:");// lcd display Room Temp
 lcd.setCursor(10,0);
 lcd.print(temp_amb);
 lcd.setCursor(15,0);
 lcd.write(1);
 lcd.setCursor(0,1);
 lcd.print("Obj Temp :");// lcd display Obj Temp
 lcd.setCursor(10,1);
 lcd.print(temp_obj);
 lcd.setCursor(15,1);
 lcd.write(1);
 delay(10000);//delay for 5000ms
 lcd.clear();


//serial Monitor
Serial.print("Room Temp = ");
Serial.println(temp_amb);
Serial.print("Object Temp = ");
Serial.println(temp_obj);
if((temp_obj < 36.1)|| (temp_obj > 37.6))// if object temperature is less 34 or greater than 38
{

  lcd.print("  Temperature  "); lcd.setCursor(0,1);lcd.print("  Warning!!!  ");delay(5000); lcd.clear();tone(12,2000);// buzzer shold go high
  delay(5000);
  noTone(12);
}
else
{
    noTone(12);//buzzer should go low
}
return finger.fingerID;
}
 
uint8_t deleteFingerprint(uint8_t id)
{
uint8_t p = -1;
lcd.clear();
lcd.print("Please wait");
p = finger.deleteModel(id);
if (p == FINGERPRINT_OK)
{
Serial.println("Deleted!");
lcd.clear();
lcd.print("Finger Deleted");
lcd.setCursor(0,1);
lcd.print("Successfully");
delay(1000);
}
 
else
{
Serial.print("Something Wrong");
lcd.clear();
lcd.print("Something Wrong");
lcd.setCursor(0,1);
lcd.print("Try Again Later");
delay(2000);
return p;
}
}
void download(int eepIndex)// for downloads of data
{
 
if(EEPROM.read(eepIndex) != 0xff)
{
Serial.print("T->");
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print(':');
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print(':');
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print(" D->");
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print('/');
if(EEPROM.read(eepIndex)<10)
Serial.print('0');
Serial.print(EEPROM.read(eepIndex++));
Serial.print('/');
Serial.print(EEPROM.read(eepIndex++)<<8 | EEPROM.read(eepIndex++));
}
else
{
Serial.print("---------------------------");
}
 
Serial.print(" ");
}
