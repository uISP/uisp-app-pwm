#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <usb.h>
#include <libusb.h>
#include <stdint.h>

#define _GNU_SOURCE
#include <getopt.h>


#define I_VENDOR_NUM        0x1d50
#define I_VENDOR_STRING     "www.ncrmnt.org"
#define I_PRODUCT_NUM       0x6032
#define I_PRODUCT_STRING    "PWM Tool"


int main(int argc, char* argv[])
{
	unsigned int val;
	int rq=0;
	char c;
	int idx=0;
	while ((c = getopt (argc, argv, "p:v:hn:")) != -1)
		switch (c)
		{
		case 'n':
			idx=atoi(optarg);
			break;
		case 'v':
			rq=1;
		case 'p':
			val = atoi(optarg);
			break;
		case 'h':
			printf("USAGE: %s -p period_in_us\n");
			printf("USAGE: %s -n channel -v high_time_in_us\n");
			exit(1);
		}
	
	usb_dev_handle *h;			
	h = nc_usb_open(I_VENDOR_NUM, I_PRODUCT_NUM, I_VENDOR_STRING, I_PRODUCT_STRING, NULL);
	if (!h) {
		fprintf(stderr, "No USB device found ;(\n");
		exit(0);
	}
	usb_set_configuration(h, 1);
	usb_claim_interface(h, 0);

	char buffer[128];
	int bytes = usb_control_msg(
                h,             // handle obtained with usb_open()
                USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, // bRequestType
                rq,      // bRequest
                val,              // wValue
                idx,              // wIndex
                buffer,             // pointer to destination buffer
                128,  // wLength
                6000
                );
	buffer[127]=0x0;
	if (bytes)
		printf(buffer);	

}
