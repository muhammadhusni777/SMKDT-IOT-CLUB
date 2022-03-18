///////////////////////////////////////////////////////////////////
////////////////////////SMKDT IOT CLUB 2022////////////////////////
//////////////////////BLYNK PROJECT BENCHMARK /////////////////////
/////I REALLY REALLY HOPE THIS MAKE SANTRI DO IOT PROJECT EASIER///
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////

//memanggil library blynk dan wifi. jangan lupa di install dulu ////

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// mengaktifkan blynk timer dan widget led supaya dapat tampil di hp
BlynkTimer timer;
WidgetLED led1(V1);

//kode autentikasi dari blynk app. ini kode unik tiap hp akan berbeda beda
char auth[] = "j84EQooPn3U3rFeFvQyReC4piye-E0FU";

//masukkan wifi mana yang kamu mau connect
char ssid[] = "Teras Java Dimsum";
char pass[] = "siomayayam";


/*--------masukkan variabel yang dibutuhkan---------*/

//nama pin dan alamatnya
const int ledPin = 16;  

//variabel untuk analog write. 
const int freq = 5000;
const int ledChannel = 1;
const int resolution = 8;

//nama pin dan alamatnya
int sensor_pin = 5;

//sensor flag berfungsi mengunci kondisi sensor. jika tidak hp akan banyak notifikasi
int sensor_flag = 0;

/*------ void setup berfungsi untuk inisialisasi pertama (booting)----*/

void setup() {  

  //pin mode berfungsi untuk menentukan pin tersebut input atau output
  pinMode(sensor_pin, INPUT_PULLUP);

 //inisialisasi pin yang digunakan untuk analog write
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);


  //debug serial. untuk standalone standarnya 9600 atau 38400. untuk iot 115200
  Serial.begin(115200);

  //jeda 10 mili detik untuk konek ke wifi
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);


  //menghubungkan ke wifi. ini default jangan diganti ganti :)
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  

  //setelah wifi terhubung maka akan menghubungkan ke aplikasi blynk
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

//VOID LOOP SUDAH FIX SEPERTI INI JANGAN DITAMBAH TAMBAHIN NANTI ESP32 NGELAG
void loop(){
  
    Blynk.run();
    timer.run();
}

void myTimerEvent()
{
  //kalau mau menambahkan fungsi disini

  //membaca sensor infrared di pin D5 dan memberi notifikasi ke hp
  int sensor_value = !digitalRead(sensor_pin);
  if (sensor_value==1 && sensor_flag==0) {
    Blynk.notify("Alert : ada mudarris asup");
    sensor_flag=1;
    led1.on();
  }
  else if (sensor_value==0) {
    sensor_flag=0;
    led1.off();
  }

  //membaca sensor analog dan memberi datanya ke hp
  int sensor_analog = analogRead(35);
  Blynk.virtualWrite(V5, sensor_analog);
}

// membaca virtual pin (data dari hp)

BLYNK_WRITE(V2) {
  int V2_value = param.asInt();
}

BLYNK_WRITE(V1)
{
  int V1_value = param.asInt();
  ledcWrite(ledChannel, V1_value);
  Serial.println(V1_value);
  
}
