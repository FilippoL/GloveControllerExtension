
#include "MicroBit.h"

MicroBit uBit;
#define DELAY 100

void Init();
void Loop();
void Release();
long map(long x, long in_min, long in_max, long out_min, long out_max);

int main()
{
    Init();
    Loop();
    Release();
}

void Init(){
     // Initialize micro:bit runtime.
    uBit.init();
    uBit.serial.baud(9600);
    }

void Loop(){

    const float VCC = 3.3; // Measured voltage of Ardunio 5V line
    const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor

    MicroBitPin P0(MICROBIT_ID_IO_P0, MICROBIT_PIN_P0, PIN_CAPABILITY_ANALOG);
    MicroBitPin P1(MICROBIT_ID_IO_P1, MICROBIT_PIN_P1, PIN_CAPABILITY_ANALOG);
    MicroBitPin P3(MICROBIT_ID_IO_P3, MICROBIT_PIN_P3, PIN_CAPABILITY_ANALOG);
    MicroBitPin P4(MICROBIT_ID_IO_P4, MICROBIT_PIN_P4, PIN_CAPABILITY_ANALOG);
    MicroBitPin P10(MICROBIT_ID_IO_P10, MICROBIT_PIN_P10, PIN_CAPABILITY_ANALOG);

    // Upload the code, then try to adjust these values to more
    // accurately calculate bend degree.
    const float STRAIGHT_RESISTANCE = 22.0; // resistance when straight
    const float BEND_RESISTANCE = 7.0; // resistance at 90 deg

    int value;
    bool sending = true;
    bool running = true;

     while(running)
    {
         if (uBit.buttonA.isPressed())
        {
            //sending = true;
        }
         else if (uBit.buttonB.isPressed())
        {
            uBit.serial.send("STOP");
            uBit.sleep(DELAY);
            sending = false;
        }

         if (uBit.buttonAB.isPressed() && running == true)
        {
            uBit.serial.send("SHUTDOWN");
            uBit.sleep(DELAY);
            running = false;
        }
         if (sending)
         {

            float flexV = value * VCC / 1023.0; //voltage
            float flexR = R_DIV * (VCC / flexV - 1.0); //resistance
            int angles [5];
            angles[0]= map(P1.getAnalogValue(), STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);
            angles[1]= map(P0.getAnalogValue(), STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);
            angles[2]= map(P3.getAnalogValue(), STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);
            angles[3]= map(P4.getAnalogValue(), STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);
            angles[4]= map(P10.getAnalogValue(), STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);



            uBit.serial.send("START");

            uBit.serial.send(" \n");
            uBit.serial.send("b");
            uBit.serial.send(angles[0]);
            uBit.serial.send("e");

            uBit.serial.send(" \n");
            uBit.serial.send("b");
            uBit.serial.send(angles[1]);
            uBit.serial.send("e");

            uBit.serial.send(" \n");
            uBit.serial.send("b");
            uBit.serial.send(angles[2]);
            uBit.serial.send("e");

            uBit.serial.send(" \n");
            uBit.serial.send("b");
            uBit.serial.send(angles[3]);
            uBit.serial.send("e");

            uBit.serial.send(" \n");
            uBit.serial.send("b");
            uBit.serial.send(angles[4]);
            uBit.serial.send("e");
            uBit.serial.send(" \n");

            uBit.serial.send("b");
            uBit.serial.send(uBit.accelerometer.getX());
            uBit.serial.send("e");
            uBit.serial.send(" \n");

            uBit.serial.send("b");
            uBit.serial.send(uBit.accelerometer.getY());
            uBit.serial.send("e");
            uBit.serial.send(" \n");

         }
    }

}

void Release(){
    release_fiber();
    }

long map(long x, long in_min, long in_max, long out_min, long out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
