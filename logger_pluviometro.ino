// *************************************************************  Bibliotecas *********
//      Relog de tiempo real
#include "RTClib.h"

//      Tarjeta SD
#include <SPI.h>
#include <SD.h>

// Sensor DTH
#include "DHT.h"

// ************************************************************************************
// *************************************************************  Objetos *************

//      Reloj de tiempo real
RTC_DS3231 rtc; 

//      Sensor de temperatura y Humedad
#define DHTPIN 8     // Pin donde está conectado el sensor
#define DHTTYPE DHT22   // Sensor DHT22
DHT dht(DHTPIN, DHTTYPE);

// ************************************************************************************
// *************************************************************  Variables Globales **

// variable para el ISR
volatile boolean tic=false;


// ************************************************************************************
// *************************************************************  Setup ***************

void setup() {
// inicializa el sensor de Temperatura Humedad
  dht.begin();
// initializa el pin digital para el led como salida
  pinMode(9, OUTPUT);  
// inicializa el puerto serial
Serial.begin(9600);                                                              // puerto serial
// inicializa la tarjeta SD
Serial.print("Inicializando tarjeta SD ...");                                    // puerto serial
// Verifica que la tarjeta se encuentre y pueda ser inicializada:
if (!SD.begin(4)) 
{
    Serial.println("Error de tarjeta,o no existe");                              // puerto serial
    // detenemos la ejecusion:
    ctr_err(1,4);
}
  Serial.println("Tarjeta Inicializada");                                        // puerto serial

// setea la interrupcion
attachInterrupt(digitalPinToInterrupt(2),isr , FALLING); 

// registra el tiempo actual
unsigned long t1=millis()

} 

// ************************************************************************************
// *************************************************************  Funciones    ********

// funcion ISR de la interrupcion
void isr()
{
  tic=true;
}

// control de errores
void ctr_err(int c,int l )
{
  while (1)
    {
      //identificador del inicio de la secuencia de error
    delay(2000); // espera dos segundos
    for(int i=1;i<5;i++)
        {  
        digitalWrite(9, HIGH);   
        delay(100);                       
        digitalWrite(9, LOW);    
        delay(100);  
        }      
        delay(2000); // espera dos segundos
     // secuencia de identificacion de error      
        // pulsos cortos  
        for(int i=1;i<=c;i++)
        {  
        digitalWrite(9, HIGH);   
        delay(100);                     
        digitalWrite(9, LOW);    
        delay(500);   
        }
        // pulsos largos  
        for(int i=1;i<=l;i++)
        {  
        digitalWrite(9, HIGH);   
        delay(500);                     
        digitalWrite(9, LOW);    
        delay(500);   
        }
                         
     }
  }
  
// ************************************************************************************
// *************************************************************  Loop ****************

void loop()
{
  // si ocurre una interrupcion tic es verdadero
  if(tic)
  {
    tic=false; // resetea la variable de tic en el pluviometro
    Serial.print(rtc.now().unixtime());                                          // puerto serial
    // abre el archivo para escritura
    File dataFile = SD.open("prot01.dat", FILE_WRITE);
    // Si el archivo esta disponible
    if (dataFile)
     {
      long dataString=rtc.now().unixtime();
      dataFile.println(dataString);
      dataFile.close();
      // escribir tambien en el serial
      Serial.println(dataString);                                                // puerto serial
      // destella el led en cada tic del balancin
      digitalWrite(9, HIGH);   
      delay(100);                       
      digitalWrite(9, LOW);
     }
     else
     {
      Serial.println("error en el archivo");                                    // puerto serial
      ctr_err(2,3);
     }
  }
  
  If ((millis()-t1)>60000)
             {
              float h = dht.readHumidity(); //Leer la Humedad
              float t = dht.readTemperature(); //Leer la temperatura en grados Celsius
              long dataString=rtc.now().unixtime();
              File dataFile = SD.open("prot01.dat", FILE_WRITE);
              dataFile.print(dataString);
              dataFile.print(",")
              dataFile.print(h)
              dataFile.print(",")
              dataFile.print(t)
              dataFile.close();
              t1=millis()
             }
  }
  
}
// ************************************************************************************
// *************************************************************  FIN *****************
