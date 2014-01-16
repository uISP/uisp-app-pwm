#include <arch/antares.h>
#include <avr/boot.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <generated/usbconfig.h>
#include <arch/vusb/usbportability.h>
#include <arch/vusb/usbdrv.h>
#include <arch/delay.h>

const int prescaler_lookup[] = { 0, 1, 8, 64, 256, 1024 };

struct avr_pwm16 {
	uint16_t icr; /* icr */
	uint8_t cs; /* prescaler value */ 
};

struct avr_pwm16 avr_timer_conf(uint64_t period_us)
{
	int i; 
	uint64_t tmp;
	struct avr_pwm16 pwm;
	for (i=1; i<ARRAY_SIZE(prescaler_lookup); i++)
	{
		tmp = (F_CPU / prescaler_lookup[i]) * period_us / 1000000 ;
		if (tmp < 65536) 
			break;
	}	
	pwm.icr = (uint16_t) tmp;
	pwm.cs = i;
	return pwm;
} 

void pwm_init(struct avr_pwm16* conf)
{
	TCCR1A = 0;
	TCCR1B = 0;
	ICR1 = conf->icr;
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B = (1 << WGM13) | (1<<WGM12) | conf->cs;
	TCCR1A |= 2<<6;
	DDRB |= 1<<1 | 1<<2;
}

char msg[128];
struct avr_pwm16 conf;
uchar   usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;
	if (rq->bRequest == 0) {
		conf = avr_timer_conf(rq->wValue.word);
		pwm_init(&conf);
		sprintf(msg, "ICR: %u  | Prescaler: %u\n", conf.icr, conf.cs);
	} else if (rq->bRequest == 1) {
		uint64_t hticks = ((uint64_t) (F_CPU / prescaler_lookup[conf.cs])) * rq->wValue.word / 1000000 ;
		if (rq->wIndex.word == 0) 
			OCR1A = (uint16_t) hticks;
		else
			OCR1B = (uint16_t) hticks;
		sprintf(msg, "Channel %d ticks %u\n", rq->wIndex.word, (uint16_t) hticks);
	}
	usbMsgPtr = msg;
	return (strlen(msg)+1);

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


ANTARES_INIT_HIGH(uinit)
{
  	usbInit();
}




/*
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
	usbPoll();
}
