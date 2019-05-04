///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                            STEPPER MOTOR CONTROL                          //
//                          CREADA POR MARCOS CABRERA                        //
//                              en PIC C COMPILER                            //
//                                04 APRIL 2019                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               HOW TO USE IT:                              //
//                                                                           //
//  La libreria funciona basicamente cargando una variable                   //
//  int1 en los pines que controlan el motor.                                //
//  Se puede trabajar en FULL STEP, HALF STEP, y WAVE DRIVE.                 //
//  Los pines de control se pueden cambiar facilmente,                       //
//  asi como la matriz usada para almacenar las configuraciones.             //
//                                                                           //
//  La funcion init_stepper() solo pone el motor en angulo                   //
//  recto y setea los valores por defecto de algunas variables               //
//  pero no es 100% necesaria.                                               //
//                                                                           //
//  El valor de cada pin en cada paso esta guardado                          //
//  en un array de variables int1 llamado 'matrix'                           //
//  que puede ser facilmente accesado por la siguiente                       //
//  ecuacion:                                                                //
//                                                                           //
//   PIN STATE = MODO + DIRECCION + ( PASO * 4 ) + PIN                       //
//                                                                           //
//  En esta ecuacion, MODO y DIRECCION señalan                               //
//  donde inicia la configuracion. PASO * 4 es                               //
//  usado para navegar en esa configuration. Y el                            //
//  numero de pin accesa el valor exacto para su estado.                     //
//                                                                           //
//  Por ejemplo, si estoy trabajando en FULL STEP y                          //
//  en direccion ANTICLOCK, significa que la configuracion                   //
//  empezara en 0+12=12. Si es el 2ndo paso que doy                          //
//  (usando escala de 0 to 3), va a sumar 1*4=4.                             //
//  Eso nos deja un total de 16, en el inicio                                //
//  de esto: '0,0,1,1,'.                                                     //
//  Entonces, solo añado el pin del cual quiero obtener                      //
//  el estado (de nuevo usando 0 a 3), por ejemplo 1, y de                   //
//  esa forma obtengo el valor exacto para ese pin, en ese                   //
//  paso y para esa configuracion y direccion.                               //
//  Facil no? ;)                                                             //
//                                                                           //
//  Solo un par de cosas mas.                                                //
//  Los pines se cambian en la variable 'stepper_pins'                       //
//  Se puede usar cualquier pin digital.                                     //
//  La variable int1 llamada inverted hace exactamente eso,                  //
//  invierte el estado de los pines. Esto en caso de que                     //
//  se trabaje con un driver como el ULN2803.                                //
//  Si se quiere usar en forma normal ponerlo en cero.                       //
//                                                                           //
//  El seteo de velocidad usa solo un delay_ms.                              //
//  Recomiendo mantenerlo entre 10 y 100 ms, pero                            //
//  Depende del driver que usen para el motor.                               //
//                                                                           //
//  Ahora si, eso es todo, disfutenlo, y si encuentran                       //    
//  como mejorar algo de la libreria, por favor compartan.                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#define FULL_STEP  0
#define HALF_STEP  28
#define WAVE_DRIVE 88
#DEFINE CLOCKWISE  0
#define ANTICLOCK  12

int1 inverted = 1;
int step=0;
int speed = 10;
int mode = 0;
int32 stepper_pins[4] = PIN_C4, PIN_C5, PIN_C6, PIN_C7;

int1 matrix[116]=  1,1,0,0,   //FULL STEP    0
                   0,1,1,0,
                   0,0,1,1,
                   1,0,0,1,   //shared row for both directions 12
                   0,0,1,1,
                   0,1,1,0,
                   1,1,0,0,   //END OF FULL STEP
                   1,0,0,0,   //INIT HALF STEP     28
                   1,1,0,0,
                   0,1,0,0,
                   0,1,1,0,
                   0,0,1,0,
                   0,0,1,1,
                   0,0,0,1,
                   1,0,0,1,   //shared row for both directions 56
                   0,0,0,1,
                   0,0,1,1,
                   0,0,1,0,
                   0,1,1,0,
                   0,1,0,0,
                   1,1,0,0,
                   1,0,0,0,   //END OF HALF STEP
                   1,0,0,0,   //WAVE DRIVE STEP    88
                   0,1,0,0,
                   0,0,1,0,
                   0,0,0,1,   //shared row for both directions 100
                   0,0,1,0,
                   0,1,0,0,
                   1,0,0,0;   //                   112
               
               
void init_stepper(){
   output_bit(stepper_pins[0], 1);
   output_bit(stepper_pins[1], 0);
   step=0;
   speed = 10;
   delay_ms(10);
}

void move_stepper(int direction, int16 steps_count){
      int index;
      int counter1, counter2;
      for( counter1=1; counter1<=steps_count; counter1++ ){
         for(counter2=0;counter2<4;counter2++){
            if((mode==HALF_STEP)&&(direction==12)){
               direction=28;
            }
            index=mode+direction+(step*4)+counter2;
            if(inverted){
               output_bit( stepper_pins[counter2], !matrix[index] );
            }  else{
               output_bit( stepper_pins[counter2], matrix[index] );
            }
         }
         if(mode==HALF_STEP){
            if(step==7){
               step=0;
            }  else{
               step++;
            }
         }  else{
            if(step==3){
               step=0;
            }  else{
               step++;
            }
         }
         delay_ms( 10+speed );
      }
}
