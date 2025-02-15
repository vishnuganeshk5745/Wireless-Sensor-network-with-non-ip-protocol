#include <16F877A.h>
#device ADC=10
#use delay(crystal=4MHz)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)  // UART for RF communication

#define TRIG_PIN PIN_C0
#define ECHO_PIN PIN_C1

// Function prototypes
float get_temperature();
unsigned int get_ultrasonic_distance();

void main() {
    float temperature;
    unsigned int distance;

    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(AN0);
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);

    while (TRUE) {
        // Step 1: Get temperature from LM35
        temperature = get_temperature();
        // Step 2: Send temperature data over RF module
        printf("T%.1f#", temperature);  // 'T' indicates temperature, '#' marks end of data

        delay_ms(1000);  // Short delay between transmissions

        // Step 3: Get distance from ultrasonic sensor
        distance = get_ultrasonic_distance();
        // Step 4: Send distance data over RF module
        printf("D%u#", distance);  // 'D' indicates distance, '#' marks end of data

        delay_ms(1000);  // Delay between readings
    }
}

// Function to get temperature from LM35 sensor
float get_temperature() {
    long int adc_value;
    float temperature;

    set_adc_channel(0);
    delay_us(20);
    adc_value = read_adc();

    temperature = (adc_value * 5.0 * 100.0) / 1024.0;
    return temperature;
}

// Function to get distance from the ultrasonic sensor
unsigned int get_ultrasonic_distance() {
    unsigned int distance = 0;
    unsigned long duration;

    output_low(TRIG_PIN);
    delay_us(2);
    output_high(TRIG_PIN);
    delay_us(10);
    output_low(TRIG_PIN);

    while (!input(ECHO_PIN));
    set_timer1(0);
    while (input(ECHO_PIN));
    duration = get_timer1();

    distance = duration / 58;
    return distance;
}

