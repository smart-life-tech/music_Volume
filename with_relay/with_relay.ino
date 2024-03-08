#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>

// Define x9c103s module control pins for left and right channels
#define IN1 4
#define IN2 6

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
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

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
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void right_smoothTransition()
{
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, LOW);
}
