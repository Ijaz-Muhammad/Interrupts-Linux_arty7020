/***************************** Include Files *********************************/
#include <stdio.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xil_util.h"
#include "xscugic.h"

static XScuGic Intc;

int Count = 0;
void BTN_handler0(void *Callback);
void BTN_handler1(void *Callback);

int main()
{
	int Status;
	XScuGic_Config *IntcConfig;

	// Initialization functions in xscugic_sinit.c
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}
	// initialize the interrupt controller
	Status = XScuGic_CfgInitialize(&Intc, IntcConfig, IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// configure the interrupts parameters
	XScuGic_SetPriorityTriggerType(&Intc, XPS_FPGA0_INT_ID, 0x0, 0x3);
	XScuGic_SetPriorityTriggerType(&Intc, XPS_FPGA1_INT_ID, 0x0, 0x3);

	// connect the the interrupts to the handlers
	Status = XScuGic_Connect(&Intc, XPS_FPGA0_INT_ID,
				(Xil_InterruptHandler)BTN_handler0, &Intc);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XScuGic_Connect(&Intc, XPS_FPGA1_INT_ID,
				(Xil_InterruptHandler)BTN_handler1, &Intc);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// enable the Interrupts
	XScuGic_Enable(&Intc, XPS_FPGA0_INT_ID);
	XScuGic_Enable(&Intc, XPS_FPGA1_INT_ID);

	// Initialize the Exception table
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
						 (Xil_ExceptionHandler) XScuGic_InterruptHandler,
						 &Intc);
	Xil_ExceptionEnable();

	// the while Loop waiting for an interrupt
	while (1);

	return 0;
	}

void BTN_handler0(void *Callback)
{
	Count++;
	xil_printf("First Interrupt Numbr %d\n\r",Count);
}
void BTN_handler1(void *Callback)
{
	Count++;
	xil_printf("Second Interrupt Numbr %d\n\r",Count);
}
