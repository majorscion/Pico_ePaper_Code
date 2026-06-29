// #include "EPD_Test.h"   //Examples
#include "dashboard.h"

int main(void)
{
	// while(1) {
		
	    // DEV_Delay_ms(10000); 
	// }
	DEV_Delay_ms(500); 
	// EPD_2in9_V2_test();
    // EPD_2in9bc_test();
    // EPD_2in9b_V3_test();
    // EPD_2in9b_V4_test();
    // EPD_2in9d_test();

    // EPD_2in13_V2_test();
	// EPD_2in13_V3_test();
 
    // ---->   EPD_2in13_V4_test();   // This is my display.

    // EPD_2in13bc_test();
    // EPD_2in13b_V3_test();
    // EPD_2in13b_V4_test();
    // EPD_2in13d_test();
    
    // EPD_2in66_test();
    // EPD_2in66b_test();
    
    // EPD_2in7_test();
    // EPD_2in7_V2_test();

    // EPD_3in7_test();
	
	// EPD_4in2_test();
    // EPD_4in2_V2_test();
    // EPD_4in2b_V2_test();
    // EPD_4in2b_V2_test_old();
    // EPD_5in65f_test();

    // EPD_5in83_V2_test();
    // EPD_5in83b_V2_test();

    // EPD_7in5_V2_test();
    // EPD_7in5_V2_test_old();
    // EPD_7in5b_V2_test();
    // EPD_7in5b_V2_test_old();

    Dashboard_Initialize();
    for(int i=0; i<10; i++)
    {
        DEV_Delay_ms(1000);
        Dashboard_Display();
    }
    Dashboard_Shutdown();

    return 0;
}
