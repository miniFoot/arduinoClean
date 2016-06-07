#include <Adafruit_NeoPixel.h>

#include <Process.h>
#include <Bridge.h>
#define ledPin  13
Process process;

int trig = 12;
int echo = 11;

int trig2 = 7;
int echo2 = 6;

long lecture_echo;
long cm;
long cm2 ;
long timeSinceTweet, timeSinceData;
int lastDistSensor;

String url = "http://5.196.7.169:8080/newgoal";
String myTweet ;
int freqTweet = 4000;
bool starting;

/* NEO PIXEL PART */
#define PIXEL_PIN 10
#define PIXEL_COUNT 10
#define PIXEL_TYPE  NEO_GRB + NEO_KHZ800

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

#define PIXEL_PIN2 10
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN2, PIXEL_TYPE);

void setup()
{
  Bridge.begin();

  strip.begin();
  strip.show();

  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  pinMode(trig2, OUTPUT);
  digitalWrite(trig2, LOW);
  pinMode(echo2, INPUT);
  Serial.begin(9600);

  timeSinceData = 0;
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
}

void loop()
{

  pinMode(ledPin, OUTPUT);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  lecture_echo = pulseIn(echo, HIGH);
  cm = lecture_echo / 58;

  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  lecture_echo = pulseIn(echo, HIGH);

  cm2 = lecture_echo / 58;

  if (cm < 19 || cm > 22 ) {
    if ((millis() - timeSinceData) > freqTweet) {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, 25, 25, 250);
      }
      strip.show();
      tweetPing("1");
      timeSinceData = millis();
    }
  }
  if (cm2 < 19 || cm2 > 22) {
    if ((millis() - timeSinceData) > freqTweet) {
      for (int i = 0; i < strip2.numPixels(); i++) {
        strip2.setPixelColor(i, 250, 25, 25);
      }
      strip2.show();
      tweetPing("0");
      timeSinceData = millis();
    }
  }
  Serial.print("Distancem : ");
  Serial.println(cm);
  Serial.println(cm2);
}
void tweetPing(String Tweet) {
  digitalWrite(ledPin, HIGH);
  Process p;
  String curlCmd;
  curlCmd = "curl " ;
  curlCmd += "-X POST -d \"";
  curlCmd += "redGoal=" ;
  curlCmd += Tweet;
  curlCmd += "\" " ;
  curlCmd += url;
  p.runShellCommand(curlCmd);
  Serial.println("Posting");
  delay(4000);
  Serial.println(curlCmd);
  Serial.println(Tweet) ;
  myTweet = "";
  // Reset des pararmètres
  timeSinceTweet = millis();

}
