#include <SPI.h>
#include <RFID.h>
#include <Time.h>
#define SS_PIN D2
#define RST_PIN D1

int timezone = 7 * 3600; //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0; //กำหนดค่า Date Swing Time

#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>
const char* ssid     = "xxxxxxxxxx";
const char* password = "xxxxxxxxxx";
#define LINE_TOKEN  "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

RFID rfid(SS_PIN, RST_PIN);

int buzzPin = D4;

// Setup variables:
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;

void setup()
{
  Serial.begin(115200);
  
   Serial.println("Starting...");

   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) 
   {
      delay(250);
      Serial.print(".");
   }

   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());

   LINE.setToken(LINE_TOKEN);

   configTime(timezone, dst, "pool.ntp.org", "time.nist.gov"); //ดึงเวลาจาก Server

  Serial.println("\nLoading time");

  while (!time(nullptr)) {

     Serial.print("*");

    delay(1000);

  }
  
  SPI.begin();
  rfid.init();
}

void loop()
{
time_t now = time(nullptr);
   struct tm* p_tm = localtime(&now);

   
  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      if (rfid.serNum[0] != serNum0
          && rfid.serNum[1] != serNum1
          && rfid.serNum[2] != serNum2
          && rfid.serNum[3] != serNum3
          && rfid.serNum[4] != serNum4
         ) {
        /* With a new cardnumber, show it. */
        Serial.println(" ");
        Serial.println("Card found");
        serNum0 = rfid.serNum[0];
        serNum1 = rfid.serNum[1];
        serNum2 = rfid.serNum[2];
        serNum3 = rfid.serNum[3];
        serNum4 = rfid.serNum[4];

        //Serial.println(" ");
        Serial.println("Cardnumber:");
        Serial.print("Dec: ");
        Serial.print(rfid.serNum[0], DEC);
        Serial.print(", ");
        Serial.print(rfid.serNum[1], DEC);
        Serial.print(", ");
        Serial.print(rfid.serNum[2], DEC);
        Serial.print(", ");
        Serial.print(rfid.serNum[3], DEC);
        Serial.print(", ");
        Serial.print(rfid.serNum[4], DEC);
        Serial.println(" ");

        Serial.print("Hex: ");
        Serial.print(rfid.serNum[0], HEX);
        Serial.print(", ");
        Serial.print(rfid.serNum[1], HEX);
        Serial.print(", ");
        Serial.print(rfid.serNum[2], HEX);
        Serial.print(", ");
        Serial.print(rfid.serNum[3], HEX);
        Serial.print(", ");
        Serial.print(rfid.serNum[4], HEX);
        Serial.println(" ");
        //buzzer
        analogWrite(3, 20);
        delay(500);
        analogWrite(3, 0);
      } 
      if (rfid.serNum[0] == 33 && rfid.serNum[1] == 160 && rfid.serNum[2] == 25 && rfid.serNum[3] == 28 && rfid.serNum[4] == 132) {
      
      LINE.notify("RFID1 ID = "+String(rfid.serNum[0])+" "+String(rfid.serNum[1])+" "+String(rfid.serNum[2])+" "+String(rfid.serNum[3])+" "+String(rfid.serNum[4])+" Date "+String(p_tm->tm_mday)+"/"+String(p_tm->tm_mon+1)+"/"+String(p_tm->tm_year+2443)+"  "+String(p_tm->tm_hour)+":"+String(p_tm->tm_min)+":"+String(p_tm->tm_sec));
    }
    if (rfid.serNum[0] == 11 && rfid.serNum[1] == 43 && rfid.serNum[2] == 147 && rfid.serNum[3] == 27 && rfid.serNum[4] == 168){
      LINE.notify("RFID2 ID = "+String(rfid.serNum[0])+" "+String(rfid.serNum[1])+" "+String(rfid.serNum[2])+" "+String(rfid.serNum[3])+" "+String(rfid.serNum[4])+" Date "+String(p_tm->tm_mday)+"/"+String(p_tm->tm_mon+1)+"/"+String(p_tm->tm_year+2443)+"  "+String(p_tm->tm_hour)+":"+String(p_tm->tm_min)+":"+String(p_tm->tm_sec));
    }
    else
     Serial.print(".");
    }
    
  }
  
  rfid.halt();
}
