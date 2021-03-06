/* Preserve a session acrosss RESET/STANDBY
 *  
 *  Using setSaveSession(true) signals the networking stack
 *  to save all state changes in EEPROM or BKUP registers.
 *  Upon a RESET there will be no join request to the gateway,
 *  but the old session will be continued.
 *    
 *  The code tries to be smart and detects a disconnect (or
 *  failure to continue the previous session), and uses
 *  rejoinOTAA() to rejoin.
 *    
 *      
 *  A good test case is to let a node connect to a gateway,
 *  wait a tad till ADR kicks in, and then press the RESET
 *  button on the node. The session should continue where
 *  it just left off.
 *  
 *  
 *  In setup() below please replace the argument to LoRaWAN.begin()
 *  with your appropriate region specific band:
 *
 *  AS923
 *  AU915
 *  EU868
 *  IN865
 *  KR920
 *  US915
 *
 *  AU915/US915 networks have 64+8 channels. Typical gateways support only
 *  8 (9) channels. Hence it's a good idea to pick the proper channel
 *  subset via select via LoRaWAN.setSubBand(),
 *    
 *  EU868/IN865 have duty cycle restrictions. For debugging it makes sense
 *  to disable those via setDutyCycle(false);
 *    
 *  For an external antenna one should set the proper antenna gain
 *  (default is 2.0) via setAntennaGain().
 *    
 *  Please edit the keys below as they are just debugging samples.
 *    
 *    
 * This example code is in the public domain.
 */

#include "LoRaWAN.h"

const char *appEui = "0101010101010101";
const char *appKey = "2B7E151628AED2A6ABF7158809CF4F3C";
const char *devEui = "0101010101010101";

void setup( void )
{
    Serial.begin(9600);
    
    while (!Serial) { }

    LoRaWAN.begin(US915);
    // LoRaWAN.setSubBand(2);
    // LoRaWAN.setDutyCycle(false);
    // LoRaWAN.setAntennaGain(2.0);
    LoRaWAN.setSaveSession(true);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

    Serial.println("JOIN( )");
}

void loop( void )
{
    if (!LoRaWAN.busy())
    {
        if (!LoRaWAN.linkGateways())
        {
            Serial.println("REJOIN( )");
            
            LoRaWAN.rejoinOTAA();
        }
        
        if (LoRaWAN.joined())
        {
            Serial.print("TRANSMIT( ");
            Serial.print("TimeOnAir: ");
            Serial.print(LoRaWAN.getTimeOnAir());
            Serial.print(", NextTxTime: ");
            Serial.print(LoRaWAN.getNextTxTime());
            Serial.print(", MaxPayloadSize: ");
            Serial.print(LoRaWAN.getMaxPayloadSize());
            Serial.print(", DR: ");
            Serial.print(LoRaWAN.getDataRate());
            Serial.print(", TxPower: ");
            Serial.print(LoRaWAN.getTxPower(), 1);
            Serial.print("dbm, UpLinkCounter: ");
            Serial.print(LoRaWAN.getUpLinkCounter());
            Serial.print(", DownLinkCounter: ");
            Serial.print(LoRaWAN.getDownLinkCounter());
            Serial.println(" )");
                
            LoRaWAN.beginPacket();
            LoRaWAN.write(0xef);
            LoRaWAN.write(0xbe);
            LoRaWAN.write(0xad);
            LoRaWAN.write(0xde);
            LoRaWAN.endPacket();
        }
    }

    delay(10000);
}
