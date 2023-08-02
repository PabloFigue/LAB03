//*****************************************************************************
/*
 * File:   main.c
 * Author: Pablo
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 10 de febrero de 2020, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include <pic16f887.h>
#include "LCD_ALL.h"

//Definicion de los puertos del LCD
#define RS PORTEbits.RE0
#define EN PORTEbits.RE1
#define D0 PORTBbits.RB0
#define D1 PORTBbits.RB1
#define D2 PORTBbits.RB2
#define D3 PORTBbits.RB3
#define D4 PORTBbits.RB4
#define D5 PORTBbits.RB5
#define D6 PORTBbits.RB6 
#define D7 PORTBbits.RB7
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

uint8_t ADC,CONT,TEMP;
uint8_t modo = 0;

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    LcdALL_Init();    //Inicializar la LCD
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       LcdALL_Set_Cursor(1,1);     //Setear el cursor en la primera linea
       LcdALL_Write_String("S1:    S2:    S3:"); //Escribir los títulos en la primera linea
       
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       if (modo == 0){
            spiWrite(0);
            PORTA = spiRead();
            ADC = PORTA;
            modo = 1;
       }else if (modo == 1){
            spiWrite(0);
            PORTA = spiRead();
            CONT = PORTA;
            modo = 0;
       }
        __delay_ms(1);
        PORTCbits.RC2 = 1;       //Slave Deselect 
       
       
       //////////////////////////////////////////////segundo SLEEVE
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0);
       TEMP = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       ////////////////////////////////////////////////
       //PORTB++;
       
       ///LCD
       LcdALL_Set_Cursor(2,1); //Setear el cursor en la segunda linea.
       LcdALL_set_value(ADC);  //Mostrar en la LCD el valor del potenciometro
       LcdALL_Write_String("   "); //Escribir en pantalla un espacio
       LcdALL_set_value(CONT);    //Mostrar en pantalla el valor del contador
       LcdALL_Write_String("    "); //Escribir en pantalla un espacio
       LcdALL_set_value(TEMP); //Escribir en pantalla un espacio
       __delay_ms(2);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    //OSCILADOR
    OSCCONbits.IRCF = 0b111;    // 8MHz
    OSCCONbits.SCS = 1;         // Utilización del oscilador Interno
    
    ANSEL = 0;
    TRISA = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISC1 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    TRISE = 0;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}