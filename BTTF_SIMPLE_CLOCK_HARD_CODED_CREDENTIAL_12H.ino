#include "TM1637Display.h"
#include "WiFiManager.h"
#include "NTPClient.h"


#define red_CLK 16
#define red1_DIO 17
#define red2_DIO 18
#define red3_DIO 19

#define AM 32
#define PM 33

bool res;
const long utcOffsetInSeconds = 3600; // Offset in second
int Hour = 0;

//========================USEFUL VARIABLES=============================
int UTC = 2; // UTC + value in hour - Summer time
int Display_backlight = 3; // Set displays brightness 0 to 7;

const char *ssid     = "YOUR SSID HERE"; 
const char *password = "WIFI PASSWORD HERE";

//======================================================================

// Setup the red displays
TM1637Display red1(red_CLK, red1_DIO);
TM1637Display red2(red_CLK, red2_DIO);
TM1637Display red3(red_CLK, red3_DIO);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds*UTC);

void setup() {

  pinMode(red_CLK, OUTPUT);
  pinMode(red1_DIO, OUTPUT);
  pinMode(red2_DIO, OUTPUT);
  pinMode(red3_DIO, OUTPUT);
  pinMode(AM, OUTPUT);
  pinMode(PM, OUTPUT);

  //manager.resetSettings();

  Serial.begin(9600);
  
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  red1.setBrightness(Display_backlight);
  red2.setBrightness(Display_backlight);
  red3.setBrightness(Display_backlight);

}

void loop() {

  timeClient.update();
  Serial.print("Time: ");
  Serial.println(timeClient.getFormattedTime());
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int currentYear = ptm->tm_year+1900;
  Serial.print("Year: ");
  Serial.println(currentYear);
  
  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);

  int currentMonth = ptm->tm_mon+1;
  Serial.print("Month: ");
  Serial.println(currentMonth);
     if (timeClient.getHours() == 0)
  { 
    Hour = 12;
    Serial.write(" =0");
  }

  else if (timeClient.getHours() == 12)
  { 
    Hour = timeClient.getHours();
    Serial.write(" =12");
  }
  
  else if (timeClient.getHours() >= 13) {
    Hour = timeClient.getHours() - 12;
    Serial.write(" >=13");
  }

  else {
    Hour = timeClient.getHours();
  }
  
red1.showNumberDecEx(currentMonth,0b01000000,true,2,0);
red1.showNumberDecEx(monthDay,0b01000000,true,2,2);
red2.showNumberDecEx(currentYear,0b00000000,true);
red3.showNumberDecEx(Hour,0b01000000,true,2,0);
red3.showNumberDecEx(timeClient.getMinutes(),0b01000000,true,2,2);

if((currentMonth*30 + monthDay) >= 121 && (currentMonth*30 + monthDay) < 331){
timeClient.setTimeOffset(utcOffsetInSeconds*UTC);} // Change daylight saving time - Summer
else {timeClient.setTimeOffset((utcOffsetInSeconds*UTC) - 3600);} // Change daylight saving time - Winter


if(timeClient.getHours()>=13){
  digitalWrite(AM,0);
  digitalWrite(PM,1);}
  
else if(timeClient.getHours()==12){
  digitalWrite(AM,0);
  digitalWrite(PM,1);}

else{
  digitalWrite(AM,1);
  digitalWrite(PM,0);}



}

