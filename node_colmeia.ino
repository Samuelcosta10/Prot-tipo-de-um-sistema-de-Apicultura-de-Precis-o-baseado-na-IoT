
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







//TEMPERATURA E HUMIDADE
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTINTERIOR 13
#define DHTEXTERIOR 23    
#define DHTTYPE    DHT11     
DHT dhtinterior(DHTINTERIOR, DHTTYPE);
DHT dhtexterior(DHTEXTERIOR, DHTTYPE);
int humidade_interior;
int temperatura_interior;
int humidade_exterior;
int temperatura_exterior;






// ARMADILHA VESPA
 #define LDR_PIN 36
 int ldr_bits;
 float u_ldr; 
#define RELAY_PIN 17
int armadilha = 0;




//INTENSIDADE SONORA
 #define SOM_PIN 37
 int som_bits;
 float u_som;
 int som;





//MASSA COLMEIA
#include "HX711.h"
const int LOADCELL_DOUT_PIN = 21;
const int LOADCELL_SCK_PIN = 22;
float massa;
HX711 scale;
#define START_PIN 39






//TRATAMENTO VARROA
int tratamento = 0;
#define SWITCH_PIN 38











//LEITURA E ESCRITA DOS DADOS NA SERIAL
void leitura_dados()
{
  
  
  //TEMPERATURA INTERIOR
  temperatura_interior = dhtinterior.readTemperature();
  if (isnan(temperatura_interior))
  {    
    Serial.println("Failed to read from DHT interior sensor!");
  }
  else 
  {
    Serial.print("Temperatura interior: ");
    Serial.print(temperatura_interior);
    Serial.println(" º Celcius");
  }
  delay(1000);





  
//TEMPERATURA EXTERIOR
  temperatura_exterior = dhtexterior.readTemperature();
  if (isnan(temperatura_exterior))
  {    
    Serial.println("Failed to read from DHT exterior sensor!");
  }
  else 
  {
    Serial.print("Temperatura exterior: ");
    Serial.print(temperatura_exterior);
    Serial.println(" º Celcius");
  }
 delay(1000);



//HUMIDADE INTERIOR
  humidade_interior = dhtinterior.readHumidity();
  if (isnan(humidade_interior))
  {
    Serial.println("Failed to read from DHT intererior sensor!");
  }
  else 
  {
    Serial.print("Humidade interior: ");
    Serial.print(humidade_interior);
    Serial.println(" %");
  }
 delay(1000);



//HUMIDADE EXTERIOR
  humidade_exterior = dhtexterior.readHumidity();
  if (isnan(humidade_exterior))
  {
    Serial.println("Failed to read from DHT exterior sensor!");
  }
  else 
  {
    Serial.print("Humidade exterior: ");
    Serial.print(humidade_exterior);
    Serial.println(" %");
  }
 delay(1000);




//MASSA COLMEIA                 
  scale.power_up();
  massa =  scale.get_units();
 if (massa < 0.1)
 {
  massa = 0.00;
 }
  scale.power_down();             // put the ADC in sleep mode
  Serial.print("Massa: ");  
  Serial.print(massa);        
  Serial.println(" kg");                                                                
  delay(1000);
 

//INTENSIDADE SONORA
som = 0;
for (int i = 0; i < 500; i++)
{
  if (analogRead(SOM_PIN) > 0)
  {
    som = 1;
  }
}

if (som != 1)
{
  som = 0;
}

Serial.print("Intensidade sonora: ");
Serial.println(som);
delay(1000);


 //TRATAMENTO VARROA
  if(analogRead(SWITCH_PIN) < 2500)
  {
  tratamento = 0; 
  }
  else
  { 
   tratamento = 1;
  }
 Serial.print("Tratamento: "); 
 Serial.println(tratamento);   
 delay(1000);




//ARMADILHA VESPA
  if(analogRead(LDR_PIN) < 2500)
  {
  digitalWrite(RELAY_PIN, LOW);   
  armadilha = 1; 
  }
  else
  {
  digitalWrite(RELAY_PIN, HIGH);  
  armadilha = 0; 
  }
  Serial.print("Armadilha: ");
  Serial.println(armadilha);
  Serial.println("--------------------------------------//----------------------------------------");  
 delay(1000);

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







//ENVIA PACOTE DOS DADOS
void envia_pacote()
{
  LoRa.beginPacket();
  LoRa.print(temperatura_interior);
  LoRa.print(";");
  LoRa.print(temperatura_exterior);
  LoRa.print(";");
  LoRa.print(humidade_interior);
  LoRa.print(";");
  LoRa.print(humidade_exterior);
  LoRa.print(";");
  LoRa.print(massa);
  LoRa.print(";");
  LoRa.print(som);
  LoRa.print(";");
  LoRa.print(tratamento);
  LoRa.print(";");
  LoRa.print(armadilha);
  LoRa.endPacket();
}








//INICIALIZAÇOES
void setup()
{
 
 
  //SERIAL
  Serial.begin(115200);


  //RELAY(ARMADILHA VESPA)
  pinMode(RELAY_PIN, OUTPUT);

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



 //TEMPERATURA E HUMIDADE INTERIOR
  dhtinterior.begin();
  Serial.println("DHT INT iniciado");
  delay(1000);

 
  //TEMPERATURA E HUMIDADE EXTERIOR
  dhtexterior.begin();
  Serial.println("DHT EXT iniciado");
  delay(1000);


 //MASSA COLMEIA
  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Retira a colmeia...");
  display.setCursor(0,10);
  display.print("Carrega START!");
  display.display();
  Serial.println("Retira a colmeia...");
  Serial.println("Carrega START!");

 while(analogRead(START_PIN) < 2500)
{
   // O código dentro deste loop será executado enquanto a leitura não for alta.
}

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-21819);
  scale.tare();  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Tara concluida...");
  display.setCursor(0,10);
  display.print("Coloca a colmeia...");
  display.setCursor(0,20);
  display.print("Pressiona START!");
  display.display();
  Serial.println("Tara concluida...");
  Serial.println("Coloque a colmeia...");
  Serial.println("Pressiona START!");

 while(analogRead(START_PIN) < 2500)
{
   // O código dentro deste loop será executado enquanto a leitura não for alta.
}

  display.setCursor(0,0);
  display.clearDisplay();
  display.print("Colmeia pronta!");
  display.display();
  Serial.println("Colmeia pronta!");
  Serial.println("--------------------------------------//----------------------------------------");  

 //DESLIGA DISPLAY
  delay(5000);
  display.clearDisplay();
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}







void loop()
{
leitura_dados();
//escrita_display();
envia_pacote();
digitalWrite(25,HIGH);
delay(1000);
digitalWrite(25,LOW);
delay(5000);
}
