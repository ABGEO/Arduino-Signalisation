#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9

#define BOOZER_PIN 8
#define RED_LED_PIN 7
#define GREEN_LED_PIN 6

bool signalisation_enabled = true;
bool signalisation_active = false;

RFID rfid(SS_PIN, RST_PIN);

String DB[] =  {
  "243-154-254-42-189",
  "84-127-73-118-20"
};

bool checkDB (String Code)
{
  for (int i(0); i < sizeof(DB); i++)
    if (Code == DB[i])
      return true;
  return false;
}

void boozer (int x)
{
  for (int i(0); i < x; i++)
  {
    tone(BOOZER_PIN, 500);
    delay(200);
    noTone(BOOZER_PIN);
  }
}

void setup()
{ 
  SPI.begin(); 
  rfid.init();
  
  pinMode(BOOZER_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
  digitalWrite(RED_LED_PIN, HIGH);
}

void loop()
{
  if (rfid.isCard() && rfid.readCardSerial())
  {
    String CardNum = String(rfid.serNum[0]) + "-" + String(rfid.serNum[1]) + "-" + 
      String(rfid.serNum[2])+ "-" + String(rfid.serNum[3])+ "-" + String(rfid.serNum[4]);
        
      if (checkDB(CardNum))
      {
        signalisation_enabled = !signalisation_enabled;
        signalisation_active = false;
        boozer(1);
        delay(300);

        if (signalisation_enabled)
        {
          digitalWrite(RED_LED_PIN, HIGH);
          digitalWrite(GREEN_LED_PIN, LOW);
        }
        else
        {
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(GREEN_LED_PIN, HIGH);
        }
      }
      else
      {
        boozer(2);
        delay(300);
      }
  }

  rfid.halt();
}
