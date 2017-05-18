
// Thank to Kimmo Karvinen & Tero Karvinen for getattentionention


const int tinyLedPin = 13;

// LED for level of concentration
const int led1 = 8;
const int led2 = 9;
const int led3 = 10;
const int led4 = 11;
const int led5 = 12;
const int moveForward = 4;

void setup()
{
        pinMode(tinyLedPin, OUTPUT);
        pinMode(led1, OUTPUT);
        pinMode(led2, OUTPUT);
        pinMode(led3, OUTPUT);
        pinMode(led4, OUTPUT);
        pinMode(led5, OUTPUT);
        
        pinMode(moveForward, OUTPUT);
        Serial.begin(115200); 
        connectHeadset();             
}

float attention = 0;
void loop()
{
    
    attention = getAttention();        
    if(attention > 0 && attention < .10){        
                digitalWrite(led1, LOW);
                digitalWrite(led2, LOW);
                digitalWrite(led3, LOW);
                digitalWrite(led4, LOW);
                digitalWrite(led5, LOW);
                digitalWrite(moveForward, LOW);
              }
    if(attention > .10 && attention < .30){        
                digitalWrite(led1, HIGH);
                digitalWrite(led2, LOW);
                digitalWrite(led3, LOW);
                digitalWrite(led4, LOW);
                digitalWrite(led5, LOW);
                digitalWrite(moveForward, LOW);
              }
                
    if(attention > 0.30 && attention < .50){
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                digitalWrite(led3, LOW);
                digitalWrite(led4, LOW);
                digitalWrite(led5, LOW);
                digitalWrite(moveForward, LOW);
    }
                
    if(attention > 0.50 && attention < .60){
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                digitalWrite(led3, HIGH);
                digitalWrite(led4, LOW);
                digitalWrite(led5, LOW);
                digitalWrite(moveForward, LOW);
    }
    if(attention > 0.70 && attention < .80){
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                digitalWrite(led3, HIGH);
                digitalWrite(led4, HIGH);
                digitalWrite(led5, LOW);
                digitalWrite(moveForward, HIGH);
    }
     
    if(attention > 0.80 && attention < .90){
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                digitalWrite(led3, HIGH);
                digitalWrite(led4, HIGH);
                digitalWrite(led5, LOW);
                digitalWrite(moveForward, HIGH);
    }
        
    if(attention > 0.90){
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                digitalWrite(led3, HIGH);
                digitalWrite(led4, HIGH);
                digitalWrite(led5, HIGH);
                digitalWrite(moveForward, HIGH);
    }
 
}

/*** Headset ***/

void connectHeadset()
{
        //setGreen();
        delay(3000);
        Serial.write(0xc2);        
        //setWhite();
}

byte readOneByte()
{
        while (!Serial.available()) {        // <7>
                delay(5); // ms
        };
        return Serial.read();
}

float getAttention()
{ // return attention percent (0.0 to 1.0); negative (-1, -2...) for error
        byte generatedChecksum = 0;        // <8>
        byte checksum = 0;
        int payloadLength = 0;
        byte payloadData[64] = {
                0
        };
        int poorQuality = 0;
        float attention = 0;

        Serial.flush();        // prevent serial buffer from filling up // <9>

        /* Sync */
        if (170 != readOneByte()) return -1;       // <10>
        if (170 != readOneByte()) return -1;

        /* Length */
        payloadLength = readOneByte();
        if (payloadLength > 169) return -2;        // <11>

        /* Checksum */
        generatedChecksum = 0;
        for (int i = 0; i < payloadLength; i++) {      // <12>
                payloadData[i] = readOneByte();        // Read payload into array
                generatedChecksum += payloadData[i];
        }
        generatedChecksum = 255 - generatedChecksum;
        checksum = readOneByte();
        if (checksum != generatedChecksum) return -3;    // <13>

        /* Payload */
        for (int i = 0; i < payloadLength; i++) {        // <14>
                switch (payloadData[i]) {
                case 0xD0:
                        //sayHeadsetConnected();
                        break;
                case 4:        // <15>
                        i++;        // <16>
                        attention = payloadData[i]; // <17>
                        break;
                case 2:
                        i++;
                        poorQuality = payloadData[i];
                        if (200 == poorQuality) {
                                //setYellow();        // <18>
                                return -4;
                        }
                        break;
                case 0xD1: // Headset Not Found
                case 0xD2: // Headset Disconnected
                case 0xD3: // Request Denied
                case -70:
                        return -5;
                        break;
                case 0x80:        // skip RAW        // <19>
                        i = i + 3;
                        break;
                case 0x83:        // skip ASIC_EEG_POWER
                        i = i + 25;
                        break;
                } // switch
        } // for

        return (float)attention / 100;        // <20>
}



