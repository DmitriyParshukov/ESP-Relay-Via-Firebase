#include <ESP8266WiFi.h>                
#include <ESP8266HTTPClient.h>                                                                    

/*------------CONFIG------------*/     
#define WIFI_SSID "YOUR_WIFI"                                          
#define WIFI_PASSWORD "******"      

String FireBaseUrl = "https://esp-fire-test-default-rtdb.europe-west1.firebasedatabase.app";           
String FireBaseSecret = "VeHg1m09vzWZI2w***************kl2W3Sk8T3";

String RelayName = "Lamp";     
int RelayPIN = 16;          
/*------------CONFIG------------*/              

String WorkUrl = ""; 
String State = "";                                                    
                                                              
void setup() 
{
  Serial.begin(115200);
  delay(1000);    
  pinMode(RelayPIN, OUTPUT);                 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                               
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);   

  WorkUrl = FireBaseUrl + "/" + RelayName + ".json?auth=" + FireBaseSecret;

  Serial.println(WorkUrl);     

  SetBaseRelayState();
}
 
void loop() 
{
  State = GetRelayState(); 

  Serial.println(State); 
                              
  if (State == "1") 
  {                                                                                                              
    digitalWrite(RelayPIN, HIGH);                                                        
  } 
  else if (State == "0") 
  {                                                 
    digitalWrite(RelayPIN, LOW);                                                         
  }
  else 
  {
    Serial.println("Error");
  }

  delay(2000);
}

String GetRelayState() 
{
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClientSecure client;
  
    client.setInsecure();
  
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");

    http.begin(client, WorkUrl); 
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] GET...\n");

    int httpCode = http.GET();

    if (httpCode > 0) {

      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String Res = http.getString();
        
        http.end();
        
        return Res;
      }
      
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    return "Error";
  }
}

void SetBaseRelayState() 
{
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClientSecure client;
  
    client.setInsecure();
  
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");

    http.begin(client, WorkUrl); 
    http.addHeader("Content-Type", "text/plain");

    Serial.print("[HTTP] PUT...\n");

    int httpCode = http.PUT("0");

    if (httpCode > 0) {

      Serial.printf("[HTTP] PUT... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String Res = http.getString();
        Serial.println("SetBaseRelayState: " + Res); 
      }
      
    } else {
      Serial.printf("[HTTP] PUT... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}
