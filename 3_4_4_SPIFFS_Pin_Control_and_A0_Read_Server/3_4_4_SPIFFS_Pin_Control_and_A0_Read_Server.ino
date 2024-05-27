/**********************************************************************
项目名称/Project          : 物理网智能家居
程序名称/Program name     : 3_4_4_SPIFFS_Pin_Control_and_A0_Read_Server
团队/Team                : 大创项目
作者/Author              : dezxu
日期/Date（YYYYMMDD）     : 20200210
程序目的/Purpose          : 
此程序用于演示如何通过网页控制智能家居并将智能家居状态实时显示于网页中。
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200210      dezxu           0.02          
20200211      dezxu          0.03          一
***********************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include<Servo.h>
#include <dht11.h>
#include <Wire.h>


# define THEdoor D8
# define pwmdeng D5
# define fankongzhi D7
# define WENSHI D3
#define RESHUIQI D6
# define LEDKONGZHI D4


ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
#include <DFPlayer_Mini_Mp3.h>       //加载MP3模块库文件



ESP8266WebServer esp8266_server(80);    // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
Servo myservo;  // 创建Servo对象用以控制伺服电机
     
void setup() {

   
    mp3_set_serial (Serial);        //设置MP3模块通讯方式
    mp3_set_volume (20);            //音量调节
    myservo.attach(THEdoor);  
    Serial.begin(9600);          // 启动串口通讯
    Serial.println("");
   Wire.begin(D1, D2);

   
  pinMode(pwmdeng, OUTPUT);
  pinMode(LEDKONGZHI, OUTPUT);   // 初始化NodeMCU控制板载LED引脚为OUTPUT
  digitalWrite(LEDKONGZHI, HIGH);// 初始化LED引脚状态
  pinMode(fankongzhi, OUTPUT); 
  digitalWrite(fankongzhi,HIGH);
  pinMode(RESHUIQI, OUTPUT);
  digitalWrite(RESHUIQI,HIGH);
  
  wifiMulti.addAP("dezheng", "xudezheng5566"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。

  int i = 0;  
  while (wifiMulti.run() != WL_CONNECTED) { // 尝试进行wifi连接。
    delay(1000);
    Serial.print(i++); Serial.print(' ');
  }
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP

  if(SPIFFS.begin()){                       // 启动闪存文件系统
    Serial.println("SPIFFS Started.");
  } else {
    Serial.println("SPIFFS Failed to Start.");
}    

   esp8266_server.on("/setdoor", handledoor);
   esp8266_server.on("/setLED1",handlePWM); 
   esp8266_server.on("/setLED", handleLED);
   esp8266_server.on("/setfan", handlefan);
   esp8266_server.on("/readADC", handleADC);
   esp8266_server.on("/readws", wenshidu);
   esp8266_server.on("/setreshui", handlereshui);  
   esp8266_server.on("/sethuahuo", handlehuahuo);
   esp8266_server.on("/setdoor", handledoor); //
   esp8266_server.onNotFound(handleUserRequest); // 处理其它网络请求
  // 启动网站服务
  esp8266_server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  esp8266_server.handleClient();
}
  
 byte x1 = 0,x2=1,z1=3,z2=4,n3=5,n4=6,n5=7,n6=8,n7=9,n8=10;
 void FU_weikai(){
  Wire.beginTransmission(8); /* begin with device address 8 */
  Wire.write("fuwei ");
  Wire.write(x1);/* sends hello string */
  Wire.endTransmission();    /* stop transmitting */

  }
  void JA_shui(){
  Wire.beginTransmission(8);
  Wire.write("jiasui ");
  Wire.write(x2);
  Wire.endTransmission();    
  }
  
void fang_wu(){
  Wire.beginTransmission(8);
  Wire.write("fangwu ");
  Wire.write(z1);
  Wire.endTransmission();    
  }

  void xunhuanyidong(){
  Wire.beginTransmission(8);
  Wire.write("fangwu ");
  Wire.write(z2);
  Wire.endTransmission();    
  }

  void jia_bi(){
  Wire.beginTransmission(8);
  Wire.write("jiabi ");
  Wire.write(n3);
  Wire.endTransmission();    
  }

  void fang_bo(){
  Wire.beginTransmission(8);
  Wire.write("jiabi ");
  Wire.write(n4);
  Wire.endTransmission();    
  }


  void hui_fu_3(){
  Wire.beginTransmission(8);
  Wire.write("huifu ");
  Wire.write(n5);
  Wire.endTransmission();    
  }

  void sanjiaobo(){
  Wire.beginTransmission(8);
  Wire.write("huifu ");
  Wire.write(n6);
  Wire.endTransmission();    
  }

void huahuo1(){
  Wire.beginTransmission(8);
  Wire.write("huahuo ");
  Wire.write(n7);
  Wire.endTransmission();    
  }

  void huahuo2(){
  Wire.beginTransmission(8);
  Wire.write("huahuo ");
  Wire.write(n8);
  Wire.endTransmission();    
  }
  
void handleLED() {//复位开关
 String ledState = "OFF";
 String LED_State = esp8266_server.arg("LEDstate"); //参考xhttp.open("GET", "setLED?LEDstate="+led, true);
 Serial.println(LED_State);
 
 if(LED_State == "1"){
  digitalWrite(LEDKONGZHI,LOW); //LED 点亮
  ledState = "ON"; //反馈参数
  //mp3_play(1);
  FU_weikai();
 } else {
  digitalWrite(LEDKONGZHI,HIGH); //LED 熄灭
  ledState = "OFF"; //反馈参数
  //mp3_play(2);
  JA_shui();
 }
 
 esp8266_server.send(200, "text/plain", ledState); //发送网页
}
void handlereshui() {//直线夹笔
 String reshuiState = "OFF";
 String reshui_State = esp8266_server.arg("reshuistate"); //参考xhttp.open("GET", "setLED?LEDstate="+led, true);
 Serial.println(reshui_State);
 
 if(reshui_State == "1"){
  digitalWrite(RESHUIQI,LOW); //LED 点亮
  reshuiState = "ON"; //反馈参数
  jia_bi();//mp3_play(8);
 } else {
  digitalWrite(RESHUIQI,HIGH); //LED 熄灭
  reshuiState = "OFF"; //反馈参数
  hui_fu_3();
  //mp3_play(9);
 }
 
 esp8266_server.send(200, "text/plain", reshuiState); //发送网页
}


void handlehuahuo() {//花活
 String huahuoState = "OFF";
 String huahuo_State = esp8266_server.arg("huahuostate"); //参考xhttp.open("GET", "setLED?LEDstate="+led, true);
 Serial.println(huahuo_State);
 
 if(huahuo_State == "1"){
  digitalWrite(RESHUIQI,LOW); //LED 点亮
  huahuoState = "ON"; //反馈参数
  huahuo1();//jia_bi();//mp3_play(8);
 } else {
  digitalWrite(RESHUIQI,HIGH); //LED 熄灭
  huahuoState = "OFF"; //反馈参数
  huahuo2();
  //hui_fu_3();
  //mp3_play(9);
 }
 
 esp8266_server.send(200, "text/plain", huahuoState); //发送网页
}


void handlefan() {//风扇
 String fanState = "OFF";
 String fan_State = esp8266_server.arg("fanstate"); //参考xhttp.open("GET", "setLED?LEDstate="+led, true);
 Serial.println(fan_State);
 
 if(fan_State == "1"){
  digitalWrite(fankongzhi,LOW); //LED 点亮
  fanState = "ON"; //反馈参数
 fang_wu(); //mp3_play(5);
 } else {
  digitalWrite(fankongzhi,HIGH); //LED 熄灭
  fanState = "OFF"; //反馈参数
  xunhuanyidong();
  //mp3_play(6);
 }
 
 esp8266_server.send(200, "text/plain", fanState); //发送网页
}

void handledoor(){//开关门
  String doorState="OFF";
  String door_State=esp8266_server.arg("doorstate");
  if(door_State == "1"){
    myservo.write(90);              // 告诉伺服电机达到'pos'变量的角度
  doorState = "ON"; //反馈参数
 // mp3_play(3);
  fang_bo();
 } else {
  myservo.write(0);              // 告诉伺服电机达到'pos'变量的角度
  doorState = "OFF"; //反馈参数
  sanjiaobo();
  //mp3_play(4);
 }
  esp8266_server.send(200, "text/plain", doorState); //发送网页
  }

void handleADC() {
 int a = analogRead(A0);
 String adcValue = String(a);
 if(a<300){
  mp3_play(7);}
 esp8266_server.send(200, "text/plain", adcValue); //发送模拟输入引脚到客户端ajax请求
}


 void wenshidu(){
  dht11 DHT11;//定义一个名字叫做DHT11，类型为dht11的值。
int DHT11PIN=WENSHI;//定义DHT11的引脚号为D3。
int chk = DHT11.read(DHT11PIN);

  // 测试 DHT 是否正确连接
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
    Serial.println("OK"); 
    break;
    case DHTLIB_ERROR_CHECKSUM: 
    Serial.println("Checksum error"); 
    break;
    case DHTLIB_ERROR_TIMEOUT: 
    Serial.println("Time out error"); 
    break;
    default: 
    Serial.println("Unknown error"); 
    break;
  }
  String wsValue=String((float)DHT11.temperature);
  esp8266_server.send(200, "text/plain", wsValue);
  }

  
// 处理用户浏览器的HTTP访问
void handleUserRequest() {         
     
  // 获取用户请求资源(Request Resource）
  String reqResource = esp8266_server.uri();
  Serial.print("reqResource: ");
  Serial.println(reqResource);
  
  // 通过handleFileRead函数处处理用户请求资源
  bool fileReadOK = handleFileRead(reqResource);

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK){                                                 
    esp8266_server.send(404, "text/plain", "404 Not Found"); 
  }
}

bool handleFileRead(String resource) {            //处理浏览器HTTP访问

  if (resource.endsWith("/")) {                   // 如果访问地址以"/"为结尾
    resource = "/index.html";                     // 则将访问地址修改为/index.html便于SPIFFS访问
  } 
  
  String contentType = getContentType(resource);  // 获取文件类型
  
  if (SPIFFS.exists(resource)) {                     // 如果访问的文件可以在SPIFFS中找到
    File file = SPIFFS.open(resource, "r");          // 则尝试打开该文件
    esp8266_server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}


void handlePWM(){
  String pwmStr = esp8266_server.arg("pwm"); // 获取用户请求中的PWM数值
  Serial.print("pwmStr = ");Serial.println(pwmStr);
  
  int pwmVal = pwmStr.toInt();              // 将用户请求中的PWM数值转换为整数
  pwmVal = 1023 - map(pwmVal,0,100,0,1023); // 用户请求数值为0-100，转为0-1023
  Serial.print("pwmVal = ");Serial.println(pwmVal);
  
  analogWrite(pwmdeng, pwmVal);         // 实现PWM引脚设置
  esp8266_server.send(200, "text/plain");//向客户端发送200响应信息
}


 
 

// 获取文件类型
String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
