#include <SoftwareSerial.h>                                    //Xbee Library
                                                               //Pin Interior Luz 6-9
#define interior1 10
#define interior2 11
#define interior3 12
#define interior4 13
                                                              //Pin Exterior Luz 2-5
#define exterior1 6
#define exterior2 7
#define exterior3 8
#define exterior4 9
                                                             //Puertos de entrada y salida asignados al Xbee
#define pinRX 2
#define pinTX 3
                                                             //Establecer puertos en el Xbee
SoftwareSerial Xbee(pinRX, pinTX);
                                                            //Variables que determinan el tiempo
int minutos, segundos;
                                                            //El valor de ID del Arduino
String ardID;
                                                            //Cuenta cuantas veces se ha entrado al arduino
int count = 0;

char action;
int light;
int luzSensor[15];
int temp = 0;


void setup() {
                                                           //Inicializar conexion Xbee
  Xbee.begin(9600); 
                                                           //Puertos del 2 al 9 de Salida
  pinMode(interior1, OUTPUT);
  pinMode(interior2, OUTPUT);
  pinMode(interior3, OUTPUT);
  pinMode(interior4, OUTPUT);

  pinMode(exterior1, OUTPUT);
  pinMode(exterior2, OUTPUT);
  pinMode(exterior3, OUTPUT);
  pinMode(exterior4, OUTPUT);

  segundos = 0, minutos = 0;
  
}

void loop() {
                                                           //Timer
    if(++segundos > 59){
                  segundos = 0;
                  if(++minutos > 1439){
                    minutos = 0; 
                  }
                }
                                                          //Checa si hay algo disponible para lectura en el Xbee
    if(Xbee.available()>0){
                                                          //Si es la primera vez que se entra al arduino se guarda el valor en la variable ardID             
              if ( count == 0 ){
                ardID = Xbee.readString();
                                                          //Si ardID == A1 significa que es el arduino correcto
                if ( ardID == "A1" ){
                  Xbee.println(ardID);
                  count = 1;
                }
                                                          //Si no es correcto el count vuelve a 0 a esperar que el valor sea correcto.
                else {
                  count = 0;
                }
              }
                                                          //La segunda vez se guarda el numero de luz que se desea acceder
              else if ( count == 1 ){

                light = Xbee.parseInt();
                count = 2;
                 }                                       //Si es la 3 vez guardo la accion que se desea.
              else if ( count == 2){
                action = Xbee.read();               
                  Xbee.println(action);
                   count = 0;
                  if (action == '3'){                   //Si la opcion es la del sensor, Se va a guardar en el arreglo sensor en el valor de la luz un 1, para que el sensor sepa cuales luces va a admin.
                       luzSensor[light]= 1;
                      
                  }
                              
           }
          
      }
                                                        // Maquina de estados de opciones de encendido y apagado          
     switch(action){
              case '1':                                 //Si es el 1. Se enciende el led
                          digitalWrite(light, HIGH ); 
                          luzSensor[light]= 0;  
                                            
              break;
              case '2':                                //Si es el 2. Se apaga el led
                          digitalWrite(light, LOW ); 
                          luzSensor[light]= 0;
                                            
              break;
                   
              default:
              break;
              
            
          }
                                                      //Lectura Análoga. Si el sensor está sobre 500 se busca en el arreglo luzSensor, para checar de las luces cual se estableció que fuera por sensor.
          if(analogRead(0) < 500){                    //Se enciende
                     for(int i=5; i < 14; i++){
                        if( luzSensor[i] == 1){
                          digitalWrite(i, HIGH);
                        }
                      }
                    }
                                                    // Si es mayor de 500 se apagan aquellas luces que tengan 1 en el arreglo.
                    else if(analogRead(0) >= 500){
                     for(int i=5; i < 14; i++){
                        if( luzSensor[i] == 1){
                          digitalWrite(i, LOW);
                        }
                      }
                    } 
                  }
