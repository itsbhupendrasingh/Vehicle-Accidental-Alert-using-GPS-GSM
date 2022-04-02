/*
 * designed by Bhupedra Singh
 * SME Dehradun
 */
const int x=A0, y=A1, z=14;
int xv=0, yv=0, zv=0;

#include <TinyGPS.h>
#include<SoftwareSerial.h>
TinyGPS gps;
SoftwareSerial gps_port(2,3); 
char sz[32];
int fr = 10, fv; //fire
int buzzer=9; //need to be defined
float latitude, longitude;
 
void setup()
{
 pinMode(buzzer, OUTPUT);// button 
 digitalWrite(buzzer, LOW);
 Serial.begin(9600);
 gps_port.begin(9600);
 delay(2000);
 init_GSM();
 delay(2000);
}

void loop()
{
  encode_gps();
  fire();
  adxl();
  delay(100);
}

void Fire_sms()
{
  digitalWrite(buzzer, HIGH);
  Serial.println("AT"); 
  delay(2000);
  Serial.println("AT+CMGF=1");
  delay(2000);
  Serial.println("AT+CMGS=\"+919927715123\"\r"); // Replace your mobile no here
  delay(2000);
  Serial.print("Car Fire Alert at ");
  Serial.print("http://www.google.com/maps/place/");
  delay(2000);
  Serial.print(latitude,6);
  Serial.print(",");
  Serial.println(longitude,6);      
  delay(1000);
  Serial.print(sz);
  Serial.print(",");
  Serial.println((char)26);//the stopping character
  delay(2000);
  digitalWrite(buzzer, LOW);
}
void Accident_sms()
{
  digitalWrite(buzzer, HIGH);
  Serial.println("AT");
  delay(2000);
  Serial.println("AT+CMGF=1"); 
  delay(2000);
  Serial.println("AT+CMGS=\"+919927715123\"\r"); // Replace your mobile no here
  delay(2000);
  Serial.print("Car Accident Alert at ");
  Serial.print("http://www.google.com/maps/place/");
  delay(2000);
  Serial.print(latitude,6);
  Serial.print(",");
  Serial.println(longitude,6);      
  delay(1000);
  Serial.print(sz);
  Serial.print(",");
  Serial.println((char)26);
  delay(2000);
  digitalWrite(buzzer, LOW);
}

void encode_gps()
{
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gps_port.available())
    {
      char a = gps_port.read();
      if(gps.encode(a)) 
       {
        getgps(gps); 
       }
    }
  }
}
void getgps(TinyGPS &gps)
{
 gps.f_get_position(&latitude, &longitude);
 print_date(gps);
}
void init_GSM()
{
  Serial.println("AT");    
  delay(2000);
  Serial.println("AT+CMGF=1"); 
  delay(2000);
}

void adxl()
{
  xv = analogRead(x);
  yv = analogRead(y);
  zv = analogRead(z);

 if((xv<280) || (xv>380) || (yv<280) || (yv>380)) // calibrate the accelerometer and put the values here
  {
    Accident_sms();
  }
  else
  {
    // do nothing
  }
}

void fire()
{
  fv=digitalRead(fr);
  if(fv==HIGH)
  {
    Fire_sms();

  }
  else
  {
    //do nothing
  }
  delay(100);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
  {
    // do nothing
  }
  else
  {
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
  }
  delay(0);
}
