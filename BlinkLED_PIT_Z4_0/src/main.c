/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);
void vGpioConfigure();
void peri_clock_gating (void) ;
void clock_config(void);
void SysClk_Init(void);
void InitPeriClkGen(void);
void vPitConfigure(uint32_t);
void vPit0_ISR(void);


void hw_init(void)
{
#if defined(DEBUG_SECONDARY_CORES)
	uint32_t mctl = MC_ME.MCTL.R;
#if defined(TURN_ON_CPU1)
	/* enable core 1 in all modes */
	MC_ME.CCTL[2].R = 0x00FE;
	/* Set Start address for core 1: Will reset and start */
#if defined(START_FROM_FLASH)
	MC_ME.CADDR[2].R = 0x11d0000 | 0x1;
#else
	MC_ME.CADDR[2].R = 0x40040000 | 0x1;
#endif /* defined(START_FROM_FLASH) */

#endif
#if defined(TURN_ON_CPU2)
	/* enable core 2 in all modes */
	MC_ME.CCTL[3].R = 0x00FE;
	/* Set Start address for core 2: Will reset and start */
#if defined(START_FROM_FLASH)
	MC_ME.CADDR[3].R = 0x13a0000 | 0x1;
#else
	MC_ME.CADDR[3].R = 0x40080000 | 0x1;
#endif /* defined(START_FROM_FLASH) */

#endif
	MC_ME.MCTL.R = (mctl & 0xffff0000ul) | KEY_VALUE1;
	MC_ME.MCTL.R =  mctl; /* key value 2 always from MCTL */
#endif /* defined(DEBUG_SECONDARY_CORES) */
}

int main(void)
{
	volatile unsigned long counter = 0;

	xcptn_xmpl ();              /* Configure and Enable Interrupts */
	peri_clock_gating();
	InitPeriClkGen();
	SysClk_Init();
	vGpioConfigure();
	vPitConfigure(40000000);
	/* Loop forever */
	for(;;) {	   
		counter++;
		if(counter < 1000000)
		{
			SIUL2.GPDO[4].B.PDO_4n = 0;  //ON
			SIUL2.GPDO[0].B.PDO_4n = 1;  //OFF
			SIUL2.GPDO[148].B.PDO_4n = 1;  //FF

		}
		else if (counter < (2000000))
		{
			SIUL2.GPDO[4].B.PDO_4n = 1; //OFF
			SIUL2.GPDO[0].B.PDO_4n = 0;  //ON
			SIUL2.GPDO[148].B.PDO_4n = 1;  //ON
		}
		else if (counter < (3000000))
		{
			SIUL2.GPDO[4].B.PDO_4n = 1; //OFF
			SIUL2.GPDO[0].B.PDO_4n = 1;  //ON
			SIUL2.GPDO[148].B.PDO_4n = 0;  //ON
		}
		else if(counter > 4000000)
		{
			counter = 0;
		}
	}
}


void vGpioConfigure()
{
	SIUL2.MSCR[4].B.OBE =1; //enable output
	SIUL2.MSCR[4].B.IBE = 0 ;
	SIUL2.MSCR[0].B.OBE =1; //enable output
	SIUL2.MSCR[0].B.IBE = 0 ;
	SIUL2.MSCR[148].B.OBE =1; //enable output
	SIUL2.MSCR[148].B.IBE = 0 ;
	SIUL2.MSCR[117].B.OBE =1; //enable output
	SIUL2.MSCR[117].B.IBE = 0 ;
}
void vPitConfigure(uint32_t Fu32Ldval)
{
	PIT.MCR.B.MDIS = 0;
	PIT.MCR.B.FRZ = 1;
	PIT.TIMER[0].TCTRL.B.TIE = 1;
	PIT.TIMER[0].LDVAL.R = Fu32Ldval ;
	//INTC configuration for PIT0
	INTC.PSR[226].B.PRC_SELN0 = 1;
	INTC.PSR[226].B.PRC_SELN1 = 0;
	INTC.PSR[226].B.PRC_SELN2 = 0;

	INTC.PSR[226].B.PRIN = 5 ;
	PIT.TIMER[0].TCTRL.B.TEN = 1; //Timer enable

}

void clock_config(void)
{
	MC_CGM.AC5_SC.B.SELCTL = 1 ; //select XOSC as ClkIN for FMPLL
	//configure FMPLL to have 160Mhz freq

}

//Enable XOSC, PLL0, PLL1, and enter LPU_STANDBY with LPU_SYS_CLK as system clock (160 MHz).
//Enable XOSC, PLL0, PLL1, and enter LPU_STANDBY with LPU_SYS_CLK as system clock (160 MHz).
void SysClk_Init(void)
{
	/* Connect XOSC to PLL */
	MC_CGM.AC5_SC.B.SELCTL=1;

	/* Configure PLL0 Dividers - 160MHz from 40Mhx XOSC */
	/* PLL input = FXOSC = 40MHz
	     VCO range = 600-1200MHz
	 */
	PLLDIG.PLLDV.B.PREDIV  = 1; 	/* Divide input clock by 1, resulting clock will be reference input for the PLL analog loop */
	PLLDIG.PLLDV.B.MFD     = 16; 	/* Loop multiplication factor 16*40 MHz */
	PLLDIG.PLLDV.B.RFDPHI  = 1; 	/* VCO post divider = 4 so 16*40/40 = 160MHz */

	PLLDIG.PLLCAL3.R = 0x09C3C000; /* Contains the denominator of the fractional loop division factor */
	/* MFDEN = 9C3C000h = 163823616 */
	PLLDIG.PLLFD.B.SMDEN = 1;		 /* Sigma delta modulation enabled */
	/* switch to PLL */
	MC_ME.DRUN_MC.R = 0x00130072;	/* MVRON = 1, FLAON = RUN mode, PLLON=1, FXOSCON = 1, FIRCON = 1, SYSCLK = PLL PHI_0 */
	MC_ME.MCTL.R = 0x30005AF0; 	/* Target mode = DRUN, KEY = 5AF0 */
	MC_ME.MCTL.R = 0x3000A50F; 	/* Target mode = DRUN, KEY = A50F */

	while(MC_ME.GS.B.S_MTRANS == 1);      /* Wait for mode transition complete */
}



void InitPeriClkGen(void)
{
	/* F160 - max 160 MHz */
	MC_CGM.SC_DC0.B.DIV = 0;  /* Freq = sysclk / (0+1) = sysclk */
	MC_CGM.SC_DC0.B.DE  = 1;  /* Enable divided clock */

	/* S80 - max 80 MHz */
	/* MC_CGM_SC_DC1[DIV] and MC_CGM_SC_DC5[DIV] must be equal at all times */
	MC_CGM.SC_DC1.B.DIV = 1;  /* Freq = sysclk / (2+1) = sysclk/2 */
	MC_CGM.SC_DC1.B.DE  = 1;  /* Enable divided clock */

	/* FS80 - max 80 MHz */
	/* MC_CGM_SC_DC1[DIV] and MC_CGM_SC_DC5[DIV] must be equal at all times */
	MC_CGM.SC_DC5.R = MC_CGM.SC_DC1.R;  /* 80 MHz max */

	/* S40 - max 40 MHz */
	MC_CGM.SC_DC2.B.DIV = 3;  /* Freq = sysclk / (3+1) = sysclk/4 */
	MC_CGM.SC_DC2.B.DE  = 1;  /* Enable divided clock */

	/* F40 - max 40 MHz (for PIT, etc.) - use default values */

	/* F80 - max 80 MHz - use default values*/

	/* F20 - clock out configured at clock_out* function */



}

void peri_clock_gating (void)
{
  MC_ME.RUN_PC[0].R = 0x00000000;  /* Gate off clock for all RUN modes */
  MC_ME.RUN_PC[1].R = 0x000000FE;  /* Configures peripheral clock for all RUN modes */
  MC_ME.PCTL[91].B.RUN_CFG = 0x1;  /* PIT: select peripheral configuration RUN_PC[1] */
}

void vPit0_ISR()
{

	SIUL2.GPDO[117].B.PDO_4n = ~(SIUL2.GPDO[117].B.PDO_4n);  //Toogle Led
	PIT.TIMER[0].TFLG.B.TIF = 1 ;

}


