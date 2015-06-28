#include <Adafruit_NeoPixel.h>

#define PIN    6                // Pin for neopixels
#define N_LEDS 150               // 30 meter test reel    

int ledPin = 13;                // choose the pin for the LED
int loc0Pin = 3;                // choose the input pin (for PIR sensor)
int loc1Pin = 4;
int pir0State = LOW;            // we start, assuming no motion detected
int pir1State = LOW;
int loc0val = 0;                // variable for reading the pin status
int loc1val = 1;

int indexPixel = 0;             // start pixel for animations
int destPixel = 10;             // destination pixel for animations
int hoverTime = 20;							// how long the sprite hovers for
int destVariance = 20;					// the randomness between hover locations

int hoverRandom;
int dir;												// +1 to move forwards, -1 to move backwards

long interval = 45;
long previousMillis = 0;        // will store last time LED was updated


Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);


 
void setup() 
{
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(loc0Pin, INPUT);     	// declare sensor as input
  pinMode(loc1Pin, INPUT);
	digitalWrite(ledPin, LOW);
  randomSeed(analogRead(0));
  strip.begin();
  strip.show();
}
 
void loop() 
{
	unsigned long currentMillis = millis();
	
  loc0val = digitalRead(loc0Pin);  	// read input loc0value
  if (loc0val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pir0State == LOW) {
      // we have just turned on
      chase(0x633051);
      // We only want to print on the output change, not state
      pir0State = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); 	// turn LED OFF
    if (pir0State == HIGH){
      
      // we have just turned off
      // We only want to print on the output change, not state
      pir0State = LOW;
    }
  }
	
  loc1val = digitalRead(loc1Pin);  	// read input loc0value
  if (loc1val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  		// turn LED ON
    if (pir1State == LOW) {
      // we have just turned on
      chase(0x552255);
      // We only want to print on the output change, not state
      pir1State = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); 			// turn LED OFF
    if (pir1State == HIGH){
      
      // we have just turned off
      // We only want to print on the output change, not state
      pir1State = LOW;
    }
  }
	
}


static void chase(uint32_t c) 
{
	indexPixel = 0;
	destPixel = indexPixel + random(destVariance,80) + 3; 
	strip.show();
	
	while (indexPixel < strip.numPixels()) {
	
		for(uint16_t i=indexPixel; i<destPixel; i++) {
		    strip.setPixelColor(i  , c); // Draw new pixel
        strip.setPixelColor(i-1, DimColor(strip.getPixelColor(i-1)));
        strip.setPixelColor(i-2, DimColor(strip.getPixelColor(i-2)));
        strip.setPixelColor(i-3, DimColor(strip.getPixelColor(i-3)));
        strip.setPixelColor(i-4, DimColor(strip.getPixelColor(i-4)));
        strip.setPixelColor(i-5, DimColor(strip.getPixelColor(i-5)));

		    strip.show();
		    delay(random(5,interval));
		}

		indexPixel = destPixel; // update current location
		hoverRandom = random(destVariance,80);
		destPixel = indexPixel + hoverRandom;
    hoverTime = random(5,40);

		for (int j=0; j<hoverTime; j++) {  //do 10 cycles of chasing
		  for (int q=0; q < 3; q++) {
          strip.setPixelColor(indexPixel-1, DimColor(strip.getPixelColor(indexPixel-1))); 
          strip.setPixelColor(indexPixel-2, DimColor(strip.getPixelColor(indexPixel-2)));
          strip.setPixelColor(indexPixel-3, DimColor(strip.getPixelColor(indexPixel-3)));
          strip.setPixelColor(indexPixel-4, DimColor(strip.getPixelColor(indexPixel-4)));
          strip.setPixelColor(indexPixel-5, DimColor(strip.getPixelColor(indexPixel-5)));    
		    for (int i=indexPixel; i < indexPixel+3; i=i+3) {
		      strip.setPixelColor(i+q, c);    //turn every third pixel on
		    }
		    strip.show();
   
		    delay(random(15,interval-10));
   
		    for (int i=indexPixel; i < indexPixel+3; i=i+3) {
		      strip.setPixelColor(i+q, 0x111111);        //turn every third pixel off
		    }
		  }
		}
	}  
}

// Calculate 50% dimmed version of a color (used by ScannerUpdate)
uint32_t DimColor(uint32_t color)
{
    // Shift R, G and B components one bit to the right
    uint32_t dimColor = strip.Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
    return dimColor;
}

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}
