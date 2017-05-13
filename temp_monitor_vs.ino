
#include "constants.h"
#include <Adafruit_MCP9808.h>

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup()
{
	Serial.begin(9600);
	setupTempSensor();
}

void loop()
{
	readTemperatureInCelsius();
	delay(1000);
}

void setupTempSensor() {
	if (!tempsensor.begin()) {
		Serial.println("Couldn't find MCP9808!");
		while (1);
	}
}

float readTemperatureInCelsius() {
	float c = tempsensor.readTempC();
	Serial.print("Temp: "); Serial.print(c); Serial.println("*C");
	return c;
}