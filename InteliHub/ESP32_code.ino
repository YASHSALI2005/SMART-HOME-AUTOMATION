#include <WiFi.h>
#include <ArduinoWebsockets.h>

using namespace websockets;
WebsocketsClient socket;
const char* websocketServer = "ws://192.168.229.19:8000/";
boolean connected = false;

const char* ssid = "omkar's_phone";
const char* password = "12345678";


int lightCount = 0;
int fanCount = 0;
int acCount = 0;
String equ;

                 
                              // define pins
int lightout = 2;
int lightin = 5;


int fans1 = 18 ;    // output pins
int fans2= 19;
int fans3=4;

int fanup = 12;    // input pins
int fandown= 13;
int fanin= 14;
 
int acin = 22;
int acout = 23;

const int fanled1 = 33;
const int fanled2 = 32;
const int fanled3 = 26;
const int fanled4 = 25;


int fanvar =0;  // fan speed count


int dellay = 0; //update 




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectWiFi();

  pinMode(2, OUTPUT);  //light on / off output
  pinMode(5, INPUT); //light on / off input


  
  pinMode(18, OUTPUT);  // fan speed 1
  pinMode(19, OUTPUT); // fan speed 2
  pinMode(4 , OUTPUT); // fan speed max
  
  pinMode(12, INPUT); // fan speed up
  pinMode(13, INPUT); // fan speed down
  pinMode(14, INPUT);  // fan on / off


  
  pinMode(22, INPUT); // ac on / off input
  pinMode(23,OUTPUT); // ac on/ off output


  pinMode(33,OUTPUT);       // fan speed indicators
  pinMode(32,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);

  
  connectToWebSocket();


  socket.onMessage(handleMessage);
  socket.onEvent(handleEvent);

  
  digitalWrite(lightout,LOW);     //set all pins low initially
  digitalWrite(fans1,LOW);
  digitalWrite(fans2,LOW);
  digitalWrite(fans3,LOW);
  digitalWrite(acout,LOW);
  

}

                                  //status pins


void connectToWebSocket() {
  connected = socket.connect(websocketServer);
  if (connected) {
    Serial.println("Connected");
  }
  else {
    Serial.println("Connection failed.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!connected) {
    Serial.println("Connecting to WebSocket server");
    connectToWebSocket();
  }
  socket.poll();

  // set pin on off by touch
  if (digitalRead(lightin) == 1) {                        //light
    if (lightCount == 0) {
      digitalWrite(lightout,HIGH);
      lightCount = 1;
      senddatatoweb("light");
      delay(500);
    }
    else if (lightCount == 1) {
      digitalWrite(lightout,LOW);
      lightCount = 0;
      senddatatoweb("light");
      delay(500);
    }
  }


  if (digitalRead(acin) == 1) {                                   //ac
    if (acCount == 0) {
      digitalWrite(acout, HIGH);
      acCount = 1;
      senddatatoweb("ac");
      delay(500);
    }
    else if (acCount == 1) {
      digitalWrite(acout, LOW);
      acCount = 0;
      senddatatoweb("ac");
      delay(500);
    }
  }


  if (digitalRead(fanin) == 1) {                                   //fan on / off pin 14
    if (fanCount == 0 && fanvar == 0) {
      digitalWrite(fans1, HIGH);
      digitalWrite(fans2, LOW);
      digitalWrite(fans3, LOW);
      fanCount = 1;
      fanvar=1;
      senddatatoweb("fan");
      senddatatoweb("fanvar");
      delay(500);
    }
    else if (fanCount == 1 && fanvar >=1) {
      digitalWrite(fans1, LOW);
      digitalWrite(fans2, LOW);
      digitalWrite(fans3, LOW);
      fanCount = 0;
      fanvar=0;
      senddatatoweb("fan");
      senddatatoweb("fanvar");
      delay(500);
    }
  }

  if(digitalRead(fanup)==1 && fanvar<4){
    fanvar++;
    senddatatoweb("fanvar");
    delay(500); 
  }

  if(digitalRead(fandown)==1 && fanvar>0){
    fanvar--;
    senddatatoweb("fanvar");
    delay(500);
  }


  switch(fanvar){
    case 0:
            digitalWrite(fans1,LOW);
            digitalWrite(fans2,LOW);
            digitalWrite(fans3,LOW);
            digitalWrite(fanled1,LOW);
            digitalWrite(fanled2,LOW);
            digitalWrite(fanled3,LOW);
            digitalWrite(fanled4,LOW);
            break;


    case 1:
            digitalWrite(fans1,HIGH);
            digitalWrite(fans2,LOW);
            digitalWrite(fans3,LOW);
            digitalWrite(fanled1,HIGH);
            digitalWrite(fanled2,LOW);
            digitalWrite(fanled3,LOW);
            digitalWrite(fanled4,LOW);
            break;

    
    case 2:
            digitalWrite(fans1,LOW);
            digitalWrite(fans2,HIGH);
            digitalWrite(fans3,LOW);
            digitalWrite(fanled1,HIGH);
            digitalWrite(fanled2,HIGH);
            digitalWrite(fanled3,LOW);
            digitalWrite(fanled4,LOW);
            break;
    case 3:
            digitalWrite(fans1,HIGH);
            digitalWrite(fans2,HIGH);
            digitalWrite(fans3,LOW);
            digitalWrite(fanled1,HIGH);
            digitalWrite(fanled2,HIGH);
            digitalWrite(fanled3,HIGH);
            digitalWrite(fanled4,LOW);
            break;

    case 4:
            digitalWrite(fans1,LOW);
            digitalWrite(fans2,LOW);
            digitalWrite(fans3,HIGH);
            digitalWrite(fanled1,HIGH);
            digitalWrite(fanled2,HIGH);
            digitalWrite(fanled3,HIGH);
            digitalWrite(fanled4,HIGH);
            break;       
  }
  


 



  //set pin on off by server code
  if (lightCount == 1 ) {
    digitalWrite(lightout, HIGH);
  }
  else {
    digitalWrite(lightout, LOW);
  }
  if (fanCount == 1 ) {
    digitalWrite(fans1, HIGH);
    digitalWrite(fanled1,HIGH);
    digitalWrite(fanled2,LOW);
    digitalWrite(fanled3,LOW);
    digitalWrite(fanled4,LOW);
  }
  else {
    digitalWrite(fans1, LOW);
  }
  if (acCount == 1 ) {
    digitalWrite(acout, HIGH);
  }
  else {
    digitalWrite(acout , LOW);
  }

  
  if(dellay == 0){
      varupdate();
      dellay=10000;
    }
    dellay--;
}



void handleMessage(WebsocketsMessage message) {
  Serial.println(message.data());
  String data = message.data();
  String status = parseData(data, 1);
  equ = parseData(data, 2);
  if (equ == "lightCount") {
    lightCount = status.toInt();
  }
  else if (equ == "fanCount")
  {
    fanCount = status.toInt();
  }
  else if (equ == "acCount")
  {
    acCount = status.toInt();
  }
  else if (equ == "fanvar")
  {
    fanvar = status.toInt();
  }

}


void varupdate(){
  socket.send("update:" + String(lightCount) + ":" + String(fanCount) + ":" +String(acCount)+":"+String(fanvar));
}


void handleEvent(WebsocketsEvent event, WSInterfaceString data) { }



String parseData(String data, int index) {
  String result = "";
  int currentIndex = 1;
  int start = 0;
  int end = data.indexOf(":");

  while (end != -1 && currentIndex <= index) {
    if (currentIndex == index) {
      result = data.substring(start, end);
      break;
    }

    start = end + 1;
    end = data.indexOf(":", start);
    currentIndex++;
  }

  return result;
}


void senddatatoweb(String equ) {
  if (equ == "light") {
    socket.send(String(lightCount) + ":light");
  }
  else if (equ == "fan") {
    socket.send(String(fanCount) + ":fan");
  }
  else if (equ == "ac") {
    socket.send(String(acCount) + ":ac");
  }
  else if (equ == "fanvar") {
    socket.send(String(fanvar) + ":fanvar");
  }
}

void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}
