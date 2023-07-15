
// LORA(SX1276)
#include <SPI.h>
#include <LoRa.h>
//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
#define BAND 866E6
#define TX_POWER            20
#define SPREADING_FACTOR    12
#define SIGNAL_BANDWIDTH    125E3
#define CODING_RATE         5





//DISPLAY(OLED)
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);





// WI-FI
#include <WiFi.h>
const char* ssid     = "FMCS";
const char* password = "917115824";
//String ipString;



//SERVIDOR WEB THINGSPEAK HTTP
#include <HTTPClient.h>
//Thingspeak
String serverAddress =  "api.thingspeak.com";
const String apiKey = "GR6EA7YAN26299QS";





//NTP SERVER
#include <NTPClient.h>
#include <WiFiUdp.h>
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
// Variables to save date and time
String formattedDate;
String day;
String hour;
String timestamp;



//DADOS
String packet ;
String packSize = "--";
String rssi = "RSSI --";
String ID;
String temperatura_interior;
String temperatura_exterior;
String humidade_interior;
String humidade_exterior;
String som;
String massa;
String tratamento;
String armadilha;


// Envio dos dados para o ThingSpeak
void sendDataToThingSpeak(String temperatura_interior, String temperatura_exterior, String humidade_interior, String humidade_exterior, String massa, String som, String tratamento, String armadilha)
{
  String url = "http://";
  url += serverAddress;
  url += "/update?api_key=";
  url += apiKey;
  url += "&field1=";
  url += String(temperatura_interior);
  url += "&field2=";
  url += String(temperatura_exterior);
  url += "&field3=";
  url += String(humidade_interior);
  url += "&field4=";
  url += String(humidade_exterior);
  url += "&field5=";
  url += String(massa);
  url += "&field6=";
  url += String(som);
  url += "&field7=";
  url += String(tratamento);
  url += "&field8=";
  url += String(armadilha);
  
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode == 200)
  {
    Serial.println("Dados enviados para o ThingSpeak com sucesso!");
  } else
  {
    Serial.print("Erro ao enviar dados para o ThingSpeak. Código de resposta HTTP: ");
    Serial.println(httpResponseCode);
  }
  http.end();
   Serial.println("--------------------------------------//----------------------------------------");
}











//RECEBE E DIVIDE PACOTE DOS DADOS
void LoraData()
{
    //Divisao do pacote 
    int separatorIndex1 = packet.indexOf(';');
    int separatorIndex2 = packet.indexOf(';', separatorIndex1 + 1);
    int separatorIndex3 = packet.indexOf(';', separatorIndex2 + 1);
    int separatorIndex4 = packet.indexOf(';', separatorIndex3 + 1);
    int separatorIndex5 = packet.indexOf(';', separatorIndex4 + 1);
    int separatorIndex6 = packet.indexOf(';', separatorIndex5 + 1);
    int separatorIndex7 = packet.indexOf(';', separatorIndex6 + 1);

    
    temperatura_interior = packet.substring(0, separatorIndex1);
    temperatura_exterior = packet.substring(separatorIndex1 + 1, separatorIndex2);
    humidade_interior = packet.substring(separatorIndex2 + 1, separatorIndex3);
    humidade_exterior = packet.substring(separatorIndex3 + 1, separatorIndex4);
    massa = packet.substring(separatorIndex4 + 1, separatorIndex5);
    som = packet.substring(separatorIndex5 + 1, separatorIndex6);
    tratamento = packet.substring(separatorIndex6 + 1, separatorIndex7);
    armadilha =  packet.substring(separatorIndex7 + 1);
 

  // RSSI
  Serial.println(rssi);  


 //TEMPERATURA INTERIOR
    Serial.print("Temperatura interior: ");
    Serial.print(temperatura_interior);
    Serial.println(" º Celcius");
  
//TEMPERATURA EXTERIOR
    Serial.print("Temperatura exterior: ");
    Serial.print(temperatura_exterior);
    Serial.println(" º Celcius");

//HUMIDADE INTERIOR
    Serial.print("Humidade interior: ");
    Serial.print(humidade_interior);
    Serial.println(" %");
 
//HUMIDADE EXTERIOR
    Serial.print("Humidade exterior: ");
    Serial.print(humidade_exterior);
    Serial.println(" %");

//MASSA COLMEIA                    
  Serial.print("Massa: ");  
  Serial.print(massa);        
  Serial.println(" kg");    


//INTENSIDADE SONORA
Serial.print("Som: ");
Serial.print(som);


//TRATAMENTO VARROA
  Serial.print("Tratamento: ");
  Serial.println(tratamento);

  
//ARMADILHA VESPA
  Serial.print("Armadilha: ");
  Serial.println(armadilha);                                                            
}





//ESCRITA DOS DADOS NO DISPLAY 
void escrita_display()
{
   
    
  display.clearDisplay();
  
  //TEMPERATURA INTERIOR
  display.setCursor(0,0);
  display.print("Temp: ");
  display.setCursor(35,0);
  display.print(temperatura_interior);
  display.setCursor(55,0);
  display.print("/");
  
  //TEMPERATURA EXTERIOR
  display.setCursor(67,0);
  display.print(temperatura_exterior);
  display.setCursor(85,0);
  display.print("C");

  //HUMIDADE INTERIOR
  display.setCursor(0,10);
  display.print("Hum: ");
  display.setCursor(35,10);
  display.print(humidade_interior);
  display.setCursor(55,10);
  display.print("/");
 
  //HUMIDADE EXTERIOR
  display.setCursor(67,10);
  display.print(humidade_exterior);
  display.setCursor(85,10);
  display.print("%");


  //MASSA COLMEIA
  display.setCursor(0,20);
  display.print("Massa: ");
  display.setCursor(45,20);
  display.print(massa);
  display.setCursor(85,20);
  display.print("Kg");


  //INTENSIDADE SONORA
  display.setCursor(0,30);
  display.print("Som: ");
  display.setCursor(35,30);
  display.print(som);


 //TRATAMENTO VARROA
  display.setCursor(0,40);
  display.print("Tratamento Varroa: ");    
  display.setCursor(115,40);
  display.print(tratamento);    

 //ARMADILHA VESPA
  display.setCursor(0,50);
  display.print("Armadilha Vespa: ");
  display.setCursor(115,50);
  display.print(armadilha);
  display.display();

}






void cbk(int packetSize)
{
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++)
  {
    packet += (char) LoRa.read();
    }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
}





void connectWiFi()
{
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
   //ipString = WiFi.localIP().toString();
   display.clearDisplay();
   display.setCursor(0,0);
   display.print("wifi iniciado");
  // display.setCursor(0,10);
   //display.print("IP address: ");
   //display.setCursor(0,20);
   //display.print(ipString);
   display.display();
   Serial.println("wifi iniciado");
   //Serial.println("IP address: ");
   //Serial.println(ipString);
   delay(5000);
}





// Function to get date and time from NTPClient
void getTimeStamp() 
{
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  // Extract date
  int splitT = formattedDate.indexOf("T");
  day = formattedDate.substring(0, splitT);
  Serial.println(day);
  // Extract time
  hour = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.println(hour);
  timestamp = day + " " + hour;
}







//INICIALIZAÇOES
void setup()
{
    //SERIAL
  Serial.begin(115200);

  
  //LED(ENVIA PACOTE)
  pinMode(25, OUTPUT);

 
  //DISPLAY(OLED) + LORA(SX1276)
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false))
  { 
    while (1);
  }
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(BAND)) 
  {
    while (1);
  }
 
  LoRa.setTxPower(TX_POWER);
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(SIGNAL_BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  Serial.println("Oled + Lora iniciado");
  delay(1000);



//WIFI
  connectWiFi();

 
// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
   //GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("NTP client iniciado");
  display.display();
  Serial.println("NTP client iniciado");
  delay(2000);




 //LoRa.onReceive(cbk);
  LoRa.receive();
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Dispositivo pronto!");
  display.display();
  Serial.println("Dispositivo pronto!");
  Serial.println("--------------------------------------//----------------------------------------");  
  delay(2000);
}




void loop()
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    connectWiFi();
  }
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  { 
    getTimeStamp();
  
   //LED(RECEBE PACOTE) PISCA
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
   
    cbk(packetSize); 
    LoraData();
    escrita_display();
    sendDataToThingSpeak(temperatura_interior, temperatura_exterior, humidade_interior, humidade_exterior,massa,som,tratamento,armadilha);

  }
}
