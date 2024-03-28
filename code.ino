
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "LAB IOT";
const char* password = "@udbsurakarta";

// Initialize Telegram BOT
#define BOTtoken "7156720220:AAEA0N-9oK1UN4vzkHNJC6rFjDKHNdBAwHU"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "7147678334"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin1 = 14;
const int ledPin2 = 12;
const int ledPin3 = 13;
bool ledState1 = LOW;
bool ledState2 = LOW;
bool ledState3 = LOW;

String mode;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
 Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to control your outputs.\n\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/led_on") {
      String welcome = "Select the light you want to turn on.\n\n";
      welcome += "/led_1 to turn GPIO 14 ON \n";
      welcome += "/led_2 to turn GPIO 12 ON \n";
      welcome += "/led_3 to turn GPIO 13 ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      bot.sendMessage(chat_id, welcome, "");
      mode = "on";
    }

    if (text == "/led_1") {
      if (mode == "on"){
      ledState1 = HIGH;
      }else if (mode == "off"){
      ledState1 = LOW;
      }else {
        String welcome = "Select the light you want to turn on.\n\n";
      welcome += "/led_1 to turn GPIO 14 ON \n";
      welcome += "/led_2 to turn GPIO 12 ON \n";
      welcome += "/led_3 to turn GPIO 13 ON \n";
      bot.sendMessage(chat_id, welcome, "");
        Serial.println("Error Mode : "+ mode);
      }
      digitalWrite(ledPin1, ledState1);
      bot.sendMessage(chat_id, "LED state set to "+ mode, "");
    }
    if (text == "/led_2") {
      if (mode == "on"){
      ledState2 = HIGH;
      }else if (mode == "off"){
      ledState2 = LOW;
      }else {
        String welcome = "Select the light you want to turn on.\n\n";
      welcome += "/led_1 to turn GPIO 14 ON \n";
      welcome += "/led_2 to turn GPIO 12 ON \n";
      welcome += "/led_3 to turn GPIO 13 ON \n";
      bot.sendMessage(chat_id, welcome, "");
        Serial.println("Error Mode : "+ mode);
      }
      digitalWrite(ledPin2, ledState2);
      bot.sendMessage(chat_id, "LED state set to "+mode, "");
    }
    if (text == "/led_3") {
      if (mode == "on"){
      ledState3 = HIGH;
      }else if (mode == "off"){
      ledState3 = LOW;
      }else {
        String welcome = "Select the light you want to turn on.\n\n";
      welcome += "/led_1 to turn GPIO 14 ON \n";
      welcome += "/led_2 to turn GPIO 12 ON \n";
      welcome += "/led_3 to turn GPIO 13 ON \n";
      bot.sendMessage(chat_id, welcome, "");
        Serial.println("Error Mode : "+ mode);
      }
      digitalWrite(ledPin3, ledState3);
      bot.sendMessage(chat_id, "LED state set to "+ mode, "");
    }
    
    if (text == "/led_off") {
      String welcome = "Select the light you want to turn Off.\n\n";
      welcome += "/led_1 to turn GPIO 14 Off \n";
      welcome += "/led_2 to turn GPIO 12 Off \n";
      welcome += "/led_3 to turn GPIO 13 Off \n";
      welcome += "/led_on to turn GPIO ON \n";
      bot.sendMessage(chat_id, welcome, "");
      mode="off";
    }
    
    if (text == "/state") {
      if (digitalRead(ledPin1)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin1, ledState1);
  digitalWrite(ledPin2, ledState2);
  digitalWrite(ledPin3, ledState3);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
