/* Common main.c for the benchmarks

   Copyright (C) 2014 Embecosm Limited and University of Bristol
   Copyright (C) 2018-2019 Embecosm Limited

   Contributor: James Pallister <james.pallister@bristol.ac.uk>
   Contributor: Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of Embench and was formerly part of the Bristol/Embecosm
   Embedded Benchmark Suite.

   SPDX-License-Identifier: GPL-3.0-or-later */

#include "support.h"

#define DWT_CTRL *((volatile unsigned *)(0xE0001000))
#define DWT_CYCCNT *((volatile unsigned *)(0xE0001000 + 0x4))

#define NUMCOMP_BIT 28
#define NOCYCCNT_BIT 25
#define CYCCNTENA_BIT 0


#define WARMUP_HEAT 1
/*
int Config_cnt = 0;
int read_cnt = 0;
void Config_execute()
{
	Config_cnt++;
}
void read_execute()
{
	read_cnt++;
}
*/


#include <stdint.h>

int __attribute__ ((used))
main (int argc __attribute__ ((unused)),
      char *argv[] __attribute__ ((unused)))
{
  int i;
  volatile int result;
  int correct;
  /*
  DWT_CTRL |= (1 << CYCCNTENA_BIT);

	uint32_t *pRccAhb1enr = (uint32_t*)0x40023830;          //      RCC AHB1 peripheral clock enable register
	uint32_t *pGpiodModeReg = (uint32_t*)0x40020C00;        //      GPIOD Mode register
	uint32_t *pGpiodDataReg = (uint32_t*)0x40020C14;
    *pRccAhb1enr |= ( 1 << 3);
    //configure LED_GREEN
    *pGpiodModeReg |= ( 1 << (2 * 12));

    *pGpiodDataReg &= ~(1 << 12);
	*/
  initialise_benchmark ();
//  warm_caches (WARMUP_HEAT);

//  start_trigger ();
  result = benchmark ();
//  stop_trigger ();

  /* bmarks that use arrays will check a global array rather than int result */

  correct = verify_benchmark (result);

  return (!correct);

}				/* main () */


/*
   Local Variables:
   mode: C
   c-file-style: "gnu"
   End:
*/
