#pragma once
#define DEBUG_PRINT_LN(...)    Serial.println(__VA_ARGS__)
#define DEBUG_PRINT_T(...)    Serial.print(__VA_ARGS__); Serial.print("\t");


#define DEBUG_SERIAL_CMD_LN(...)     DEBUG_PRINT_LN(__VA_ARGS__)
#define DEBUG_SERIAL_CMD_T(...)      DEBUG_PRINT_T(__VA_ARGS__)

