
#include <Adafruit_MCP9808.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ArduinoJson.h>

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup()
{
	Serial.begin(9600);
	setupTempSensor();
	setupFileSystem();
}

void loop()
{
	readTemperatureInCelsius();
	loadConfig("/config.json");
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

void setupWifi() {
	/*Wifi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print("Connecting to Wifi, SSID: ");
		Serial.print(ssid);
		Serial.print(", PASS: ");
		Serial.println(pass);
		delay(1000);
	}*/
}

void setupFileSystem() {
	Serial.println("mounting FS...");
	if (!SPIFFS.begin()) {
		Serial.println("Cannot mount FS!");
		while (1);
	}
	Serial.println("Mounted file system");
}

boolean loadConfig(const char* filePath) {
	if (!SPIFFS.exists(filePath)) {
		Serial.println("Config file does not exist");
		return false;
	}

	File configFile = SPIFFS.open(filePath, "r");
	if (!configFile) {
		Serial.println("Failed to load config file");
		return false;
	}

	Serial.println("Config file loaded");
	size_t size = configFile.size();
	std::unique_ptr<char[]> buf(new char[size]);
	configFile.readBytes(buf.get(), size);
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed parsing config json");
		return false;
	}
	
	Serial.println("Parsed config json");
	const char* ssid = json["ssid"];
	Serial.print("SSID: "); Serial.println(ssid);

	return true;
}