#define  CONFIG_AMLROM_SPL 1
#include <timer.c>
#include <timming.c>
#include <uartpin.c>
#include <serial.c>
#include <pinmux.c>
#include <sdpinmux.c>
#include <memtest.c>
#include <pll.c>
#include <ddr.c>
#include <mtddevices.c>
#include <sdio.c>
#include <debug_rom.c>

#include <loaduboot.c>


unsigned main(unsigned __TEXT_BASE,unsigned __TEXT_SIZE)
{

#ifdef WA_AML8726_M3_REF_V10
	//PWREN GPIOAO_2, PWRHLD GPIOAO_6 pull up
	//@WA-AML8726-M3_REF_V1.0.pdf
	//@AppNote-M3-CorePinMux.xlsx
	clrbits_le32(P_AO_GPIO_O_EN_N, ((1<<2)|(1<<6)));
	setbits_le32(P_AO_GPIO_O_EN_N,((1<<18)|(1<<22)));
#endif

	int i;
		
    //Adjust 1us timer base
    timer_init();
	  //default uart clock.
    serial_init(__plls.uart);
    serial_put_dword(get_utimer(0));
    writel(0,P_WATCHDOG_TC);//disable Watchdog
    // initial pll
    pll_init(&__plls);
    
    __udelay(100000);//wait for a uart input 
	 if(serial_tstc()){
	    debug_rom(__FILE__,__LINE__);
	 }
    // initial ddr
    ddr_init_test();
    // load uboot
    load_uboot(__TEXT_BASE,__TEXT_SIZE);
    serial_puts("Systemp Started\n");
    //wait serial_puts end.
    for(i = 0; i < 10; i++)
		  __udelay(1000);
    return 0;
}