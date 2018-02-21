#include <DHT.h> 
#define DHTPIN 5
#define DHTTYPE DHT11 
int led1=6;
int fan = 11;
int tempMin = 25;   // the temperature to start the fan
int tempMax = 39;   // the maximum temperature when fan is at 100%
int fanSpeed;
DHT dht(DHTPIN, DHTTYPE);
String apiKey = "HXEI6FEULV24OSU9";
String ssid="ap1234";
String password ="ap123456";
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (Serial1.available()){
        c=Serial1.read();
        Serial.print(c);
      }
    }
                   
}
boolean thingSpeakWrite(float value1, float value2){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  Serial1.println(cmd);
 Serial.println(cmd);
  if(Serial1.find("Error")){
   Serial.println("AT+CIPSTART error");
    return false;
  }
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  getStr += "\r\n\r\n";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial1.println(cmd);
Serial.println(cmd);
  delay(100);
  if(Serial1.find(">")){
    Serial1.print(getStr);
   Serial.print(getStr);
  }
  else{
    Serial1.println("AT+CIPCLOSE");
    Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
void setup() {                       
  Serial.begin(9600); 
  dht.begin();         
  Serial1.begin(115200);
  showResponse(1000);
  Serial1.println("AT+CWMODE=1");
  showResponse(1000);
  Serial1.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");
  showResponse(5000);
  Serial.println("Setup completed");
   pinMode(led1,OUTPUT);
   pinMode(fan, OUTPUT);
}
void loop() {
   float t = dht.readTemperature();
   float h = dht.readHumidity();
        Serial.println("Temp="+String(t)+" *C");
      Serial.println("Humidity="+String(h)+" %");
           thingSpeakWrite(t,h);                                    
  if(t< tempMin) { // if temp is lower than minimum temp 
      fanSpeed = 0; // fan is not spinning 
      digitalWrite(fan, LOW); 
   } 
   if((t  >= tempMin) && (t <= tempMax)) { // if temperature is higher than minimum temp 
      fanSpeed = map(t, tempMin, tempMax, 32, 255); // the actual speed of fan 
      digitalWrite(fan, fanSpeed); // spin the fan at the fanSpeed speed 
      digitalWrite(led1,HIGH);
      Serial.println( map(t, tempMin, tempMax, 0, 100));
   } 
  delay(20000);  
}
