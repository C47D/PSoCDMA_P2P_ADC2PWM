#include <project.h>
#include "cyapicallbacks.h"

/* Defines for DMA */
#define DMA_BYTES_PER_BURST 1
#define DMA_REQUEST_PER_BURST 1
#define DMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA_DST_BASE (CYDEV_PERIPH_BASE)

volatile uint8_t foo;

void DMA_Config(void);

int main(){
    
    CyGlobalIntEnable;

    /* Components init */
    PWM_Start();
    ADC_Start();
    isr_Start();
    
    /* DMA configuration */
    DMA_Config();
    
    /* Start the ADc convertion */
    ADC_StartConvert();

    for(;;){
        /* Here we can put the MCU to sleep to save amps */
    }
}

void DMA_Config(){

    /* Variable declarations for DMA */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_Chan;
    uint8 DMA_TD[1];

    /* DMA Configuration for DMA */
    DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST,
                                    DMA_REQUEST_PER_BURST,
                                    HI16(DMA_SRC_BASE),
                                    HI16(DMA_DST_BASE));
    DMA_TD[0] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_TD[0], 1, DMA_TD[0], DMA__TD_TERMOUT_EN);
    /* Transfer data from ADC Result register to PWM CMP register */
    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)ADC_DEC_SAMP_PTR), LO16((uint32)PWM_COMPARE1_LSB_PTR));
    CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    CyDmaChEnable(DMA_Chan, 1);
}

void isr_Interrupt_InterruptCallback(void){
    foo ^= 1; 
}

/* [] END OF FILE */
