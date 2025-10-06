/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"

static XIntc Intc;

int Count = 0;
void BTN_handler0(void *Callback);
void BTN_handler1(void *Callback);

u32 *GPIO = XPAR_GPIO_0_BASEADDR;

int main()
{
	int Status;

	// initialize the interrupt controller
	Status = XIntc_Initialize(&Intc, XPAR_AXI_INTC_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// connect the first handler
	Status = XIntc_Connect(&Intc, XPAR_AXI_INTC_0_DEBOUNCER_0_INTOUT_INTR,
			  (XInterruptHandler)BTN_handler0, XPAR_AXI_INTC_0_DEBOUNCER_0_INTOUT_INTR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = XIntc_Connect(&Intc, XPAR_AXI_INTC_0_DEBOUNCER_1_INTOUT_INTR,
			  (XInterruptHandler)BTN_handler1, XPAR_AXI_INTC_0_DEBOUNCER_1_INTOUT_INTR);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// enable the interrupts
	XIntc_Enable(&Intc, XPAR_AXI_INTC_0_DEBOUNCER_0_INTOUT_INTR);
	XIntc_Enable(&Intc, XPAR_AXI_INTC_0_DEBOUNCER_1_INTOUT_INTR);

	// Start the interrupt controller in real mode
	Status = XIntc_Start(&Intc, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// Initialize the Exception table
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
						 (Xil_ExceptionHandler) XIntc_InterruptHandler,
						 &Intc);
	Xil_ExceptionEnable();

	// the while Loop waiting for an interrupt
	while (1);

	return 0;
	}

void BTN_handler0(void *Callback)
{
	Count++;
	GPIO[0] = Count;
}
void BTN_handler1(void *Callback)
{
	Count++;
	GPIO[2] = Count;
}
