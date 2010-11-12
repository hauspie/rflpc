/* Some constants */
	.equ STACK_SIZE, 256

	
/********************************************/
/* stack section */
	.section ".stack", "w"
_stack_top:	
	.space STACK_SIZE
_stack_bottom:
/********************************************/	


	
/********************************************/	
/* Interrupt vector section, see section 34.3.3 p 745 of the user manual for internal interrupts
	and table 50 p73 */
	.section ".interrupt_vector"
	.long	_stack_bottom 		/* initial value of SP */
	.long	_reset 			/* address of the reset routine */
	.long	_default_handler	/* Non Masquable Interrupt handler */
	.long	_default_handler	/* Hardware Fault handler */
	.long	_default_handler	/* Memory protection unit exception handler */
	.long	_default_handler	/* Bus fault handler */
	.long	_default_handler	/* Usage fault handler */
	.long	0			/* Reserved */
	.long	0			/* Reserved */
	.long	0			/* Reserved */
	.long	0			/* Reserved */
	.long	_default_handler	/* Supervisor mode call handler */
	.long	0			/* Reserved for debug */
	.long	_default_handler	/* pend sv handler (c.f. p746 of user manual) */
	.long	_default_handler	/* system timer tick handler (generated when the timer reaches 0) */

	/* From now, all interrupts are external IRQ for peripherals */
	.long	_default_handler	/* Watchdog timer */
	.long	_default_handler	/* Timer0 */
	.long	_default_handler	/* Timer1 */
	.long	_default_handler	/* Timer2 */
	.long	_default_handler	/* Timer3 */
	.long	_default_handler	/* UART0 */
	.long	_default_handler	/* UART1 */
	.long	_default_handler	/* UART2 */
	.long	_default_handler	/* UART3 */
	.long	_default_handler	/* PWM1 */
	.long	_default_handler	/* I2C0 */
	.long	_default_handler	/* I2C1 */
	.long	_default_handler	/* I2C2 */
	.long	_default_handler	/* SPI */
	.long	_default_handler	/* SSP0 */
	.long	_default_handler	/* SSP1 */
	.long	_default_handler	/* PLL0 Lock */
	.long	_default_handler	/* Real Time Clock */
	.long	_default_handler	/* External Interrupt 0 */
	.long	_default_handler	/* External Interrupt 1 */
	.long	_default_handler	/* External Interrupt 2 */
	.long	_default_handler	/* External Interrupt 3 */
	.long	_default_handler	/* Analogic Digital Converter */
	.long	_default_handler	/* Brown-Out detect */
	.long	_default_handler	/* USB */
	.long	_default_handler	/* CAN */
	.long	_default_handler	/* General Purpose DMA */
	.long	_default_handler	/* I2S */
	.long 	_default_handler	/* Ethernet */
	.long	_default_handler	/* Repetitive Interrupt Timer */
	.long	_default_handler	/* Motor Control PWM */
	.long	_default_handler	/* Quadrature Encoder */
	.long	_default_handler	/* PLL1 Lock */

	/* Assemble section in thumb mode */
	.thumb
	
	

/********************************************/
/* code section */
	.section ".text"
	/* make start an external symbol */
	.globl	_start
_reset:
	b	_start
_start:
	b 	main /* branch inside C main program */
	b	.
_default_handler:
	b	.	/* infinite loop */
