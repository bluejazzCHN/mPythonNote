#include <Arduino.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <SmartLeds.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE,22,23);
//u8g2 instance
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, 22, 23); //22,23 is the pin of esp32 chip not a handbit P19 or P20 according the shcema.

const char *ssid = "ZSJ_HOME";
const char *password = "QQqq11!!";
IPAddress ip;

const int LED_COUNT = 3;
const int DATA_PIN = 17;
const int CHANNEL = 0;

SmartLed leds(LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer);

void serialInit()
{
  Serial.begin(9600);
  while (!Serial)
  {
    continue;
  }
  Serial.println();
  Serial.println("Serial initialation is done!");
}

int buttonB = 2;
void buttonBPress()
{
  Serial.println("Button B is pressed!");
  Serial.print("Sound Level:");
  Serial.println(analogRead(36));
}

void functionIOInit()
{
  pinMode(buttonB,INPUT);
  attachInterrupt(buttonB,buttonBPress,FALLING);
  pinMode(36,INPUT);
} 

void wifiInit()
{
  Serial.println("WiFi attempt to connect ...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ip = WiFi.localIP();
  Serial.println(ip);
}
uint8_t hue;
void showGradient() {
    hue++;
    // Use HSV to create nice gradient
    for ( int i = 0; i != LED_COUNT-1; i++ )
        leds[ i ] = Hsv{ static_cast< uint8_t >( hue + 10 * i ), 125, 125 };
    leds.show();
    // Show is asynchronous; if we need to wait for the end of transmission,
    // we can use leds.wait(); however we use double buffered mode, so we
    // can start drawing right after showing.
}
void printDemo()
{
  u8g2.setFont(u8g2_font_unifont_t_chinese2); // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do
  {
    u8g2.setCursor(0, 15);
    u8g2.print("Hello World!");
    //    u8g2.setCursor(0, 30);
    //    u8g2.print("你好世界");		// Chinese "Hello World"

    //    u8g2.drawBox(0, 55, 80, 10);
    //    u8g2.drawCircle(110, 7, 10, U8G2_DRAW_ALL);
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    u8g2.drawGlyph(110, 15, 0x2603); /* dec 9731/hex 2603 Snowman */
  } while (u8g2.nextPage());
  // showGradient();
  delay(1000);
}

void setup(void)
{
  //u8g2 initial
  u8g2.begin();
  serialInit();
  wifiInit();
  functionIOInit();
}

void loop(void)
{
  printDemo();
}