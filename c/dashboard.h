#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include "Debug.h"
#include <stdlib.h> // malloc() free()



enum DashboardMode {
    MODE_NORMAL,
    MODE_BYPASS_ON,
    MODE_BYPASS_OFF,
    MODE_SKIP
};

enum PumpStatus {
    PUMP_OFF,               // Pump is off
    PUMP_NORMAL,            // Pump is running normally
    PUMP_CHECK_WATER        // High current, high RPM, check water level or clean filter
};

int Dashboard_Initialize(void);
int Dashboard_Shutdown(void);

int Dashboard_SetMode(enum DashboardMode mode);

int Dashboard_SetNextEvent(char* nextEvent);

int Dashboard_SetPumpStatus(enum PumpStatus status);

int Dashboard_UpdateCurrentTime(void);

int Dashboard_Display(void);

// Everything operates on this global dashboard instance
struct Dashboard {
    enum DashboardMode mode;
    char* nextEvent;
    enum PumpStatus pumpStatus;
    PAINT_TIME currentTime;
    bool isInitialized;
    UBYTE* frameBuffer;
} dashboard;


#endif // DASHBOARD_H
