#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include "Debug.h"
// #include <stdlib.h> // malloc() free()

// Enumerations
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

// Function declarations
int Dashboard_Initialize(void);
int Dashboard_Shutdown(void);

int Dashboard_SetMode(enum DashboardMode mode);

int Dashboard_SetNextEvent(char* nextEvent);

int Dashboard_SetPumpStatus(enum PumpStatus status);

int Dashboard_UpdateCurrentTime(void);

int Dashboard_Display(void);

// Global dashboard instance - defined in dashboard.c, declared extern here
extern struct Dashboard dashboard;

#endif // DASHBOARD_H
