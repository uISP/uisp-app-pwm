#include <arch/antares.h>
#include <avr/boot.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <generated/usbconfig.h>
#include <arch/vusb/usbportability.h>
#include <arch/vusb/usbdrv.h>
#include <arch/delay.h>

uchar   usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	return 0;
}

uchar usbFunctionWrite(uchar *data, uchar len)
{

}

inline void usbReconnect()
{
	DDRD=0xff;
	_delay_ms(250);
	DDRD=0;
}

ANTARES_INIT_LOW(io_init)
{
	DDRC=1<<2;
	PORTC=0xff;
 	usbReconnect();
	
}

/*
ANTARES_INIT_HIGH(uinit)
{
  	usbInit();
}
*/

const int prescaler_lookup[] = { 0, 1, 8, 64, 256, 1024 };


/*
int pwm_setup(int period_ns)
{
	int i; 
	uint32_t tmp;
	uint16_t icr;
	for (i=ARRAY_SIZE(prescaler_lookup); i>=0; i--)
	{
		tmp = F_CPU / ms / prescaler_lookup[i];
		if (tmp < 65535)
			break;
	}
	icr = (uint16_t) tmp;
} 


#define PRESCALER 8
#define ICR_VALUE  (F_CPU / 1000 * 20 / PRESCALER)
#if ICR_VALUE > 65535
#error ICR TOO BIG
#endif

#define NS_TO_ICR(n) (ICR_VALUE * n / 20000)

ANTARES_INIT_HIGH(pwm_init)
{
	TCCR1A = 0;
	ICR1 = ICR_VALUE;
	TCCR1A = (1<<WGM11);
	TCCR1B = (1 << WGM13) | (1<<WGM12) | (1 << CS11);
	TCCR1A |= 2<<6;

	DDRB |= 1<<1;
	//OCR1A = NS_TO_ICR(900);
	OCR1A = NS_TO_ICR(1000);
}

int j = 1000;
ANTARES_APP(usb_app)
{
	usbPoll();
	OCR1A = NS_TO_ICR(j++);
	delay_ms(50);
}
*/


ANTARES_APP(usb_app)
{
	DDRB=0xff;
	usbPoll();
	PORTB^=0xff;
	delay_ms(500);
}
