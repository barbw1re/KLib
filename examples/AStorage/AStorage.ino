/**
 * This example should be run twice.
 *
 * The first time will store the data,
 * and the second time retrieve and display
 * them on the Serial Monitor.
 */

#include <Arduino.h>
#include <AStorage.h>

struct Data {
    int a;
    long b;
    char name[20];
};

AStorage storage;

void setup()
{
    Serial.begin(9600);

    Data data;
    storage.Read(&data, sizeof(Data));
    if (data.a != 113) {
        // First run - let's set some data
        Serial.println(F("No data found in storage"));
        data.a = 113;
        data.b = 85650L;
        strcpy(data.name, "It is me");
        storage.Write(&data, sizeof(Data));
    }
    else {
        // Found data
        Serial.println(F("Found data:"));
        Serial.print(F("a: "));
        Serial.println(data.a);
        Serial.print(F("b: "));
        Serial.println(data.b);
        Serial.print(F("name: "));
        Serial.println(data.name);
    }
}

void loop()
{
}
