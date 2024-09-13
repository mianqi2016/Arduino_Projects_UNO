/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows the basic function of library for HUSKYLENS via I2c.
 
 Created 2020-03-13
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/

#include "HUSKYLENS.h"
#include "Wire.h"

HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
void printResult(HUSKYLENSResult result);

int idGot = 0;
int counter = 0;
const unsigned long VERIFYINTERNAL = 10;
unsigned long VERIFYTIME = 0;
int widthCurrent = 0, heightCurrent = 0;
int width = 0, height = 0;
const int INCREASEMENT = 10;
#define SPEAKER 12
#define BEEP 11

void setup() {
    Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    pinMode(SPEAKER, OUTPUT);
    pinMode(BEEP, OUTPUT);
}

void loop() {
    if (!huskylens.request()) 
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) 
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    else if(!huskylens.available()) 
        {
          Serial.println(F("No block or arrow appears on the screen!"));
          width = 0;  // when colorId == 1 is out of field of vision.
          height = 0; //!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
        }   
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            if (result.command == COMMAND_RETURN_BLOCK){
            Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
            //delay(1000);
              idGot = result.ID;
              if(idGot == 1){
                //  if( (result.width> 60) || (result.height > 60)  ){
                if( ((result.width - width) > INCREASEMENT) || ((result.height - height) > INCREASEMENT)  ){
                  //Serial.println(result.width - width);
                  //Serial.println(result.height - height);
                    speaking();
                    width = result.width;
                    height = result.height;
                    delay(VERIFYINTERNAL);
                  } 
                 else
                    keepSilence(); 
               } 
              else
                  keepSilence();     
                
              }        
            //printResult(result);
        }  // end of while...
          
    }
    
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
        delay(1000);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}

void speaking(void){
      digitalWrite(SPEAKER, HIGH);
      digitalWrite(SPEAKER, LOW);
      digitalWrite(BEEP, HIGH);
      delay(10);
      digitalWrite(BEEP, LOW);
}

void keepSilence(void){
      digitalWrite(SPEAKER, LOW);
      digitalWrite(BEEP, LOW);
}

//void sampling(void){
//      delay(VERIFYINTERNAL);
//      width = result.width;
//      height = result.height;
//}
