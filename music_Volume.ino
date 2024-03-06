#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
/*
 
 * 1 - INC - Arduino pin 4
 * 2 - U/D - Arduino pin 6
 * 3 - VH  - 5V
 * 4 - VSS - GND
 * 5 - VW  - Output: 
 * 6 - VL  - GND
 * 7 - CS  - Arduino pin 10
 * 8 - VCC - 5V
 */

#include <DigiPotX9Cxxx.h>

// Define x9c103s module control pins for left and right channels
#define LEFT_INC_PIN 4
#define LEFT_UD_PIN 6
#define LEFT_CS_PIN 10 // not used
#define RIGHT_INC_PIN 5
#define RIGHT_UD_PIN 7
#define RIGHT_CS_PIN 11 // not used

DigiPot left_pot(LEFT_INC_PIN, LEFT_UD_PIN, LEFT_CS_PIN);
DigiPot right_pot(RIGHT_INC_PIN, RIGHT_UD_PIN, RIGHT_CS_PIN);
// Define DFPlayer Mini RX and TX pins
#define DFPLAYER_RX 3
#define DFPLAYER_TX 2

// Define PIR sensor pin
#define PIR_SENSOR_PIN 8

int leftResistance = 0;    // Initial resistance value for left channel
int rightResistance = 255; // Initial resistance value for right channel

SoftwareSerial DF1201SSerial(DFPLAYER_RX, DFPLAYER_TX);
DFRobot_DF1201S DF1201S;

bool motionDetected = false;

void setup()
{
    Serial.begin(115200);
    DF1201SSerial.begin(115200);

    // Initialize DFPlayer Mini
    while (!DF1201S.begin(DF1201SSerial))
    {
        Serial.println("DFPlayer Mini initialization failed. Check connection.");
        delay(1000);
    }

    // Set volume to 20
    DF1201S.setVol(20);

    // Enter music mode
    DF1201S.switchFunction(DF1201S.MUSIC);

    // Set playback mode to "repeat all"
    DF1201S.setPlayMode(DF1201S.ALLCYCLE);

    // Initialize PIR sensor pin
    pinMode(PIR_SENSOR_PIN, INPUT);

    // Initialize x9c103s module control pins for left and right channels
    pinMode(LEFT_INC_PIN, OUTPUT);
    pinMode(LEFT_UD_PIN, OUTPUT);
    pinMode(LEFT_CS_PIN, OUTPUT);
    pinMode(RIGHT_INC_PIN, OUTPUT);
    pinMode(RIGHT_UD_PIN, OUTPUT);
    pinMode(RIGHT_CS_PIN, OUTPUT);
    // Start playing
    DF1201S.start();
    Serial.print("The time length the current song has played:");
    // Get the time length the current song has played
    Serial.println(DF1201S.getCurTime());

    Serial.print("The total length of the currently-playing song: ");
    // Get the total length of the currently-playing song
    Serial.println(DF1201S.getTotalTime());
    Serial.print("The name of the currently-playing file: ");
}
bool left = true;
bool right = true;
void loop()
{
    // Check for motion
    if (digitalRead(PIR_SENSOR_PIN) == HIGH)
    {
        motionDetected = true;
        if (left)
        {
            Serial.println("motion detected transitioning to right");
            right = true;
            right_smoothTransition(); // Smoothly transition to right channel
            left = false;
        }
    }
    else
    {
        if (right)
        {
            Serial.println("motion not detected transitioning to left");
            left = true;
            left_smoothTransition(); // Smoothly transition to left channel
            right = false;
        }
    }

    if (DF1201S.getCurTime() >= DF1201S.getTotalTime())
    {
        DF1201S.start();
    }
}

void left_smoothTransition()
{
    for (int i = 0; i < 100; i++)
    {
        left_pot.increase(1);
        delay(20);
    }

    for (int i = 0; i < 100; i++)
    {
        right_pot.decrease(1);
        delay(20);
    }
}

void right_smoothTransition()
{
    for (int i = 0; i < 100; i++)
    {
        right_pot.increase(1);
        delay(20);
    }

    for (int i = 0; i < 100; i++)
    {
        left_pot.decrease(1);
        delay(20);
    }
}
// Smooth transition between channels
void smoothTransition()
{
    for (int i = 0; i < 255; i++)
    {
        if (motionDetected)
        {
            leftResistance = 255 - i; // Decrease resistance for left channel
            rightResistance = i;      // Increase resistance for right channel
            setResistance(LEFT_INC_PIN, LEFT_UD_PIN, LEFT_CS_PIN, leftResistance, HIGH);
            setResistance(RIGHT_INC_PIN, RIGHT_UD_PIN, RIGHT_CS_PIN, rightResistance, LOW);
            delay(10); // Adjust as needed for transition speed
        }
        else
        {
            leftResistance = 255 - i; // Decrease resistance for left channel
            rightResistance = i;      // Increase resistance for right channel
            setResistance(LEFT_INC_PIN, LEFT_UD_PIN, LEFT_CS_PIN, rightResistance, LOW);
            setResistance(RIGHT_INC_PIN, RIGHT_UD_PIN, RIGHT_CS_PIN, leftResistance, HIGH);
            delay(10); // Adjust as needed for transition speed
        }
    }
}

// Set resistance using x9c103s module
void setResistance(int incPin, int udPin, int csPin, int value, bool dir)
{
    digitalWrite(csPin, LOW); // Select the device
    delayMicroseconds(1);

    // Set direction (up or down)
    digitalWrite(udPin, dir); // Up direction

    // Increment/Decrement resistance value
    for (int i = 0; i < value; i++)
    {
        digitalWrite(incPin, HIGH); // Increment
        delayMicroseconds(10);      // Delay to ensure proper operation
        digitalWrite(incPin, LOW);  // End increment
        delayMicroseconds(10);      // Delay to ensure proper operation
    }

    digitalWrite(csPin, HIGH); // Deselect the device
}