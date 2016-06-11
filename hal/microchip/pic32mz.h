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

#define PIC32REG(a)      *(volatile unsigned*) (0xBF800000 + (a))

/* UART 2 control registers */
#define U2MODE      PIC32REG (0x22200)
#define U2MODECLR   PIC32REG (0x22204)
#define U2MODESET   PIC32REG (0x22208)
#define U2MODEINV   PIC32REG (0x2220C)
#define U2STA       PIC32REG (0x22210)
#define U2STACLR    PIC32REG (0x22214)
#define U2STASET    PIC32REG (0x22218)
#define U2STAINV    PIC32REG (0x2221C)
#define U2TXREG     PIC32REG (0x22220)
#define U2RXREG     PIC32REG (0x22230)
#define U2BRG       PIC32REG (0x22240)
#define U2BRGCLR    PIC32REG (0x22244)
#define U2BRGSET    PIC32REG (0x22248)
#define U2BRGINV    PIC32REG (0x2224C)

/* UART important bits */
#define UTXBF       (1<<9)
#define UTXEN       (1<<10)
#define RTSMD       (1<<11)
#define URXEN       (1<<12)
#define UTXINV      (1<<13)
#define FRZ         (1<<14)
#define ON          (1<<15)

/* Port H control registers */
#define ANSELH      PIC32REG (0x60700) 
#define ANSELHCLR   PIC32REG (0x60704)
#define ANSELHSET   PIC32REG (0x60708)
#define ANSELHINV   PIC32REG (0x6070C)
#define TRISH       PIC32REG (0x60710)
#define TRISHCLR    PIC32REG (0x60714)
#define TRISHSET    PIC32REG (0x60718)
#define TRISHINV    PIC32REG (0x6071C)
#define PORTH       PIC32REG (0x60720)
#define PORTHCLR    PIC32REG (0x60724)
#define PORTHSET    PIC32REG (0x60728)
#define PORTHINV    PIC32REG (0x6072C)
#define LATH        PIC32REG (0x60730)
#define LATHCLR     PIC32REG (0x60734)
#define LATHSET     PIC32REG (0x60738)
#define LATHINV     PIC32REG (0x6073C)

/* Port H control registers */
#define ANSELD      PIC32REG (0x60300)
#define ANSELDCLR   PIC32REG (0x60304)
#define ANSELDSET   PIC32REG (0x60308)
#define ANSELDINV   PIC32REG (0x6030C)
#define TRISD       PIC32REG (0x60310)
#define TRISDCLR    PIC32REG (0x60314)
#define TRISDSET    PIC32REG (0x60318)
#define TRISDINV    PIC32REG (0x6031C)
#define PORTD       PIC32REG (0x60320)
#define PORTDCLR    PIC32REG (0x60324)
#define PORTDSET    PIC32REG (0x60328)
#define PORTDINV    PIC32REG (0x6032C)
#define LATD        PIC32REG (0x60330)
#define LATDCLR     PIC32REG (0x60334)
#define LATDSET     PIC32REG (0x60338)
#define LATDINV     PIC32REG (0x6033C)