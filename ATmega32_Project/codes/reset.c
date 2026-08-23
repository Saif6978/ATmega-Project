#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/wdt.h>

int main(void)
{
    // Disable watchdog first
    wdt_disable();

    // Enable watchdog with shortest timeout (~15 ms)
    wdt_enable(WDTO_15MS);

    while(1)
    {
        // Wait until watchdog resets the MCU
    }

    return 0;
}