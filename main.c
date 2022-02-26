/* 
 * File:   main.c
 * Author: Nazzal Naseer
 * ID: 1637253
 *
 * Created on October 31, 2021, 10:50 AM
 */

#define F_CPU 1000000

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>


/*
 * 
 */


#define _NOP()  asm("nop")

int button_pressed() {                            // checking what button's been pressed
    if (!(PIND & (1 << PIND2))) {
        _delay_ms(200);
        while (!(PIND & (1 << PIND2))) {
        }

        return 3;                               // return value corresponds to the button
    }
    else if (!(PIND & (1 << PIND3))) {
        _delay_ms(200);
        while (!(PIND & (1 << PIND3))) {
        }

        return 2;
    }
    if (!(PIND & (1 << PIND4))) {
        _delay_ms(200);
        while (!(PIND & (1 << PIND4))) {
        }

        return 1;
    }
    else {
        return 0;                                               // no button's been pressed
    }
}

void click()                                                    // Makes a buzz when the user presses a button 
{
    for (int i = 0; i < 10; i++)
    {
        
        PORTB |= (1 << PORTB2);
        _delay_ms(1);

        PORTB &= ~(1 << PORTB2);
        _delay_ms(0.5);
        
    }
}

void fail()                                                     // Failed attempt accompanied by a descending tone
{
    for (int i = 0; i < 500; i++) {
        
        PORTB |= (1 << PORTB2);
        _delay_ms(0.5);

        PORTB &= ~(1 << PORTB2);
        _delay_ms(0.5);
    }

    for (int i = 0; i < 100; i++) {
        
        PORTB |= (1 << PORTB2);
        _delay_ms(1);

        PORTB &= ~(1 << PORTB2);
        _delay_ms(1);
    }
}

void success()                                                  // Successful attempt accompanied by an ascending tone
{
    for (int i = 0; i < 100; i++) {
        
        PORTB |= (1 << PORTB2);
        _delay_ms(1);

        PORTB &= ~(1 << PORTB2);
        _delay_ms(1);
    }

    for (int i = 0; i < 500; i++) {
        
        PORTB |= (1 << PORTB2);
        _delay_ms(0.5);

        PORTB &= ~(1 << PORTB2);
        _delay_ms(0.5);
    }
}

int main(int argc, char** argv) {
    
    PORTB &= ~(1 << PORTB1);      // output for LED        
    DDRB |= (1 << DDB1);   
    
    PORTB &= ~(1 << PORTB2);      // output for Buzzer    
    DDRB |= (1 << DDB2);   
    
    DDRD &= ~(1 << DDD2);         // Input for button 3
    PORTD |= (1 << PORTD2); 

    DDRD &= ~(1 << DDD3);         // Input for button 2               
    PORTD |= (1 << PORTD3);
    
    DDRD &= ~(1 << DDD4);         // Input for button 1             
    PORTD |= (1 << PORTD4); 
     
    int current_state = 0;       // To keep track of the index of the passcord array that the user has reached 
    int pass[] = {1, 2, 3, 1};   // The passcode 
    
    int code_size = (sizeof(pass)/sizeof(int));     // to avoid hard-coding

    while (1) {

         int x = button_pressed();          // fetches what button's been pressed, if any 
         
         if ( x!= 0)
         {
             click();                       // buzz when a button's been pressed
         }
         
         if (x == pass[current_state] || (x && current_state == code_size))         // to see if the passcode unlock attempt is successful
         { 
             if (current_state == code_size) {                                      // toggles the lock (back off) by pressing any key after a successful attempt
                 PORTB ^= (1 << PORTB1);
                 current_state = 0; 
             }
             else if (current_state == (code_size - 1)) {                           // toggles the lock (on) after a successfull attempt
                 success();                                                         // success buzz
                 PORTB ^= (1 << PORTB1);
                 current_state++;  
             }
             else if (current_state < code_size) {                                  // graduates user to next index
                 current_state++;  
             }
         }
         else if (x != 0 && x != pass[current_state])                               // if user presses wrong button, user has to start over
         {
             current_state = 0; 
             if (current_state == code_size) {
                fail();                                                             // fail buzz
             }
         }
    }

    return (EXIT_SUCCESS);
}

