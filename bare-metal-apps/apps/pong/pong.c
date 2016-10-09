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

/*************************************************************
 * Ping-Pong application - Inter-VM communication.
 * 
 * To execute the Ping-Pong set the CFG_FILE on the main 
 * Makefile to the sample-2VMs.cfg configuration file.  
 * 
 */

#include <arch.h>
#include <libc.h>
#include <guest_interrupts.h>
#include <hypercalls.h>


volatile int32_t t2 = 0;

void irq_timer(){
	t2++;     
}


char buffer[128];
char resp[] = "pong!";

int main() {
	uint32_t source;
	int32_t ret;
    
	interrupt_register(irq_timer, GUEST_TIMER_INT);
    
	serial_select(UART2);
	printf("\npong VM ID %d", get_guestid());
	while (1){
		ret = ReceiveMessage(&source, buffer, sizeof(buffer), 1);
		if (ret<0){
			print_net_error(ret);
		}else{
			printf("\npong VM: message from VM ID %d: \"%s\" (%d bytes)", source, buffer, ret);
			SendMessage(source, resp, strlen(resp)+1);
		}
	}
    
	return 0;
}

