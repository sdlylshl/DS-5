;------------------------------------------------------------------------------+
;                                                                              |
;    Module SPI                                                                |
;                                                                              |
;------------------------------------------------------------------------------+
;                                                                              |
;    SPI.asm                                                                   |
;    Jan Ornter                                                                |
;    DATE:   11-9-2005                                                         |
;    VER.:   1.0                                                               |
;    This class provides functions for the modified physical SPI-Layer         |
;                                                                              |
;                                                                              |
;------------------------------------------------------------------------------+
#include "Project.inc"
;	when overriding these values, you will have to change the source code

#define IRQ_PORT    PORTA
#define IRQ_PIN     2       ;IRQ RA2
#define	CE_PIN	    4		;radio enable

#define SPI_PORT		PORTC	
#define SPI_CS_PIN		1		; Chip select output
#define	SPI_SCLK_PIN	2		; SPI Clock Output
#define	SPI_MOSI_PIN	3		; Serial output
#define	SPI_MISO_PIN	4		; Serial input


	udata
SPI_Buffer res 1

SPI_ovr	udata_ovr
Count00 res 1

	global SPI_Buffer
	global SPI__ReadWrite
	code

;------------------------------------------------------------------------------+
;                                                                              |
;    SPI__ReadWrite( SPI_Buffer )                                                  |
;                                                                              |
;------------------------------------------------------------------------------+
;                                                                              |
;    This macro shifts data out of the MCU through the SPI-Interface.          |
;                                                                              |
;                                                                              |
;    Parameters:                                                               |
;    SPI_Buffer - The significant Byte of the Data                             |
;                                                                              |
;                                                                              |
;                                                                              |
;    Stacklevel: 1                                                             |
;                                                                              |
;                                                                              |
;    Example:                                                                  |
;    movlw   0x0f                                                              |
;    movwf   SPI_Buffer                                                        |
;    call    SPI__ReadWrite                                                    |
;                                                                              |
;                                                                              |
;    Description:                                                              |
;        This sends 0x0f over the SPI-Bus                                      |
;                                                                              |
;------------------------------------------------------------------------------+  

SPI__ReadWrite
	movlw	.8
	banksel Count00
	movwf	Count00
    
	banksel TRISC
	movf	TRISC,W
	andlw	b'11110001'     ; Set SPI_MOSI_PIN,SPI_SCLK_PIN, and CS as outputs 
    iorlw	b'00010000'     ; Set SPI_MISO_PIN as input
	movwf	TRISC

	banksel PORTC
	bcf		PORTC,SPI_SCLK_PIN
	bcf		PORTC,SPI_CS_PIN
    bsf		STATUS, C
    ;通过移位实现数据交换
ShiftLoop
	banksel SPI_Buffer
	rlf		SPI_Buffer,F
	banksel	STATUS
	btfss	STATUS,C
	bcf		PORTC,SPI_MOSI_PIN
	btfsc	STATUS,C		
	bsf		PORTC,SPI_MOSI_PIN
    ;SCLK上升沿读取写入数据
	bsf		PORTC,SPI_SCLK_PIN
    ;读取SPI总线上的数据
	btfss	PORTC,SPI_MISO_PIN
	bcf		STATUS, C
	btfsc	PORTC,SPI_MISO_PIN			
	bsf		STATUS, C
	bcf		PORTC,SPI_SCLK_PIN
  
;	CLRWDT
	banksel Count00
	decfsz	Count00,F
	goto	ShiftLoop
    
	banksel SPI_Buffer
	rlf		SPI_Buffer,F   
    
	banksel PORTC
	bsf		PORTC,SPI_CS_PIN
	;bsf		PORTC,SPI_SCLK_PIN
	return
	END
