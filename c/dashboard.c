#include "dashboard.h"
#include "EPD_2in13_V4.h"
#include <stdlib.h>
#include <string.h>

// Global dashboard instance - only one definition in the entire program
struct Dashboard {
    enum DashboardMode mode;
    char* nextEvent;
    enum PumpStatus pumpStatus;
    PAINT_TIME currentTime;
    bool isInitialized;
    UBYTE* frameBuffer;
} dashboard;

// Initialize the dashboard
int Dashboard_Initialize(void) {
    if (DEV_Module_Init() != 0) {
        Debug("Failed to initialize DEV module\n");
        return -1;
    }

    EPD_2in13_V4_Init();
    EPD_2in13_V4_Clear();
    Debug("EPD initialized and cleared\n");

    // Initialize dashboard structure
    dashboard.mode = MODE_NORMAL;
    dashboard.nextEvent = "Hello World!";
    dashboard.pumpStatus = PUMP_OFF;
    memset(&dashboard.currentTime, 0, sizeof(PAINT_TIME));
    UWORD Imagesize = ((EPD_2in13_V4_WIDTH % 8 == 0)? (EPD_2in13_V4_WIDTH / 8 ): (EPD_2in13_V4_WIDTH / 8 + 1)) * EPD_2in13_V4_HEIGHT;
    dashboard.frameBuffer = (UBYTE*)malloc(Imagesize);
    if (dashboard.frameBuffer == NULL) {
        Debug("Failed to allocate frame buffer\n");
        return -1;
    }

    Debug("Frame buffer allocated\n");

    Paint_NewImage(dashboard.frameBuffer, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 90, WHITE); // Initialize paint with white background
    Paint_Clear(WHITE); // Clear the paint to white

    Debug("Dashboard initialized\n");
    dashboard.isInitialized = true;
    return 0;
}

// Shutdown the dashboard
int Dashboard_Shutdown(void) {
    if (dashboard.isInitialized) {
        Debug("Shutting down dashboard\n");
        EPD_2in13_V4_Init();
        EPD_2in13_V4_Clear();

        Debug("EPD cleared before sleep\n");
        EPD_2in13_V4_Sleep();

        // Important: wait at least 2s

        DEV_Delay_ms(2000);
        DEV_Module_Exit();
        dashboard.isInitialized = false;
        Debug("Dashboard shutdown complete\n");

        // Free the frame buffer
        if (dashboard.frameBuffer != NULL) {
            free(dashboard.frameBuffer);
            dashboard.frameBuffer = NULL;
        }
        Debug("Frame buffer freed\n");
    }
    return 0;
}

// Set the dashboard mode
int Dashboard_SetMode(enum DashboardMode mode) {
    // TODO: Implement mode setting logic
    return 0;
}

// Set the next event text
int Dashboard_SetNextEvent(char* nextEvent) {
    // TODO: Implement next event setting logic
    return 0;
}

// Set the pump status
int Dashboard_SetPumpStatus(enum PumpStatus status) {
    // TODO: Implement pump status setting logic
    return 0;
}

// Update the current time display
int Dashboard_UpdateCurrentTime(void) {
    // TODO: Implement time update logic - fetch time from DS3231 RTC module
    return 0;
}

// Display the dashboard
int Dashboard_Display(void) {
    if (!dashboard.isInitialized) {
        Debug("Dashboard not initialized\n");
        return -1;
    }
    Paint_Clear(WHITE); // Clear the paint to white

    int line = 0;
    Paint_DrawString_EN(0, line * 16, dashboard.nextEvent, &Font16, BLACK, WHITE);

    EPD_2in13_V4_Display(dashboard.frameBuffer); // Display the frame buffer on the e-paper display
    return 0;
}

