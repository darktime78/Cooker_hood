#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
 
#define LED1 PD4
#define LED2 PD5
#define LED3 PD6
#define LED4 PD7
 
#define PERIOD_SCAN_BUTTON 25 //Period scan button press 50 ms for 8MHz ~ 2ms
#define PERIOD_SCAN_TEMP  250 //Period scan temperature 500 ms 8MHz ~ 2ms
 
 
//volatile long  millis;
volatile char scan_button_task;
volatile char scan_temp_task;

void InitSystem(void);
void InitDigitalSensor(void);
void InitTimer(void);


 
ISR(TIMER2_OVF_vect)
{
    //millis++;
    scan_button_task++;
    scan_temp_task++;
    // checking the buttons press
    if (scan_button_task==PERIOD_SCAN_BUTTON) {
                                                scan_button_task=0;
                                                scanbutton();
                                              }
    // checking the values temperature sensors
    if (scan_temp_task==PERIOD_SCAN_TEMP) {
                                            scan_button_task=0;
                                            scantemp();
                                          }
}
 
int main(void)
{
    
 
    for (;;) {
        // checking the button status flags
        mode_selection();
        // checking the auto mode flag
        if(cm.auto) {auto_mode();}
        // sleep mode     
        sleep_cpu();
             }
 
    return 0; // What for??? many bytes in code =)
}

void InitSystem (void) {

    // GPIO Setup
    //DDRB |= (1<<)|(1<<)|(1<<);
    //DDRB |= (1<<)|(1<<)|(1<<);
    DDRD |= (1<<LED1)|(1<<LED2)|(1<<LED3)|(1<<LED4);

    // set sleep mode
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_enable();
}
void InitTimer(void) {
    cli();
    // Timer2 Setup
    TIMSK |=(1<<TOIE2);  // interrupt enable
    TCCR2 = (1<<CS01) | (1<<CS00); // prescaler 64; IRQ freq ~ 976us for 16MHz ~ 1ms, 488us - 8Mhz ~ 2ms
    // preset variables
    //millis=0;
    scan_button_task=0;
    scan_temp_task=0;     
    // Let's go...
    sei();
}