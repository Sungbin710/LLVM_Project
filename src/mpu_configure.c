#include "mpu_configure.h"

//void MPU_Config(uint32_t start_address, uint32_t size, uint32_t num) 
void MPU_Config(uint32_t start_address, uint32_t size, uint32_t num) __attribute__((section(".MPU")))
{
        /* MPU Disable */
        /* Disable the MPU and clear the control register*/
		SCB->SHCSR &= ~(1 << 16);
        MPU->CTRL = 0U;

        MPU->RNR = num;
        //MPU->RBAR = (start_address & 0xFFFFFFE0);
        MPU->RBAR = (start_address);

		//MPU->RASR = 0;
		MPU->RASR = (0b111 << AP_BIT) | ((size & 0x1F) << SIZE_BIT) | (0b1 << ENABLE_BIT);

        /* MPU Enable */
        MPU->CTRL = (1 << 2) | (1 << 0);
        //MPU->CTRL = 1;
		SCB->SHCSR |= (1 << 16);
}

void MPU_zero_region(uint32_t start_address, uint32_t size, uint32_t num) __attribute__((section(".MPU")))
{
        /* MPU Disable */
        /* Disable fault exceptions */
//        SCB->SHCSR &= ~(1 << 16);

        /* Disable the MPU and clear the control register*/
		SCB->SHCSR &= ~(1 << 16);
        MPU->CTRL = 0U;

        MPU->RNR = num;
        MPU->RBAR = (start_address & 0xFFFFFFE0);

        MPU->RASR = ((0b1) << XN_BIT) | /* no execute */                 /* Instruction fetches enabled */
                                ((0b011) << AP_BIT)|   /*full access*/   /* No access(P,U) */
                                ((0b000) << TEX_BIT)|
                                ((0b0) << S_BIT)|
                                ((0b0) << C_BIT)|
                                ((0b0) << B_BIT)|
                                ((0b00000000) << SRD_BIT)|
                                ((size & 0x1F) << SIZE_BIT)|     /* 32Byte Region */
                                ((0b1) << ENABLE_BIT);

        /* MPU Enable */
        MPU->CTRL = (1 << 2) | (1 << 0);
        //MPU->CTRL = 1;
        SCB->SHCSR |= (1 << 16);
}
/*

void MPU_Disable()
{
	SCB->SHCSR &= ~(1 << 16);
}
void MPU_Enable()
{
	SCB->SHCSR |= (1 << 16);
}
*/

uint32_t Read_Caller_addr()__attribute__((section(".MPU")))
{
	uint32_t addr;
	MPU->RNR = 1;
	addr = ((MPU->RBAR) & (0xFFFFFFE0));
	return addr;
}

uint32_t Read_Caller_size()__attribute__((section(".MPU")))
{
	uint32_t size;
	MPU->RNR = 1;
	size = ((MPU->RASR) >> 1) & 0x1F;
	return size;
}
