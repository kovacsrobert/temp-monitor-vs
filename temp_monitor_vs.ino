
#include <Adafruit_MCP9808.h>
#include <ESP8266WiFi.h>
#include <FS.h>
#include <ArduinoJson.h>

const char* configFilePath = "/config.json";

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
JsonObject* config;

void setup()
{
	Serial.begin(9600);
	setupTempSensor();
	setupFileSystem();
	setupConfig();
	setupWifi();
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

void setupWifi() {
	JsonObject& json = *config;
	const char* ssid = json["ssid"];
	const char* pass = json["pass"];
	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		/*Serial.print("Connecting to Wifi, SSID: "); Serial.print(ssid); Serial.print(", PASS: "); Serial.println(pass);*/
		Serial.print("Connecting to Wifi...");
		delay(1000);
	}
	Serial.println("Connected to Wifi");
}

void setupFileSystem() {
	Serial.println("Mounting file system...");
	if (!SPIFFS.begin()) {
		Serial.println("Cannot mount file system!");
		while (1);
	}
	Serial.println("Mounted file system");
}

void setupConfig() {
	if (!loadConfig(configFilePath)) {
		Serial.println("Failed to load configuration!");
		while (1);
	}
	Serial.println("Configuration loaded");
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
	config = &jsonBuffer.parseObject(buf.get());

	if (!config->success()) {
		Serial.println("Failed parsing config json");
		config = NULL;
		return false;
	}

	return true;
}