#include <16F877A.h>
#use delay(crystal=4MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

#define LCD_ENABLE_PIN PIN_D2
#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7

#include <lcd.c>

char received_data[10];  // Buffer for received data

// Function prototypes
void process_received_data(char* data);

void main() {
    char ch;
    int index = 0;

    lcd_init();
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "Waiting...");

    while (TRUE) {
        if (kbhit()) {  // Check if data is received
            ch = getc();  // Read a character
            if (ch == '#') {  // End of a data frame
                received_data[index] = '\0';  // Null-terminate the string
                process_received_data(received_data);  // Process the data
                index = 0;  // Reset index for next data
            } else {
                received_data[index++] = ch;  // Store received character
            }
        }
    }
}

// Process the received data and display it on the LCD
void process_received_data(char* data) {
    lcd_gotoxy(1, 1);

    if (data[0] == 'T') {
        // Display temperature
        lcd_putc('\f');  // Clear LCD
        printf(lcd_putc, "Temp: %s C", &data[1]);
    } else if (data[0] == 'D') {
        // Display distance
        lcd_putc('\f');  // Clear LCD
        printf(lcd_putc, "Dist: %s cm", &data[1]);
    }
    delay_ms(2000);  // Delay before switching to next display
}
