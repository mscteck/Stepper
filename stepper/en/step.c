///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                            STEPPER MOTOR CONTROL                          //
//                          CREATED BY MARCOS CABRERA                        //
//                                04 APRIL 2019                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                               HOW TO USE IT:                              //
//                                                                           //
//  The library works basically by loading a int1 variable                   //
//  into the pins that drive the motor.                                      //
//  Its possible to work the stepper in FULL STEP, HALF STEP,                //
//  and WAVE DRIVE.                                                          //
//  The pins used to control the motor can be easily changed,                //
//  and also the matrix used to create the rutines.                          //
//                                                                           //
//  The init_stepper() function just puts it on a right                      //
//  angle and set the default values for some variables,                     //
//  but its not 100% necessary.                                              //
//                                                                           //
//  The value for the pins on each step are stored                           //
//  in an array of int1 variables called 'matrix'                            //
//  and they can be easily accessed                                          //
//  by the following equation:                                               //
//                                                                           //
//   PIN STATE = MODE + DIRECTION + ( STEP * 4 ) + PIN                       //
//                                                                           //
//  On this equation, MODE and DIRECTION points                              //
//  where starts the configuration. STEP * 4 is                              //
//  used to navigate on that configuration. And                              //
//  the number of the pin access the exact value.                            //
//                                                                           //
//  For example, if im working in FULL STEP and                              //
//  ANTICLOCK direction, it means that the config                            //
//  will start at 0+12=12. If its the 2nd step that                          //
//  im on (using 0 to 3), it'll add 1*4=4.                                   //
//  That will let me with a total of 16,                                     //
//  on the start of this: '0,0,1,1,'.                                        //
//  So i just add the pin that i wanna get the value                         //
//  (again using 0 to 3), for example 1, and that way                        //
//  i get the exact value for that pin on that step                          //
//  for that configuration and direction.                                    //
//  Easy, isn't it? ;)                                                       //
//                                                                           //
//  Just a few more things.                                                  //
//  The pins can be changed on the 'stepper_pins'                            //
//  variable. Any digital pin can be used.                                   //
//  The int1 called inverted its to do exactly that,                         //
//  invert the state of the pins. Just in case that                          //
//  you use a driver like ULN2803.                                           //
//  Turn it to 0 to use it in normal states.                                 //
//                                                                           //
//  The speed set use a simple delay_ms.                                     //
//  As a recomendation, keep it between 10 and 100 ms.                       //
//  But it depends on what motor driver you use.                             //
//                                                                           //
//  Now thats all, enjoy it, and if you find                                 //    
//  something to improve, feel free to share it.                             //
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
