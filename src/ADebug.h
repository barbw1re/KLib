#ifndef _ADEBUG_H_
#define _ADEBUG_H_

#ifndef KLIB_ADEBUG
#define ADEBUG_SETUP(baud)
#define ADEBUG_ASSERT(condition)
#define ADEBUG_WRITE(message)
#define ADEBUG_PRINT(format, ...)
#define ADEBUG_START_BLOCK()
#define ADEBUG_END_BLOCK(name)
#else

#include <Arduino.h>

#define ADEBUG_SETUP(baud)        adbg_debugger.Setup(baud)
#define ADEBUG_ASSERT(condition)  adbg_debugger.Assert(condition, __FILE__, __LINE__)
#define ADEBUG_WRITE(message)     adbg_debugger.Write(message)
#define ADEBUG_PRINT(format, ...) adbg_debugger.Print(format, __VA_ARGS__)
#define ADEBUG_START_BLOCK()      adbg_debugger.StartBlock()
#define ADEBUG_END_BLOCK(name)    adbg_debugger.EndBlock(name)

// Allow these buffer sizes to be overridden
#ifndef ADEBUG_MAX_LOG_LENGTH
#define ADEBUG_MAX_LOG_LENGTH   150
#endif

#ifndef ADEBUG_MAX_BLOCK_DEPTH
#define ADEBUG_MAX_BLOCK_DEPTH  10
#endif

struct ADebug {
    // Buffer for log messages
    char logBuffer[ADEBUG_MAX_LOG_LENGTH];

    // Storage for block timings
    byte thisBlock;
    unsigned long blockTimings[ADEBUG_MAX_BLOCK_DEPTH];

    ADebug()
    {
        thisBlock = 0;
        logBuffer[0] = '\0';
    }

    void Setup(unsigned int baud)
    {
        Serial.begin(baud);
        while (!Serial) {
            // Wait for serial port to connect
            delay(10);
        }

        Serial.println(F("Debug Started"));
    }

    void Assert(const bool condition, const char *file, const int line)
    {
        if (condition) return;

        Serial.println(F("*** ERROR ***"));
        Print("Assert failed at %s:%d", file, line);
    }

    void Write(const __FlashStringHelper *message)
    {
        Serial.println(message);
    }

    void Print(const char *format, ...)
    {
        va_list args;
        va_start(args, format);

        if (vsnprintf(logBuffer, ADEBUG_MAX_LOG_LENGTH, format, args) > 0) {
            Serial.println(logBuffer);
        }
    }

    void StartBlock()
    {
        if (thisBlock >= ADEBUG_MAX_BLOCK_DEPTH) {
            // Too many block levels
            Serial.println(F("* WARNING *"));
            Serial.println(F("Too many debug block levels"));
            return;
        }

        blockTimings[thisBlock] = micros();
        thisBlock++;
    }

    void EndBlock(const __FlashStringHelper *name)
    {
        if (thisBlock == 0) {
            Serial.println(F("* WARNING *"));
            Serial.println(F("Debug block level sync error"));
            Serial.print(name);
            Serial.println(F(" ended with no matching beginning"));
            return;
        }

        thisBlock--;

        unsigned long time = micros() - blockTimings[thisBlock];

        Serial.print(F("Timing for block "));
        Serial.print(name);
        Serial.print(F(": "));
        if (time > 5000) {
            Serial.print(time / 1000L);
            Serial.println(F(" milliseconds"));
        }
        else {
            Serial.print(time);
            Serial.println(F(" microseconds"));
        }
    }
};

static ADebug adbg_debugger;

#endif

#endif
