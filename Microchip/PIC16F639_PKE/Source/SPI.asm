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
#define	AFECS		PORTC,1		; Chip select output
#define	SCK			PORTC,2		; SPI Clock Output
#define	SDIO		PORTC,3		; Serial output

#define FLAG_SPI_BIT0 0	
	udata
SPI_BufferH res 1
SPI_BufferL res 1

SPI_ovr	udata_ovr
Count00 res 1

flag_ovr	udata_ovr
flag	res 1		;using bit 0

	global SPI_BufferH, SPI_BufferL
	global SPI__Read, SPI__Write
	code
 
;------------------------------------------------------------------------------+
;                                                                              |
;     SPI_BufferH  SPI_BufferL SPI__Read( SPI_BufferH  SPI_BufferL )            |
;                                                                              |
;------------------------------------------------------------------------------+
;                                                                              |
;    This macro reads two Bytes from the SPI-Bus.                              |
;    Put the Read command and the address in the SPI_BufferH and SPI_BufferL   |
;    Registers.                                                                |
;    Then call SPI__Read.                                                       |
;    Then read the returned values in SPI_BufferH and SPI_BufferL.             |
;                                                                              |
;                                                                              |
;    Parameters:                                                               |
;    SPI_BufferH - The most significant Byte of the Data                       |
;    SPI_BufferL - The least significant Byte of the Data                      |
;                                                                              |
;                                                                              |
;    Returns:                                                                  |
;    SPI_BufferH - The most significant Byte of the Data                       |
;    SPI_BufferL - The least significant Byte of the Data                      |
;                                                                              |
;                                                                              |
;    Stacklevel: 1                                                             |
;                                                                              |
;                                                                              |
;    Example:                                                                  |
;    banksel SPI_BufferH                                                       |
;    movlw   0xf0                                                              |
;    movwf   SPI_BufferH                                                       |
;    movlw   0x0f                                                              |
;    movwf   SPI_BufferL                                                       |
;    call    SPI__Read                                                          |
;                                                                              |
;                                                                              |
;    Description:                                                              |
;        This sends 0xf00f over the SPI-Bus, and reads the answer to           |
;        SPI_BufferH and SPI_BufferL.                                          |
;                                                                              |
;------------------------------------------------------------------------------+
;读取SPI总线上的数据，先得发送出数据，然后才能读取  
SPI__Read
	banksel	flag
	bsf		flag,FLAG_SPI_BIT0
	goto	SPI__ShiftOutBuffer
;------------------------------------------------------------------------------+
;                                                                              |
;    SPI__Write( SPI_BufferH  SPI_BufferL )                                     |
;                                                                              |
;------------------------------------------------------------------------------+
;                                                                              |
;    This macro shifts data out of the MCU through the SPI-Interface.          |
;                                                                              |
;                                                                              |
;    Parameters:                                                               |
;    SPI_BufferH - The most significant Byte of the Data                       |
;    SPI_BufferL - The least significant Byte of the Data                      |
;                                                                              |
;                                                                              |
;                                                                              |
;    Stacklevel: 1                                                             |
;                                                                              |
;                                                                              |
;    Example:                                                                  |
;    banksel SPI_BufferH                                                       |
;    movlw   0xf0                                                              |
;    movwf   SPI_BufferH                                                       |
;    movlw   0x0f                                                              |
;    movwf   SPI_BufferL                                                       |
;    call    SPI__Write                                                         |
;                                                                              |
;                                                                              |
;    Description:                                                              |
;        This sends 0xf00f over the SPI-Bus                                    |
;                                                                              |
;------------------------------------------------------------------------------+
SPI__Write
	banksel	flag
	bcf		flag,FLAG_SPI_BIT0
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Subroutine: ShiftOutSPIBuffer
;   
; Description: This routine is used to shift data out of the microcontroller
;					onto the SPI bus.
;
;Notes:
;1.	This routine assumes 16-bit data is in SSPBufH and SSPBufL already.
;3.	Control the ports as follows:
;		Clear SCK/ALERT
;		Clear chip select
;Loop
;		Set or clear LFDATA/SDIO pin.
;		Set SCK/ALERT
;		Clear SCK/ALERT
;		Goto Loop 16 times
;		Set chip select
;Count00
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;	This method shifts data out of the MCU through the SPI-Interface.
;
;
;	@param SPI_BufferH The most significant Byte of the Data
;	@param SPI_BufferL The least significant Byte of the Data
;
;	@example
;	banksel SPI_BufferH
;	movlw	0xf0
;	movwf	SPI_BufferH
;	movlw	0x0f
;	movwf	SPI_BufferL
;	call	SPI__ShiftOutBuffer
;	@end-ex
;	@ex-desc This sends 0xf00f over the SPI-Bus
;
;	@status Tested
;
;	@stacklevel 1
;
;-----------------------------------------------------------------------
;从PIC16F639这一端向AFE通过SPI移出数据:SPI.BufferH,SPI.BufferL.高位先被移出  
;满足时序如下：  
;    1.SCLK/ALERT拉低  
;    2.CS拉低  
;    3.根据要移出的数值置位或清零SDIO  
;    4.SCLK/ALERT拉高  
;    5.SCLK/ALERT拉低  
;    6.(3-5)循环16次  
;    7.CS拉高  
;   flag.0  
;------------------------------------------------------------------------
SPI__ShiftOutBuffer	
	banksel TRISC
	movf	TRISC,W
	andlw	b'11110001'     ; Set SDIO,SCK, and CS as outputs 
	movwf	TRISC
	movlw	.16
	banksel Count00
	movwf	Count00
	banksel PORTC
	bcf		SCK
	bcf		AFECS
ShiftOutLoop
	banksel SPI_BufferH
	rlf		SPI_BufferL,F
	rlf		SPI_BufferH,F
	banksel	PORTC
	btfss	STATUS,C
	bcf		SDIO
	btfsc	STATUS,C		
	bsf		SDIO
	bsf		SCK
	nop
	nop
	bcf		SCK
;	CLRWDT
	banksel Count00
	decfsz	Count00,F
	goto	ShiftOutLoop
	banksel PORTC
	bsf		AFECS
	bsf		SCK
	banksel	flag
	btfss	flag,FLAG_SPI_BIT0
	goto	SPI__end
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; Subroutine: ShiftInSPIBuffer
;   
; Description: This routine is used to shift data into the microcontroller
;					from the SPI bus.
;
;Notes:
;1.	This routine loads 16-bit data into the SSPBufH and SSPBufL registers.
;3.	Control the ports as follows:
;		Clear SCK/ALERT
;		Clear chip select
;Loop
;		Set SCK/ALERT
;		Shift in the LFDATA/SDIO pin value.
;		Clear SCK/ALERT
;		Goto Loop 16 times
;		Set chip select
;Count00
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;	This method shifts data from the SPI-Bus into the MCU
;
;
;	@return SPI_BufferH The most significant Byte of the Data
;	@return SPI_BufferL The least significant Byte of the Data
;
;	@example
;	call	SPI__ShiftInBuffer
;	banksel SPI_BufferH
;	movf	SPI_BufferH,W
;	banksel RegH
;	movwf	RegH
;	banksel SPI_BufferH
;	movf	SPI_BufferL,W
;	banksel RegL
;	movwf	RegL
;	@end-ex
;	@ex-desc This stores the data from the SPI-Bus in RegH and RegL.
;
;	@status Tested
;
;	@stacklevel 1
;
;----------------------------------------------------------------------
;从PIC16F639的SPI线上移进数据:SPI.BufferH,SPI.BufferL.高位先被移进  
;满足时序如下：  
;    1.SCLK/ALERT拉低  
;    2.CS拉低  
;    3.根据要移出的数值置位或清零SDIO  
;    4.SCLK/ALERT拉高  
;    5.SCLK/ALERT拉低  
;    6.(3-5)循环16次  
;    7.CS拉高  
;   flag.0  
;---------------------------------------------------------------------
SPI__ShiftInBuffer
	banksel TRISC
	bsf		TRISC,3			;Set SDIO as an input
	banksel Count00
	movlw	.16
	movwf	Count00
	banksel PORTC
	bcf		SCK
	bcf		AFECS
ShiftInLoop
	banksel PORTC
	bsf		SCK
	btfss	SDIO
	bcf		STATUS, C
	btfsc	SDIO			
	bsf		STATUS, C
	bcf		SCK
	banksel SPI_BufferL
	rlf		SPI_BufferL,F
	rlf		SPI_BufferH,F
;	CLRWDT
	banksel Count00
	decfsz	Count00,F
	goto	ShiftInLoop
	banksel PORTC
	bsf		AFECS
	bsf		SCK
SPI__end
	banksel TRISC
	movf	TRISC,W
	iorlw	b'00001110'
	movwf	TRISC	
	
	return
	END
