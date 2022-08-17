#include "mbed.h"
#include "TextLCD.h"
#include "Keypad.h"
#include "HCSR04.h"
#include <stdio.h>
TextLCD my_lcd(PTE20, PTE21, PTE22, PTE23, PTE29, PTE30, TextLCD::LCD16x2);
Keypad kpad(PTA12, PTD4, PTA2, PTA1, PTC9, PTC8, PTA5, PTA4);
HCSR04 sensor(PTC13, PTC12);


long distance_2;
long distance;
    
PwmOut led_red(PTA13);
PwmOut led_green(PTD5);


PwmOut ledGreen(LED_GREEN);
PwmOut ledRed(LED_RED);
PwmOut ledBlue(LED_BLUE);


SPI spi(PTD2, PTD3, PTD1);          // Arduino compatible MOSI, MISO, SCLK
DigitalOut cs(PTD0);                // Chip select


const unsigned char led2[]= {
    0x01,0x01,0x01,0x71,0x71,0x01,0x01,0x01
};  //less far car
const unsigned char led3[]= {
    0x01,0x01,0x01,0x39,0x39,0x01,0x01,0x01
};  //less less far car

const unsigned char led4[]= {
    0x01,0x01,0x01,0x1D,0x1D,0x01,0x01,0x01
};  //less less less far car

const unsigned char led5[]= {
    0x01,0x01,0x01,0x0F,0x0F,0x01,0x01,0x01
};  //car that hits


/// Send two bytes to SPI bus
void SPI_Write2(unsigned char MSB, unsigned char LSB)
{
    cs = 0;                         // Set CS Low
    spi.write(MSB);                 // Send two bytes
    spi.write(LSB);
    cs = 1;                         // Set CS High
}

/// MAX7219 initialisation
void Init_MAX7219(void)
{
    SPI_Write2(0x09, 0x00);         // Decoding off
    SPI_Write2(0x0A, 0x08);         // Brightness to intermediate
    SPI_Write2(0x0B, 0x07);         // Scan limit = 7
    SPI_Write2(0x0C, 0x01);         // Normal operation mode
    SPI_Write2(0x0F, 0x0F);         // Enable display test
    wait_ms(500);                   // 500 ms delay
    SPI_Write2(0x01, 0x00);         // Clear row 0.
    SPI_Write2(0x02, 0x00);         // Clear row 1.
    SPI_Write2(0x03, 0x00);         // Clear row 2.
    SPI_Write2(0x04, 0x00);         // Clear row 3.
    SPI_Write2(0x05, 0x00);         // Clear row 4.
    SPI_Write2(0x06, 0x00);         // Clear row 5.
    SPI_Write2(0x07, 0x00);         // Clear row 6.
    SPI_Write2(0x08, 0x00);         // Clear row 7.
    SPI_Write2(0x0F, 0x00);         // Disable display test
    wait_ms(500);                   // 500 ms delay
}


int main() {

    cs = 1;                         // CS initially High
    spi.format(8,0);                // 8-bit format, mode 0,0
    spi.frequency(1000000);         // SCLK = 1 MHz
    Init_MAX7219();                 // Initialize the LED controller
    
ledBlue = 1;
ledGreen = 1;
ledRed = 1;

my_lcd.printf("Enter The Dist:");
my_lcd.locate(0,1);
char key;
int released = 1;
char inputArray[16];
int i = 0 ;
int dig1;
  int c = 0;




while(i<16){
    key = kpad.ReadKey(); //read the current key pressed
    if(key == '\0')
    released = 1; //set the flag when all keys are released
    if((key != '\0') && (released == 1)) { //if a key is pressed AND previous key was released
    if (key == 'A'){
        break;
    }  

    
    else{
     inputArray[c] = key;
     c++;
     my_lcd.printf("%c", key);
     }
     released = 0;
        }
     }
              sscanf(inputArray, "%d%", &dig1);
          while(1) {
                 
        long distance = sensor.distance(CM); 
                 wait(1.0);
        long distance_2 = sensor.distance(CM);
        

            my_lcd.cls();  
            my_lcd.printf("CM  %d  \n",distance);
             if ((distance > 0  ) &&(distance<=dig1)) { 
                 
                                       
                    if ((distance >=(dig1/3)  ) &&(distance<=((dig1+dig1)/3))){
                     led_green=0;
                         led_red=1;
                         wait(0.05f);
                         led_red=0.8;
                         wait(0.05f);
                         led_red=0.6;    
                         wait(0.05f);
                         led_red=0.4;
                         wait(0.05f);
                         led_red=0.2;
                         wait(0.05f);
                         
                         
                         }
                     else if ((distance > 0  ) &&(distance<(dig1/3))) {
                         led_red=1;
                         led_green=0;
                         }
                    else {
                        my_lcd.printf("Very close!!");
                    led_red=0;
                    led_green=1;
                     }
                     }
                    
                 else if ((distance > 0  )  &&(distance>=dig1))
                 {
                my_lcd.printf("Not Close  :((");
                led_red=0;
                led_green=0;
                } 
      if (distance<distance_2) { // getting further away
        ledBlue=0; 
        ledGreen=1;
        ledRed=1;  
            for(int i=1; i<9; i++)      // Write first character (8 rows)
            SPI_Write2(i,led3[i-1]);
                wait(0.2);  
                                  // 1 sec delay
        for(int i=1; i<9; i++)      // Write second character

            SPI_Write2(i,led2[i-1]);
            wait(0.2); 
             
       
                        }
    else if (distance>distance_2) { // getting closer
        ledBlue=1;
        ledGreen=0;
        ledRed=0; 
        for(int i=1; i<9; i++)      // Write second character
            SPI_Write2(i,led4[i-1]);
        wait(0.2); 
                for(int i=1; i<9; i++)      // Write second character
            SPI_Write2(i,led5[i-1]);
        wait(0.2);

}
                    else {
                         ledGreen=1;
                         ledRed=1;
                         ledBlue=1; 
        }       

}
}
