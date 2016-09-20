/*
Copyright (c) 2016, prpl Foundation

Permission to use, copy, modify, and/or distribute this software for any purpose with or without 
fee is hereby granted, provided that the above copyright notice and this permission notice appear 
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

This code was written by Carlos Moratelli at Embedded System Group (GSE) at PUCRS/Brazil.

*/


#include <types.h>
#include <hal.h>
#include <config.h>
#include <kernel.h>
#include <tlb.h>
#include <malloc.h>
#include <libc.h>
#include <vcpu.h>
#include <dispatcher.h>
#include <globals.h>
#include <hypercall.h>
#include <common.h>
#include <mips_cp0.h>


extern _heap_size;


#define STR(x) #x
#define STR_VALUE(x) STR(x)


static void print_config(void)
{
        printf("\n===========================================================");
        printf("\nprplHypervsior %s [%s, %s]", STR_VALUE(HYPVERSION), __DATE__, __TIME__);
        printf("\nCopyright (c) 2016, prpl Foundation");
        printf("\n===========================================================");
        printf("\nCPU ID:        %s", CPU_ID);
        printf("\nARCH:          %s", CPU_ARCH);
        printf("\nSYSCLK:        %dMHz", CPU_SPEED/1000000);
        printf("\nHeap Size:     %dKbytes", (int)(&_heap_size)/1024);
        printf("\nScheduler      %dms", QUANTUM_SCHEDULER);
        printf("\nVMs:           %d\n", NVMACHINES);
}


/** C code entry. Called from hal/$(BOARD)/boot.S */
int32_t hyper_init(){
    
    /* Specific board configuration. */
    early_platform_init();    
    
    print_config();
    
    /* Now inialize the hardware */
    /* Processor inicialization */
    if(LowLevelProcInit()){
        return 1;
    }
            
    /* Initialize memory */
    /* Register heap space on the allocator */ 
     if(init_mem()){        
        return 1;
    }
    
    /*Initialize processor structure*/
    if(initProc()){
        return 1;
    } 
    
    if(initializeShedulers()){
        return 1;
    }
    
    /*Initialize vcpus and virtual machines*/
    initializeMachines();

    /* Initialize device drivers */    
    drivers_init();

    /* Run scheduler .*/
    runScheduler();  
    
    hal_start_hyper();

    /* start system timer */
    start_timer();
    
    /* Should never reach this point !!! */
    return 0;
}

/** Handle guest exceptions */
static uint32_t GuestExitException(){
	uint32_t guestcause = getGCauseCode();
	uint32_t epc = getEPC();
	uint32_t ret = SUCEEDED;
	
	switch (guestcause) {
	    case 0x0:	
			ret =  InstructionEmulation(epc);
	    	break;
	    case 0x2:
			ret = hypercall_execution();			
			break;
		default:
			break;
	}
	
	curr_vcpu->pc = epc+4;
        setEPC(curr_vcpu->pc);
	return ret;
}


void configureGuestExecution(uint32_t exCause){
	
	uint32_t count;
	uint32_t currentCount;
	uint32_t elapsedTime;
    
	if(exCause == RESCHEDULE || exCause == CHANGE_TO_TARGET_VCPU){
		dispatcher();
    }

	
    contextRestore();
}


void general_exception_handler(){
        uint32_t CauseCode = getCauseCode();

        switch (CauseCode){
        case    GUESTEXITEXCEPTION:   
                        GuestExitException();
                        break;

        /* TLB load, store or fetch exception */
        case    0x3:                                            
        case    0x2:
                        Warning("\nTLB miss: VCPU: %d EPC 0x%x", curr_vcpu->id, getEPC());
                        while(1);
        default:
                /* panic */
                Warning("VM will be stopped due to error Cause Code 0x%x, EPC 0x%x, VCPU ID 0x%x", CauseCode, getEPC(), curr_vcpu->id);
                while(1);
        }
        
}
