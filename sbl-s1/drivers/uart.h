// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>

#include <mcom03-errors.h>

#define UART0		 (0U)
#define UART1		 (1U)
#define UART2		 (2U)
#define UART3		 (3U)
#define UART_MAX_NUMBER	 (3U)
#define UART_DEFAULT	 UART0
#define BAUDRATE_DEFAULT (115200)

#define ON  1
#define OFF 0

#define SET   1
#define RESET 0

#define LOOPBACK_ON   (1U)
#define LOOPBACK_OFF  (0U)
#define TRANSMITE_OFF (1U)
#define TRANSMITE_ON  (0U)

/*------------- RBR  ( reset value 0, offset 0x0 )  ( Read only ) Receiver Buffer Register -------------*/
// This register is available only if the DLAB bit (LCR [7]) is 0x0
// Range                    Name              Description
// bits [31:8]        -           reserve
// bits [7:0]        RBR          The register of input data
#define UART_RBR_OFFSET	    0
#define UART_RBR_RESETVALUE 0
#define UART_RBR_MASK	    0xFF

#define UART_RBR_RBR_POS      0
#define UART_RBR_RBR_MASK     (0xff << UART_RBR_RBR_POS)
#define UART_RBR_RBR(val)     (((val)&0xff) << UART_RBR_RBR_POS)
#define UART_RBR_RBR_GET(reg) (((reg) >> UART_RBR_RBR_POS) & 0xff)

/*------------ THR ( reset value 0, offset 0x0 )  ( Write only )  Transmitte buffer register ---------*/
// This register is available only if the DLAB bit (LCR [7]) is 0x0
// Range                    Name              Description
// bits [31:8]              -              reserve
// bits [7:0]               THR            The register of output data
#define UART_THR_OFFSET	    0
#define UART_THR_RESETVALUE 0
#define UART_RBR_MASK	    0xFF

#define UART_THR_THR_POS      0
#define UART_THR_THR_MASK     (0xff << UART_THR_THR_POS)
#define UART_THR_THR(val)     (((val)&0xff) << UART_THR_THR_POS)
#define UART_THR_THR_GET(reg) (((reg) >> UART_THR_THR_POS) & 0xff)

/*------------  DLL ( reset value 0, offset 0x0 )  ( R/W )  Lower register of frequency divider -----------------*/
// This register is available only if the DLAB bit (LCR [7]) is 0x1
// Range                    Name            Description
// bits [31:8]              -               reserve
// bits [7:0]               DLL             Lowest 8 bits of the frequency divider register

#define UART_DLL_OFFSET	    0
#define UART_DLL_RESETVALUE 0
#define UART_DLL_MASK	    0xFF

#define UART_DLL_DLL_POS      0
#define UART_DLL_DLL_MASK     (0xff << UART_DLL_DLL_POS)
#define UART_DLL_DLL(val)     (((val)&0xff) << UART_DLL_DLL_POS)
#define UART_DLL_DLL_GET(reg) (((reg) >> UART_DLL_DLL_POS) & 0xff)

/*-------- DLH ( reset value 0, offset 0x4 )  ( R/W )  High register of the frequency divider DLH ----------------*/
// This register is available only if the DLAB bit (LCR [7]) is 0x1
// The data transfer rate is determined by the formula  BAUD = frequency UART / (16 * DL), where DL = {DH, DL}
// If DLL and DLH are set to 0, data transfer via the serial port is disabled, after setting the DLH register value, at least 8 UART clock cycles must pass
// Range                    Name              Description
// bits [31:8]              -               reserve
// bits [7:0]               DLH             High 8 bits of the frequency divider register

#define UART_DLH_OFFSET	    0x4
#define UART_DLH_RESETVALUE 0
#define UART_DHL_MASK	    0xFF

#define UART_DLH_DLH_POS      0
#define UART_DLH_DLH_MASK     (0xff << UART_DLH_DLH_POS)
#define UART_DLH_DLH(val)     (((val)&0xff) << UART_DLH_DLH_POS)
#define UART_DLH_DLH_GET(reg) (((reg) >> UART_DLH_DLH_POS) & 0xff)

/*--------- IER ( reset value 0, offset 0x4 ) ( R/W ) Interrupt enable register IER ---------*/
// This register is available only if the DLAB bit (LCR [7]) is 0x0
// Range                    Name            Description
// bits [31:8]                -             reserve
// bits [7]                PTIME            programmable interrupt threshold 0x0 – off 0x1 – on
// bits [6:5]              -                reserve
// bits [4]                ELCOLR           status reset method 0x0 - The status field of the LSR register are cleared either by reading the Rx FIFO or by reading the LSR register. 0x1 - Status fields are cleared only by reading the LSR register
// bits [3]                EDSSI            Enable interrupt by modem status 0x1 – enable  0x0 – disable
// bits [2]                ELSI             Enable interrupt on the state of the receive line. 0x1 - enable 0x0 - disable
// bits [1]                ETBEI            THRE register interrupt enabl. 0x1 – enable 0x0 – disable
// bits [0]                ERBFI            Enable Received Data Available Interrupt 0x1 - enable 0x0 - disable

#define UART_IER_OFFSET	    0x4
#define UART_IER_RESETVALUE 0
#define UART_IER_MASK	    0xFF

#define UART_IER_PTIME_POS	7
#define UART_IER_PTIME_MASK	(0x1 << UART_IER_PTIME_POS)
#define UART_IER_PTIME(val)	(((val)&0x1) << UART_IER_PTIME_POS)
#define UART_IER_PTIME_GET(reg) (((reg) >> UART_IER_PTIME_POS) & 0x1)

#define UART_IER_ELCOLR_POS	 4
#define UART_IER_ELCOLR_MASK	 (0x1 << UART_IER_ELCOLR_POS)
#define UART_IER_ELCOLR(val)	 (((val)&0x1) << UART_IER_ELCOLR_POS)
#define UART_IER_ELCOLR_GET(reg) (((reg) >> UART_IER_ELCOLR_POS) & 0x1)

#define UART_IER_EDSSI_POS	3
#define UART_IER_EDSSI_MASK	(0x1 << UART_IER_EDSSI_POS)
#define UART_IER_EDSSI(val)	(((val)&0x1) << UART_IER_EDSSI_POS)
#define UART_IER_EDSSI_GET(reg) (((reg) >> UART_IER_EDSSI_POS) & 0x1)

#define UART_IER_ELSI_POS      2
#define UART_IER_ELSI_MASK     (0x1 << UART_IER_ELSI_POS)
#define UART_IER_ELSI(val)     (((val)&0x1) << UART_IER_ELSI_POS)
#define UART_IER_ELSI_GET(reg) (((reg) >> UART_IER_ELSI_POS) & 0x1)

#define UART_IER_ETBEI_POS	1
#define UART_IER_ETBEI_MASK	(0x1 << UART_IER_ETBEI_POS)
#define UART_IER_ETBEI(val)	(((val)&0x1) << UART_IER_ETBEI_POS)
#define UART_IER_ETBEI_GET(reg) (((reg) >> UART_IER_ETBEI_POS) & 0x1)

#define UART_IER_ERBFI_POS	0
#define UART_IER_ERBFI_MASK	(0x1 << UART_IER_ERBFI_POS)
#define UART_IER_ERBFI(val)	(((val)&0x1) << UART_IER_ERBFI_POS)
#define UART_IER_ERBFI_GET(reg) (((reg) >> UART_IER_ERBFI_POS) & 0x1)

/*----------- FCR ( reset value 0, offset 0x8 ) ( R/W???? )  FIFO control register  -----------*/
// Range                    Name             Description
// bits [31:8]              -                reserved
// bits [7:6 ]              RT               Receiver FIFO interrupt threshold.
// bits [5:4 ]              TET              THRE interrupt level control
// bits [3]                 DMAM             Reserve
// bits [2]                 XFIFOR           Resetting the FIFO of the transmitter. The bit is cleared after writing.
// bits [1]                 RFIFOR           Resetting the FIFO of the receiver.
// bits [0]                 FIFOE            Control FIFO 0 - FIFO off 1 - FIFO on

#define UART_FCR_OFFSET	    0x8
#define UART_FCR_RESETVALUE 0
#define UART_FCR_MASK	    0xFF

#define UART_FCR_RT_POS	     6
#define UART_FCR_RT_MASK     (0x3 << UART_FCR_RT_POS)
#define UART_FCR_RT(val)     (((val)&0x3) << UART_FCR_RT_POS)
#define UART_FCR_RT_GET(reg) (((reg) >> UART_FCR_RT_POS) & 0x3)

#define UART_FCR_TET_POS      4
#define UART_FCR_TET_MASK     (0x3 << UART_FCR_TET_POS)
#define UART_FCR_TET(val)     (((val)&0x3) << UART_FCR_TET_POS)
#define UART_FCR_TET_GET(reg) (((reg) >> UART_FCR_TET_POS) & 0x3)

#define UART_FCR_DMAM_POS      3
#define UART_FCR_DMAM_MASK     (0x3 << UART_FCR_DMAM_POS)
#define UART_FCR_DMAM(val)     (((val)&0x3) << UART_FCR_DMAM_POS)
#define UART_FCR_DMAM_GET(reg) (((reg) >> UART_FCR_DMAM_POS) & 0x3)

#define UART_FCR_XFIFOR_POS	 2
#define UART_FCR_XFIFOR_MASK	 (0x1 << UART_FCR_XFIFOR_POS)
#define UART_FCR_XFIFOR(val)	 (((val)&0x1) << UART_FCR_XFIFOR_POS)
#define UART_FCR_XFIFOR_GET(reg) (((reg) >> UART_FCR_XFIFOR_POS) & 0x1)

#define UART_FCR_RFIFOR_POS	 1
#define UART_FCR_RFIFOR_MASK	 (0x1 << UART_FCR_RFIFOR_POS)
#define UART_FCR_RFIFOR(val)	 (((val)&0x1) << UART_FCR_RFIFOR_POS)
#define UART_FCR_RFIFOR_GET(reg) (((reg) >> UART_FCR_RFIFOR_POS) & 0x1)

#define UART_FCR_FIFOE_POS	0
#define UART_FCR_FIFOE_MASK	(0x1 << UART_FCR_FIFOE_POS)
#define UART_FCR_FIFOE(val)	(((val)&0x1) << UART_FCR_FIFOE_POS)
#define UART_FCR_FIFOE_GET(reg) (((reg) >> UART_FCR_FIFOE_POS) & 0x1)

/*------  IIR ( reset value 0, offset 0x8 ) ( R/W )  Interrupt identification register ------*/
// Range                    Name                Description
// bits [31:8]              reserve
// bits [7:6]               FIFOSE              FIFO working status indication. 0x0 - disabled 0x1 - enabled
// bits [5:4]               reserve
// bits [3:0]               IID                 Interrupt ID (IID). Displays the interrupt with the highest priority
//                                0x0110 – receiver line status
//                                   0x1000 – data available on the receiving line
//                                   0x1100 – receiver timeout
//                                   0x0010 – interrupt THRE
//                                   0x0000 – modem status
//                                   0x0111 – write attempt detection
#define UART_IIR_OFFSET	    0x8
#define UART_IIR_RESETVALUE 0
#define UART_IIR_MASK	    0xFF

#define UART_IIR_FIFOSE_POS	 6
#define UART_IIR_FIFOSE_MASK	 (0x3 << UART_IIR_FIFOSE_POS)
#define UART_IIR_FIFOSE(val)	 (((val)&0x3) << UART_IIR_FIFOSE_POS)
#define UART_IIR_FIFOSE_GET(reg) (((reg) >> UART_IIR_FIFOSE_POS) & 0x3)

#define UART_IIR_IID_POS      0
#define UART_IIR_IID_MASK     (0xf << UART_IIR_IID_POS)
#define UART_IIR_IID(val)     (((val)&0xf) << UART_IIR_IID_POS)
#define UART_IIR_IID_GET(reg) (((reg) >> UART_IIR_IID_POS) & 0xf)

/*-------- LCR  ( reset value 0, offset 0xC ) ( R/W )    Line control register --------*/
// Range                    Name         Description
// bits [31:8]             reserved
// bits [7]                DLAB           Divider access bit, used to enable reading and writing of DLL / LPDLL and LPDLH registers. 0x0 - disabled 0x1 - enabled
// bits [6]                BC             Line break bit. 0x0 - normal operation. 0x1 - sout is set to low.
// bits [5]                SP             Parity setting bit. Writable when USR [0] is not 0, used to set the parity.
// bits [4]                EPS            Parity parity bit 0x0 - odd 0x1 - even
// bits [3]                PEN            Parity resolution
// bits [2]                STOP           Number of stop bits 0x0 - 1 stop bit, 0x1 - 1.5 stop bits, if LCR [0: 1] = 0, otherwise 2 stop bits.
// bits [1:0]              DLS            The number of data bits in the transmitted character. Writable when USR [0] is not 0.
//                                      0x00 – 5 bit
//                                      0x01 – 6 bit
//                                      0x10 – 7 bit
//                                      0x11 – 8 bit

#define UART_LCR_OFFSET	    0xC
#define UART_LCR_RESETVALUE 0
#define UART_LCR_MASK	    0xFF

#define UART_LCR_DLAB_POS      7
#define UART_LCR_DLAB_MASK     (0x1 << UART_LCR_DLAB_POS)
#define UART_LCR_DLAB(val)     (((val)&0x1) << UART_LCR_DLAB_POS)
#define UART_LCR_DLAB_GET(reg) (((reg) >> UART_LCR_DLAB_POS) & 0x1)

#define UART_LCR_BC_POS	     6
#define UART_LCR_BC_MASK     (0x1 << UART_LCR_BC_POS)
#define UART_LCR_BC(val)     (((val)&0x1) << UART_LCR_BC_POS)
#define UART_LCR_BC_GET(reg) (((reg) >> UART_LCR_BC_POS) & 0x1)

#define UART_LCR_SP_POS	     5
#define UART_LCR_SP_MASK     (0x1 << UART_LCR_SP_POS)
#define UART_LCR_SP(val)     (((val)&0x1) << UART_LCR_SP_POS)
#define UART_LCR_SP_GET(reg) (((reg) >> UART_LCR_SP_POS) & 0x1)

#define UART_LCR_EPS_POS      4
#define UART_LCR_EPS_MASK     (0x1 << UART_LCR_EPS_POS)
#define UART_LCR_EPS(val)     (((val)&0x1) << UART_LCR_EPS_POS)
#define UART_LCR_EPS_GET(reg) (((reg) >> UART_LCR_EPS_POS) & 0x1)

#define UART_LCR_PEN_POS      3
#define UART_LCR_PEN_MASK     (0x1 << UART_LCR_PEN_POS)
#define UART_LCR_PEN(val)     (((val)&0x1) << UART_LCR_PEN_POS)
#define UART_LCR_PEN_GET(reg) (((reg) >> UART_LCR_PEN_POS) & 0x1)

#define UART_LCR_STOP_POS      2
#define UART_LCR_STOP_MASK     (0x1 << UART_LCR_STOP_POS)
#define UART_LCR_STOP(val)     (((val)&0x1) << UART_LCR_STOP_POS)
#define UART_LCR_STOP_GET(reg) (((reg) >> UART_LCR_STOP_POS) & 0x1)

#define UART_LCR_DLS_POS      0
#define UART_LCR_DLS_MASK     (0x3 << UART_LCR_DLS_POS)
#define UART_LCR_DLS(val)     (((val)&0x3) << UART_LCR_DLS_POS)
#define UART_LCR_DLS_GET(reg) (((reg) >> UART_LCR_DLS_POS) & 0x3)

/*-------- MCR ( reset value 0, offset 0x10 ) ( R/W )   Modem control register --------*/
// Range                    Name           Description
//  bits [31:7]             reserve
//  bits [6]                SIRE           IR mode activation
//  bits [5]                AFCE           Enables automatic flow control control. Possible if FIFO is enabled (FCR [0] = 1). 0 - disabled 1 - enabled
//  bits [4]                LoopBack       Loopback Activation Used to activate diagnostic mode for testing purposes.
//  bits [3]                OUT2           Programmable output 2 The value written in the field is inverted and transmitted to the output
//  bits [2]                OUT1           Programmable output 1 The value written in the field is inverted and transmitted to the output
//  bits [1]                RTS            Request for sending data. This bit is used to control the RTS_N pin. The RTS_N pin is used to inform the terminal modem that the UART is ready to communicate.
//  bits [0]                DTR            Dtr_n line control Used to directly control the dtr_n signal.
#define UART_MCR_OFFSET	    0x10
#define UART_MCR_RESETVALUE 0
#define UART_MCR_MASK	    0x7F

#define UART_MCR_SIRE_POS      6
#define UART_MCR_SIRE_MASK     (0x1 << UART_MCR_SIRE_POS)
#define UART_MCR_SIRE(val)     (((val)&0x1) << UART_MCR_SIRE_POS)
#define UART_MCR_SIRE_GET(reg) (((reg) >> UART_MCR_SIRE_POS) & 0x1)

#define UART_MCR_AFCE_POS      5
#define UART_MCR_AFCE_MASK     (0x1 << UART_MCR_AFCE_POS)
#define UART_MCR_AFCE(val)     (((val)&0x1) << UART_MCR_AFCE_POS)
#define UART_MCR_AFCE_GET(reg) (((reg) >> UART_MCR_AFCE_POS) & 0x1)

#define UART_MCR_LOOPBACK_POS	   4
#define UART_MCR_LOOPBACK_MASK	   (0x1 << UART_MCR_LOOPBACK_POS)
#define UART_MCR_LOOPBACK(val)	   (((val)&0x1) << UART_MCR_LOOPBACK_POS)
#define UART_MCR_LOOPBACK_GET(reg) (((reg) >> UART_MCR_LOOPBACK_POS) & 0x1)

#define UART_MCR_OUT2_POS      3
#define UART_MCR_OUT2_MASK     (0x1 << UART_MCR_OUT2_POS)
#define UART_MCR_OUT2(val)     (((val)&0x1) << UART_MCR_OUT2_POS)
#define UART_MCR_OUT2_GET(reg) (((reg) >> UART_MCR_OUT2_POS) & 0x1)

#define UART_MCR_OUT1_POS      2
#define UART_MCR_OUT1_MASK     (0x1 << UART_MCR_OUT1_POS)
#define UART_MCR_OUT1(val)     (((val)&0x1) << UART_MCR_OUT1_POS)
#define UART_MCR_OUT1_GET(reg) (((reg) >> UART_MCR_OUT1_POS) & 0x1)

#define UART_MCR_RTS_POS      1
#define UART_MCR_RTS_MASK     (0x1 << UART_MCR_RTS_POS)
#define UART_MCR_RTS(val)     (((val)&0x1) << UART_MCR_RTS_POS)
#define UART_MCR_RTS_GET(reg) (((reg) >> UART_MCR_RTS_POS) & 0x1)

#define UART_MCR_DTR_POS      0
#define UART_MCR_DTR_MASK     (0x1 << UART_MCR_DTR_POS)
#define UART_MCR_DTR(val)     (((val)&0x1) << UART_MCR_DTR_POS)
#define UART_MCR_DTR_GET(reg) (((reg) >> UART_MCR_DTR_POS) & 0x1)

/*--------- LSR ( reset value 0, offset 0x14 )  ( Read only ) Line state register --------*/
// Range                    Name            Description
// bits [31:8]              reserve
// bits [7]                 RFE             Receiver error bit. Cleared when read by LSR.
// bits [6]                 TEMT            No data transmitted bit. It is set to 1 if there is no data in the FIFO buffer and the shift register of the transmitter.
// bits [5]                 THRE            No data bit in the transmitter buffer. The bit is cleared when a character is written to the THR register.
// bits [4]                 BI              Line break. The bit is cleared when the contents of the LSR are read.
// bits [3]                 FE              Cropping error. The bit is cleared when the contents of the LSR are read.
// bits [2]                 PE              Parity (check bit) error. The bit is cleared when the contents of the LSR are read.
// bits [1]                 OE              Overflow error. The bit is cleared when the contents of the LSR are read. 0x0
// bits [0]                 DR              Data Ready Bit Indicates the presence of data in the receiver that has not yet been read.
#define UART_LSR_OFFSET	    0x14
#define UART_LSR_RESETVALUE 0
#define UART_LSR_MASK	    0xFF

#define UART_LSR_RFE_POS      7
#define UART_LSR_RFE_MASK     (0x1 << UART_LSR_RFE_POS)
#define UART_LSR_RFE(val)     (((val)&0x1) << UART_LSR_RFE_POS)
#define UART_LSR_RFE_GET(reg) (((reg) >> UART_LSR_RFE_POS) & 0x1)

#define UART_LSR_TEMT_POS      6
#define UART_LSR_TEMT_MASK     (0x1 << UART_LSR_TEMT_POS)
#define UART_LSR_TEMT(val)     (((val)&0x1) << UART_LSR_TEMT_POS)
#define UART_LSR_TEMT_GET(reg) (((reg) >> UART_LSR_TEMT_POS) & 0x1)

#define UART_LSR_THRE_POS      5
#define UART_LSR_THRE_MASK     (0x1 << UART_LSR_THRE_POS)
#define UART_LSR_THRE(val)     (((val)&0x1) << UART_LSR_THRE_POS)
#define UART_LSR_THRE_GET(reg) (((reg) >> UART_LSR_THRE_POS) & 0x1)

#define UART_LSR_BI_POS	     4
#define UART_LSR_BI_MASK     (0x1 << UART_LSR_BI_POS)
#define UART_LSR_BI(val)     (((val)&0x1) << UART_LSR_BI_POS)
#define UART_LSR_BI_GET(reg) (((reg) >> UART_LSR_BI_POS) & 0x1)

#define UART_LSR_FE_POS	     3
#define UART_LSR_FE_MASK     (0x1 << UART_LSR_FE_POS)
#define UART_LSR_FE(val)     (((val)&0x1) << UART_LSR_FE_POS)
#define UART_LSR_FE_GET(reg) (((reg) >> UART_LSR_FE_POS) & 0x1)

#define UART_LSR_PE_POS	     2
#define UART_LSR_PE_MASK     (0x1 << UART_LSR_PE_POS)
#define UART_LSR_PE(val)     (((val)&0x1) << UART_LSR_PE_POS)
#define UART_LSR_PE_GET(reg) (((reg) >> UART_LSR_PE_POS) & 0x1)

#define UART_LSR_OE_POS	     1
#define UART_LSR_OE_MASK     (0x1 << UART_LSR_OE_POS)
#define UART_LSR_OE(val)     (((val)&0x1) << UART_LSR_OE_POS)
#define UART_LSR_OE_GET(reg) (((reg) >> UART_LSR_OE_POS) & 0x1)

#define UART_LSR_DR_POS	     0
#define UART_LSR_DR_MASK     (0x1 << UART_LSR_DR_POS)
#define UART_LSR_DR(val)     (((val)&0x1) << UART_LSR_DR_POS)
#define UART_LSR_DR_GET(reg) (((reg) >> UART_LSR_DR_POS) & 0x1)

#define UART_LSR_RECIEVER_ERRORS 0x9e

/*--------- MSR ( reset value 0, offset 0x18 )  ( Read only )  Modem state register -----------*/
//When writing to bits 0, 1, 2 or 3 of logical 1, to display the change in the modem settings, interrupts will be generated by the modem status (if enabled), regardless of whether
//when the recording took place. Bits (bits 0, 1, 3) can be set after reset even if their corresponding modem signals are inactive, since the synchronized version
//Modem Signals has a reset value of 0 and changes to a value of 1 after a reset. To prevent unwanted interruptions, reading the MSR can be done after
//reset.
// Range                    Name             Description
// bits [31:8]               reserve
// bits [7]                  DCD             Detection of storage medium. Used to read the status of the dcd_n line.
// bits [6]                  RI              Access indicator. Used to read the state of the ri_n line.
// bits [5]                  DSR             Data readiness. Used to read the status of the dsr_n line.
// bits [4]                  CTS             Ready to send. Used to read the status of the cts_n line.
// bits [3]                  DDCD            Dcd_n line change detection Determines if the dcd_n line has changed since the last read of the MSR register.
// bits [2]                  TERI            Line change detection ri_n. Determines if the ri_n line has changed since the last read of the MSR register.
// bits [1]                  DDSR            Detection of line change dsr_n. Determines if the dsr_n line has changed since the last read of the MSR register.
// bits [0]                  DCTS            Cts_n line change detection. Determines if the cts_n line has changed since the last read of the MSR register.

#define UART_MSR_OFFSET	    0x18
#define UART_MSR_RESETVALUE 0
#define UART_MSR_MASK	    0xFF

#define UART_MSR_DCD_POS      7
#define UART_MSR_DCD_MASK     (0x1 << UART_MSR_DCD_POS)
#define UART_MSR_DCD(val)     (((val)&0x1) << UART_MSR_DCD_POS)
#define UART_MSR_DCD_GET(reg) (((reg) >> UART_MSR_DCD_POS) & 0x1)

#define UART_MSR_RI_POS	     6
#define UART_MSR_RI_MASK     (0x1 << UART_MSR_RI_POS)
#define UART_MSR_RI(val)     (((val)&0x1) << UART_MSR_RI_POS)
#define UART_MSR_RI_GET(reg) (((reg) >> UART_MSR_RI_POS) & 0x1)

#define UART_MSR_DSR_POS      5
#define UART_MSR_DSR_MASK     (0x1 << UART_MSR_DSR_POS)
#define UART_MSR_DSR(val)     (((val)&0x1) << UART_MSR_DSR_POS)
#define UART_MSR_DSR_GET(reg) (((reg) >> UART_MSR_DSR_POS) & 0x1)

#define UART_MSR_CTS_POS      4
#define UART_MSR_CTS_MASK     (0x1 << UART_MSR_CTS_POS)
#define UART_MSR_CTS(val)     (((val)&0x1) << UART_MSR_CTS_POS)
#define UART_MSR_CTS_GET(reg) (((reg) >> UART_MSR_CTS_POS) & 0x1)

#define UART_MSR_DDCD_POS      3
#define UART_MSR_DDCD_MASK     (0x1 << UART_MSR_DDCD_POS)
#define UART_MSR_DDCD(val)     (((val)&0x1) << UART_MSR_DDCD_POS)
#define UART_MSR_DDCD_GET(reg) (((reg) >> UART_MSR_DDCD_POS) & 0x1)

#define UART_MSR_TERI_POS      2
#define UART_MSR_TERI_MASK     (0x1 << UART_MSR_TERI_POS)
#define UART_MSR_TERI(val)     (((val)&0x1) << UART_MSR_TERI_POS)
#define UART_MSR_TERI_GET(reg) (((reg) >> UART_MSR_TERI_POS) & 0x1)

#define UART_MSR_DDSR_POS      1
#define UART_MSR_DDSR_MASK     (0x1 << UART_MSR_DDSR_POS)
#define UART_MSR_DDSR(val)     (((val)&0x1) << UART_MSR_DDSR_POS)
#define UART_MSR_DDSR_GET(reg) (((reg) >> UART_MSR_DDSR_POS) & 0x1)

#define UART_MSR_DCTS_POS      0
#define UART_MSR_DCTS_MASK     (0x1 << UART_MSR_DCTS_POS)
#define UART_MSR_DCTS(val)     (((val)&0x1) << UART_MSR_DCTS_POS)
#define UART_MSR_DCTS_GET(reg) (((reg) >> UART_MSR_DCTS_POS) & 0x1)

/*--------------- SCR ( reset value 0, offset 0x1C )   ( R/W ) Scratch control register -------------*/
// Range                    Name             Description
// bits  [31:8]              reserved
// bits  [7:0]               SCR             This register is for temporary storage of data. Has no specific purpose in controlling the controller.
#define UART_SCR_OFFSET	    0x1c
#define UART_SCR_RESETVALUE 0
#define UART_SCR_MASK	    0xFF

#define UART_SCR_SCR_POS      0
#define UART_SCR_SCR_MASK     (0xff << UART_SCR_SCR_POS)
#define UART_SCR_SCR(val)     (((val)&0xff) << UART_SCR_SCR_POS)
#define UART_SCR_SCR_GET(reg) (((reg) >> UART_SCR_SCR_POS) & 0xff)

/*----------- SRBR0 - SRBR15 ( reset value 0, offset 0x30 + n*4 )  ( Read only ) Shadow Receive Buffer Register ---------*/
// Range                    Name            Description
// bits [31:8]              reserved
// bits [7:0]               SRBRn           This register duplicates the RBR register. You can contact him at one of the 32 addresses.
#define UART_SRBR_OFFSET(n)  (0x30 + (n)*4)
#define UART_SRBR_RESETVALUE 0
#define UART_SRBB_MASK	     0xFF

#define UART_SRBR_POS	   0
#define UART_SRBR_MASK	   (0xff << UART_SRBR_POS)
#define UART_SRBR(val)	   (((val)&0xff) << UART_SRBR_POS)
#define UART_SRBR_GET(reg) (((reg) >> UART_SRBR_POS) & 0xff)

/*--------- FAR ( reset value 0, offset 0x70 )  ( R/W  ) FIFO access register --------*/
// Range                    Name          Description
// bits [31:1]              reserved
// bits [0]                 FAR           FIFO access register. Allows to allow direct access to FIFO buffers
#define UART_FAR_OFFSET	    0x70
#define UART_FAR_RESETVALUE 0
#define UART_FAR_MASK	    0x1

#define UART_FAR_FAR_POS      0
#define UART_FAR_FAR_MASK     (0x1 << UART_FAR_FAR_POS)
#define UART_FAR_FAR(val)     (((val)&0x1) << UART_FAR_FAR_POS)
#define UART_FAR_FAR_GET(reg) (((reg) >> UART_FAR_FAR_POS) & 0x1)

/*--------- TFR ( reset value 0, offset 0x74 )  ( Read only ) Transmit FIFO Read ---------*/
// Range                    Name              Description
// bits [31:8]              reserved
// bits [7:0]               TFR             Reading transmitter FIFO data Available in FIFO access mode (FAR [0] = 1).
#define UART_TFR_OFFSET	    0x74
#define UART_TFR_RESETVALUE 0
#define UART_TFR_MASK	    0xFF

#define UART_TFR_TFR_POS      0
#define UART_TFR_TFR_MASK     (0xff << UART_TFR_TFR_POS)
#define UART_TFR_TFR(val)     (((val)&0xff) << UART_TFR_TFR_POS)
#define UART_TFR_TFR_GET(reg) (((reg) >> UART_TFR_TFR_POS) & 0xff)

/*-------- RFW ( reset value 0, offset 0x78 )  ( Read only )  Receive FIFO Write --------*/
// Range                    Name            Description
// bits [31:10]            reserved
// bits [9]                RFFE            Framing error recording. When the FIFO mode is disabled, write an error to the RBR register.
// bits [8]                RFPE            Parity error recording. When the FIFO mode is disabled, it writes an error to the RBR register.
// bits [7:0]              RFWD            Writing to the FIFO of the receiver Available in FIFO access mode (FAR [0] = 1)

#define UART_RFW_OFFSET	    0x78
#define UART_RFW_RESETVALUE 0
#define UART_RFW_MASK	    0x3FF

#define UART_RFW_RFFE_POS      9
#define UART_RFW_RFFE_MASK     (0x1 << UART_RFW_RFFE_POS)
#define UART_RFW_RFFE(val)     (((val)&0x1) << UART_RFW_RFFE_POS)
#define UART_RFW_RFFE_GET(reg) (((reg) >> UART_RFW_RFFE_POS) & 0x1)

#define UART_RFW_RFPE_POS      8
#define UART_RFW_RFPE_MASK     (0x1 << UART_RFW_RFPE_POS)
#define UART_RFW_RFPE(val)     (((val)&0x1) << UART_RFW_RFPE_POS)
#define UART_RFW_RFPE_GET(reg) (((reg) >> UART_RFW_RFPE_POS) & 0x1)

#define UART_RFW_RFWD_POS      0
#define UART_RFW_RFWD_MASK     (0xff << UART_RFW_RFWD_POS)
#define UART_RFW_RFWD(val)     (((val)&0xff) << UART_RFW_RFWD_POS)
#define UART_RFW_RFWD_GET(reg) (((reg) >> UART_RFW_RFWD_POS) & 0xff)

/*--------- USR ( reset value 0, offset 0x7c )  ( Read only )  UART status register -------*/
// Range                    Name            Description
// bits [31:5]              reserved
// bits [4]                 RFF             The receiver's FIFO is full. 0x0 - Receiver FIFO is not full. 0x1 - Receiver FIFO full
// bits [3]                 RFNE            The receiver FIFO is not empty. 0x0 - Receiver FIFO is not empty. 0x1 - Receiver FIFO is empty.
// bits [2]                 TFE             The transmitter FIFO is empty. 0x0 - The transmitter FIFO is not empty. 0x1 - transmitter FIFO is empty
// bits [1]                 TFNF            The transmitter FIFO is not full. 0x0 - Transmitter FIFO is not full. 0x1 - transmitter FIFO full
// bits [0]                 BUSY            UART is busy. This bit is set to 1 in the following cases:
//          1. Currently, data is being transmitted via the serial interface.
//          2. Register THR contains data for transmission, with non-zero value of the frequency divider ({DLH, DLL} ~ = 0) and LCR.DLAB = 0.
//          3. Currently, data is being received via the serial interface.
//          4. Register RBR contains data to be received.
#define UART_USR_OFFSET	    0x7c
#define UART_USR_RESETVALUE 0
#define UART_USR_MASK	    0x1F

#define UART_USR_RFF_POS      4
#define UART_USR_RFF_MASK     (0x1 << UART_USR_RFF_POS)
#define UART_USR_RFF(val)     (((val)&0x1) << UART_USR_RFF_POS)
#define UART_USR_RFF_GET(reg) (((reg) >> UART_USR_RFF_POS) & 0x1)

#define UART_USR_RFNE_POS      3
#define UART_USR_RFNE_MASK     (0x1 << UART_USR_RFNE_POS)
#define UART_USR_RFNE(val)     (((val)&0x1) << UART_USR_RFNE_POS)
#define UART_USR_RFNE_GET(reg) (((reg) >> UART_USR_RFNE_POS) & 0x1)

#define UART_USR_TFE_POS      2
#define UART_USR_TFE_MASK     (0x1 << UART_USR_TFE_POS)
#define UART_USR_TFE(val)     (((val)&0x1) << UART_USR_TFE_POS)
#define UART_USR_TFE_GET(reg) (((reg) >> UART_USR_TFE_POS) & 0x1)

#define UART_USR_TFNF_POS      1
#define UART_USR_TFNF_MASK     (0x1 << UART_USR_TFNF_POS)
#define UART_USR_TFNF(val)     (((val)&0x1) << UART_USR_TFNF_POS)
#define UART_USR_TFNF_GET(reg) (((reg) >> UART_USR_TFNF_POS) & 0x1)

#define UART_USR_BUSY_POS      0
#define UART_USR_BUSY_MASK     (0x1 << UART_USR_BUSY_POS)
#define UART_USR_BUSY(val)     (((val)&0x1) << UART_USR_BUSY_POS)
#define UART_USR_BUSY_GET(reg) (((reg) >> UART_USR_BUSY_POS) & 0x1)

/*-------- TFL ( reset value 0, offset 0x80 )  ( Read only )  Transmit FIFO level ---------*/
// Range                    Name            Description
// bits [31:6]              reserve
// bits [5:0]               tfl             FIFO level of the transmitter. Displays the number of data words in the transmitter FIFO.

#define UART_TFL_OFFSET	    0x80
#define UART_TFL_RESETVALUE 0
#define UART_TFL_MASK	    0x3F

#define UART_TFL_TFL_POS      0
#define UART_TFL_TFL_MASK     (0x3f << UART_TFL_TFL_POS)
#define UART_TFL_TFL(val)     (((val)&0x3f) << UART_TFL_TFL_POS)
#define UART_TFL_TFL_GET(reg) (((reg) >> UART_TFL_TFL_POS) & 0x3f)

/*----------- RFL ( reset value 0, offset 0x84 )  ( Read only ) Receive FIFO Level -----------*/
// Range                    Name            Description
// bits [31:6]              reserve
// bits [5:0]               rfl             Receiver FIFO level. Displays the number of data words in the receiver FIFO.
#define UART_RFL_OFFSET	    0x84
#define UART_RFL_RESETVALUE 0
#define UART_RFL_MASK	    0x3F

#define UART_RFL_RFL_POS      0
#define UART_RFL_RFL_MASK     (0x3f << UART_RFL_RFL_POS)
#define UART_RFL_RFL(val)     (((val)&0x3f) << UART_RFL_RFL_POS)
#define UART_RFL_RFL_GET(reg) (((reg) >> UART_RFL_RFL_POS) & 0x3f)

/*---------- SRR ( reset value 0, offset 0x88 )  ( Read only ) Software Reset Register ---------*/
// Range                    Name              Description
// bits [31:3 ]             reserve
// bits [2]                 XFR             Duplicates FCR [2]
// bits [1]                 RFR             Duplicates FCR [1]
// bits [0]                 UR              UART soft reset
#define UART_SRR_OFFSET	    0x88
#define UART_SRR_RESETVALUE 0
#define UART_SRR_MASK	    0x7

#define UART_SRR_XFR_POS      2
#define UART_SRR_XFR_MASK     (0x1 << UART_SRR_XFR_POS)
#define UART_SRR_XFR(val)     (((val)&0x1) << UART_SRR_XFR_POS)
#define UART_SRR_XFR_GET(reg) (((reg) >> UART_SRR_XFR_POS) & 0x1)

#define UART_SRR_RFR_POS      1
#define UART_SRR_RFR_MASK     (0x1 << UART_SRR_RFR_POS)
#define UART_SRR_RFR(val)     (((val)&0x1) << UART_SRR_RFR_POS)
#define UART_SRR_RFR_GET(reg) (((reg) >> UART_SRR_RFR_POS) & 0x1)

#define UART_SRR_UR_POS	     0
#define UART_SRR_UR_MASK     (0x1 << UART_SRR_UR_POS)
#define UART_SRR_UR(val)     (((val)&0x1) << UART_SRR_UR_POS)
#define UART_SRR_UR_GET(reg) (((reg) >> UART_SRR_UR_POS) & 0x1)

/*------------ SRTS ( reset value 0, offset 0x8c )  ( Read only )  Shadow Request to Send -----------*/
// Range                    Name              Description
// bits [31:1]              reserve
// bits [0]                 SRTS          Duplicates MCR[1]
#define UART_SRTS_OFFSET     0x8C
#define UART_SRTS_RESETVALUE 0
#define UART_SRTS_MASK	     0x1

#define UART_SRTS_SRTS_POS	0
#define UART_SRTS_SRTS_MASK	(0x1 << UART_SRTS_SRTS_POS)
#define UART_SRTS_SRTS(val)	(((val)&0x1) << UART_SRTS_SRTS_POS)
#define UART_SRTS_SRTS_GET(reg) (((reg) >> UART_SRTS_SRTS_POS) & 0x1)

/*-------- SBCR ( reset value 0, offset 0x90 )  ( R/W ) Shadow Break Control Register ----------*/
// Range                    Name              Description
// bits [31:1]             reserved
// bits [0]                 SBCB            Duplicates LCR[6]
#define UART_SBCR_OFFSET     0x90
#define UART_SBCR_RESETVALUE 0
#define UART_SBCR_MASK	     0x1

#define UART_SBCR_SBCB_POS	0
#define UART_SBCR_SBCB_MASK	(0x1 << UART_SBCR_SBCB_POS)
#define UART_SBCR_SBCB(val)	(((val)&0x1) << UART_SBCR_SBCB_POS)
#define UART_SBCR_SBCB_GET(reg) (((reg) >> UART_SBCR_SBCB_POS) & 0x1)

/*------ SDMAM ( reset value 0, offset 0x94 )  ( R/W )  Shadow DMA Mode --------------------*/
// Range                    Name              Description
// bits [31:1]             reserve
// bits [0]                 SDMAM           Duplicates FCR[3]            0x0
#define UART_SDMAM_OFFSET     0x94
#define UART_SDMAM_RESETVALUE 0
#define UART_SDMAM_MASK	      0x1

#define UART_SDMAM_SDMAM_POS	  0
#define UART_SDMAM_SDMAM_MASK	  (0x1 << UART_SDMAM_SDMAM_POS)
#define UART_SDMAM_SDMAM(val)	  (((val)&0x1) << UART_SDMAM_SDMAM_POS)
#define UART_SDMAM_SDMAM_GET(reg) (((reg) >> UART_SDMAM_SDMAM_POS) & 0x1)

/*----- SFE ( reset value 0, offset 0x98 )  ( R/W )  Shadow FIFO Enable ---------------*/
// Range                    Name              Description
// bits [31:1]              reserved
// bit  [0]                 SFE             Duplicates FCR[0]            0x0
#define UART_SFE_OFFSET	    0x98
#define UART_SFE_RESETVALUE 0
#define UART_SFE_MASK	    0x1

#define UART_SFE_SFE_POS      0
#define UART_SFE_SFE_MASK     (0x1 << UART_SFE_SFE_POS)
#define UART_SFE_SFE(val)     (((val)&0x1) << UART_SFE_SFE_POS)
#define UART_SFE_SFE_GET(reg) (((reg) >> UART_SFE_SFE_POS) & 0x1)

/*--------- SRT ( reset value 0, offset 0x9C )  ( R/W ) Shadow RCVR Trigger -----------*/
// Range                    Name              Description
// bits [31:2]              reserved
// bits [1:0]               SRT             Duplicates FCR[7:6]          0x0
#define UART_SRT_OFFSET	    0x9C
#define UART_SRT_RESETVALUE 0
#define UART_SRT_MASK	    0x1

#define UART_SRT_SRT_POS      0
#define UART_SRT_SRT_MASK     (0x3 << UART_SRT_SRT_POS)
#define UART_SRT_SRT(val)     (((val)&0x3) << UART_SRT_SRT_POS)
#define UART_SRT_SRT_GET(reg) (((reg) >> UART_SRT_SRT_POS) & 0x3)

/*--------- STET ( reset value 0, offset 0xA0 ) ( R/W )  Shadow TX Empty Trigger ------*/
// Range                    Name              Description
// bits [31:2]              reserved
// bits [1:0]               STET          Duplicates FCR[5:4]                                  0x0
#define UART_STET_OFFSET     0xa0
#define UART_STET_RESETVALUE 0
#define UART_STET_MASK	     0x1

#define UART_STET_STET_POS	0
#define UART_STET_STET_MASK	(0x3 << UART_STET_STET_POS)
#define UART_STET_STET(val)	(((val)&0x3) << UART_STET_STET_POS)
#define UART_STET_STET_GET(reg) (((reg) >> UART_STET_STET_POS) & 0x3)

/* HTX ( reset value 0, offset 0xA4 )  ( R/W ) Halt TX ------------*/
// Range                    Name              Description
// bits [31:1]              reserved
// bits [0]                 HTX            Stop transmission                                  0x0

#define UART_HTX_OFFSET	    0xa4
#define UART_HTX_RESETVALUE 0
#define UART_HTX_MASK	    0x1

#define UART_HTX_HTX_POS      0
#define UART_HTX_HTX_MASK     (0x1 << UART_HTX_HTX_POS)
#define UART_HTX_HTX(val)     (((val)&0x1) << UART_HTX_HTX_POS)
#define UART_HTX_HTX_GET(reg) (((reg) >> UART_HTX_HTX_POS) & 0x1)

/*------- DMASA ( reset value 0, offset 0xA8 )  ( R/W )  DMA Software Acknowledge */
// Range                    Name              Description
// bits [31:1]              reserved
// bits [0]                 DMASA          DMA software alerts                          0x0
#define UART_DMASA_OFFSET     0xa8
#define UART_DMASA_RESETVALUE 0
#define UART_DMASA_MASK	      0x1

#define UART_DMASA_DMASA_POS	  0
#define UART_DMASA_DMASA_MASK	  (0x1 << UART_DMASA_DMASA_POS)
#define UART_DMASA_DMASA(val)	  (((val)&0x1) << UART_DMASA_DMASA_POS)
#define UART_DMASA_DMASA_GET(reg) (((reg) >> UART_DMASA_DMASA_POS) & 0x1)

/*---------- TCR ( reset value 0, offset 0xAC )  ( R/W ) Transmission Control Register --------*/
// Range                    Name              Description
// bits [31:5]              reserved
// bits [4:3]               XFER_MODE      Transmission mode
// bits [2]                 DE_POL         Signal polarity DE 0x0 - active high state 0x1 - active low state
// bits [1]                 RE_POL         RE signal polarity 0x0 - active high state 0x1 - active low state
// bits [0]                 RS485_EN       RS485 mode 0x0 - disabled 0x1 - enabled
#define UART_TCR_OFFSET	    0xAC
#define UART_TCR_RESETVALUE 0
#define UART_TCR_MASK	    0x1F

#define UART_TCR_XFER_MODE_POS	    3
#define UART_TCR_XFER_MODE_MASK	    (0x3 << UART_TCR_XFER_MODE_POS)
#define UART_TCR_XFER_MODE(val)	    (((val)&0x3) << UART_TCR_XFER_MODE_POS)
#define UART_TCR_XFER_MODE_GET(reg) (((reg) >> UART_TCR_XFER_MODE_POS) & 0x3)

#define UART_TCR_DE_POL_POS	 2
#define UART_TCR_DE_POL_MASK	 (0x1 << UART_TCR_DE_POL_POS)
#define UART_TCR_DE_POL(val)	 (((val)&0x1) << UART_TCR_DE_POL_POS)
#define UART_TCR_DE_POL_GET(reg) (((reg) >> UART_TCR_DE_POL_POS) & 0x1)

#define UART_TCR_RE_POL_POS	 1
#define UART_TCR_RE_POL_MASK	 (0x1 << UART_TCR_RE_POL_POS)
#define UART_TCR_RE_POL(val)	 (((val)&0x1) << UART_TCR_RE_POL_POS)
#define UART_TCR_RE_POL_GET(reg) (((reg) >> UART_TCR_RE_POL_POS) & 0x1)

#define UART_TCR_RS485_EN_POS	   0
#define UART_TCR_RS485_EN_MASK	   (0x1 << UART_TCR_RS485_EN_POS)
#define UART_TCR_RS485_EN(val)	   (((val)&0x1) << UART_TCR_RS485_EN_POS)
#define UART_TCR_RS485_EN_GET(reg) (((reg) >> UART_TCR_RS485_EN_POS) & 0x1)

/*------ DE_EN ( reset value 0, offset 0xB0 ) ( R/W ) Signal control register DE -------*/
// Range                    Name              Description
// bits [31:1]              reserved
// bits [0]                 DE_ENABLE        Signal control register DE 0x0 - reset 0x1 - set
#define UART_DE_EN_OFFSET     0xB0
#define UART_DE_EN_RESETVALUE 0
#define UART_DE_EN_MASK	      0x1

#define UART_DE_EN_DE_ENABLE_POS      0
#define UART_DE_EN_DE_ENABLE_MASK     (0x1 << UART_DE_EN_DE_ENABLE_POS)
#define UART_DE_EN_DE_ENABLE(val)     (((val)&0x1) << UART_DE_EN_DE_ENABLE_POS)
#define UART_DE_EN_DE_ENABLE_GET(reg) (((reg) >> UART_DE_EN_DE_ENABLE_POS) & 0x1)

/*------- RE_EN ( reset value 0, offset 0xB4 )  ( R/W ) Signal control register RE ------*/
// Range                    Name              Description
// bits [31:1]              reserved
// bits [0]                 RE_ENABLE    0x0 - reset 0x1 - set

#define UART_RE_EN_OFFSET     0xB4
#define UART_RE_EN_RESETVALUE 0
#define UART_RE_EN_MASK	      0x1

#define UART_RE_EN_RE_ENABLE_POS      0
#define UART_RE_EN_RE_ENABLE_MASK     (0x1 << UART_RE_EN_RE_ENABLE_POS)
#define UART_RE_EN_RE_ENABLE(val)     (((val)&0x1) << UART_RE_EN_RE_ENABLE_POS)
#define UART_RE_EN_RE_ENABLE_GET(reg) (((reg) >> UART_RE_EN_RE_ENABLE_POS) & 0x1)

/*------- DET ( reset value 0, offset 0xB8 )  ( R/W ) Timer DE --------*/
// Range                    Name              Description
// bits [31:24]             reserved
// bits [23:16]             DE_DEASSERTION_TIME            Reset timer control DE
// bits [15:8]              reserved
// bits [7:0]               DE_ASSERTION_TIME              DE setting timer control
#define UART_DET_OFFSET	    0xB8
#define UART_DET_RESETVALUE 0
#define UART_DET_MASK	    0xFF00FF

#define UART_DET_DE_DEASSERTION_TIME_POS      16
#define UART_DET_DE_DEASSERTION_TIME_MASK     (0xff << UART_DET_DE_DEASSERTION_TIME_POS)
#define UART_DET_DE_DEASSERTION_TIME(val)     (((val)&0xff) << UART_DET_DE_DEASSERTION_TIME_POS)
#define UART_DET_DE_DEASSERTION_TIME_GET(reg) (((reg) >> UART_DET_DE_DEASSERTION_TIME_POS) & 0xff)

#define UART_DET_DE_ASSERTION_TIME_POS	    0
#define UART_DET_DE_ASSERTION_TIME_MASK	    (0xff << UART_DET_DE_ASSERTION_TIME_POS)
#define UART_DET_DE_ASSERTION_TIME(val)	    (((val)&0xff) << UART_DET_DE_ASSERTION_TIME_POS)
#define UART_DET_DE_ASSERTION_TIME_GET(reg) (((reg) >> UART_DET_DE_ASSERTION_TIME_POS) & 0xff)

/*------- TAT ( reset value 0, offset 0xBC )  ( R/W )  turn around time -------*/
// Range                    Name              Description
// bits [31:16]             RE_TO_DE          Delay for switching from RE to DE
// bits [15:0]              DE_TO_RE          Delay for switching from RE to DE
#define UART_TAT_OFFSET	    0xBC
#define UART_TAT_RESETVALUE 0
#define UART_TAT_MASK	    0xFFFFFFFF

#define UART_TAT_RE_TO_DE_POS	   16
#define UART_TAT_RE_TO_DE_MASK	   (0xffff << UART_TAT_RE_TO_DE_POS)
#define UART_TAT_RE_TO_DE(val)	   (((val)&0xffff) << UART_TAT_RE_TO_DE_POS)
#define UART_TAT_RE_TO_DE_GET(reg) (((reg) >> UART_TAT_RE_TO_DE_POS) & 0xffff)

#define UART_TAT_DE_TO_RE_POS	   0
#define UART_TAT_DE_TO_RE_MASK	   (0xffff << UART_TAT_DE_TO_RE_POS)
#define UART_TAT_DE_TO_RE(val)	   (((val)&0xffff) << UART_TAT_DE_TO_RE_POS)
#define UART_TAT_DE_TO_RE_GET(reg) (((reg) >> UART_TAT_DE_TO_RE_POS) & 0xffff)

/*------- DLF ( reset value 0, offset 0xC0 )  ( R/W )  Fractional divider register ------*/
// Range                    Name              Description
// bits [31:4]              reserved
// bits [3:0]               DLF               Fractional part of the frequency divider. Defined as DLF * (2 ^ DLF_SIZE)
#define UART_DLF_OFFSET	    0xC0
#define UART_DLF_RESETVALUE 0
#define UART_DLF_MASK	    0xF

#define UART_DLF_DLF_POS      0
#define UART_DLF_DLF_MASK     (0xf << UART_DLF_DLF_POS)
#define UART_DLF_DLF(val)     (((val)&0xf) << UART_DLF_DLF_POS)
#define UART_DLF_DLF_GET(reg) (((reg) >> UART_DLF_DLF_POS) & 0xf)

/*-------- CPR ( reset value 0, offset 0xF4 )  ( Read only ) Component Parameter Register ---------*/
// bits [31:24]             reserved
// bits [23:16]             FIFO_MODE                   Encoding of FIFO_MODE configuration parameter value.
// bits [15:14]             reserved
// bits [13]                DMA_EXTRA                   Encoding of DMA_EXTRA configuration parameter value.
// bits [12]                UART_ADD_ENCODED_PARAMS     Encoding of UART_ADD_ENCODED_PARAMS configuration parameter value.
// bits [11]                SHADOW                      Encoding of SHADOW configuration parameter value.
// bits [10]                FIFO_STAT                   Encoding of FIFO_STAT configuration parameter value.
// bits [9]                 FIFO_ACCESS                 Encoding of FIFO_ACCESS configuration parameter value.
// bits [8]                 ADDITIONAL_FEAT             Encoding of ADDITIONAL_FEATURES configuration parameter value.
// bits [7]                 SIR_LP_MODE                 Encoding of SIR_LP_MODE configuration parameter value.
// bits [6]                 SIR_MODE                    Encoding of SIR_MODE configuration parameter value.
// bits [5]                 THRE_MODE                   Encoding of THRE_MODE configuration parameter value.
// bits [4]                 AFCE_MODE                   Encoding of AFCE_MODE configuration parameter value.
// bits [3:2]               reserved
// bits [1:0]               APB_DATA_WIDTH              Encoding of APB_DATA_WIDTH configuration parameter value.

#define UART_CPR_OFFSET	    0xF4
#define UART_CPR_RESETVALUE 0xFFFF3FF3
#define UART_CPR_MASK

#define UART_CPR_FIFO_MODE_POS	    16
#define UART_CPR_FIFO_MODE_MASK	    (0xff << UART_CPR_FIFO_MODE_POS)
#define UART_CPR_FIFO_MODE(val)	    (((val)&0xff) << UART_CPR_FIFO_MODE_POS)
#define UART_CPR_FIFO_MODE_GET(reg) (((reg) >> UART_CPR_FIFO_MODE_POS) & 0xff)

#define UART_CPR_RESERVED_POS	   14
#define UART_CPR_RESERVED_MASK	   (0x3 << UART_CPR_RESERVED_POS)
#define UART_CPR_RESERVED(val)	   (((val)&0x3) << UART_CPR_RESERVED_POS)
#define UART_CPR_RESERVED_GET(reg) (((reg) >> UART_CPR_RESERVED_POS) & 0x3)

#define UART_CPR_DMA_EXTRA_POS	    13
#define UART_CPR_DMA_EXTRA_MASK	    (0x1 << UART_CPR_DMA_EXTRA_POS)
#define UART_CPR_DMA_EXTRA(val)	    (((val)&0x1) << UART_CPR_DMA_EXTRA_POS)
#define UART_CPR_DMA_EXTRA_GET(reg) (((reg) >> UART_CPR_DMA_EXTRA_POS) & 0x1)

#define UART_CPR_UART_ADD_ENCODED_PARAMS_POS  12
#define UART_CPR_UART_ADD_ENCODED_PARAMS_MASK (0x1 << UART_CPR_UART_ADD_ENCODED_PARAMS_POS)
#define UART_CPR_UART_ADD_ENCODED_PARAMS(val) (((val)&0x1) << UART_CPR_UART_ADD_ENCODED_PARAMS_POS)
#define UART_CPR_UART_ADD_ENCODED_PARAMS_GET(reg) \
	(((reg) >> UART_CPR_UART_ADD_ENCODED_PARAMS_POS) & 0x1)

#define UART_CPR_SHADOW_POS	 11
#define UART_CPR_SHADOW_MASK	 (0x1 << UART_CPR_SHADOW_POS)
#define UART_CPR_SHADOW(val)	 (((val)&0x1) << UART_CPR_SHADOW_POS)
#define UART_CPR_SHADOW_GET(reg) (((reg) >> UART_CPR_SHADOW_POS) & 0x1)

#define UART_CPR_FIFO_STAT_POS	    10
#define UART_CPR_FIFO_STAT_MASK	    (0x1 << UART_CPR_FIFO_STAT_POS)
#define UART_CPR_FIFO_STAT(val)	    (((val)&0x1) << UART_CPR_FIFO_STAT_POS)
#define UART_CPR_FIFO_STAT_GET(reg) (((reg) >> UART_CPR_FIFO_STAT_POS) & 0x1)

#define UART_CPR_FIFO_ACCESS_POS      9
#define UART_CPR_FIFO_ACCESS_MASK     (0x1 << UART_CPR_FIFO_ACCESS_POS)
#define UART_CPR_FIFO_ACCESS(val)     (((val)&0x1) << UART_CPR_FIFO_ACCESS_POS)
#define UART_CPR_FIFO_ACCESS_GET(reg) (((reg) >> UART_CPR_FIFO_ACCESS_POS) & 0x1)

#define UART_CPR_ADDITIONAL_FEAT_POS	  8
#define UART_CPR_ADDITIONAL_FEAT_MASK	  (0x1 << UART_CPR_ADDITIONAL_FEAT_POS)
#define UART_CPR_ADDITIONAL_FEAT(val)	  (((val)&0x1) << UART_CPR_ADDITIONAL_FEAT_POS)
#define UART_CPR_ADDITIONAL_FEAT_GET(reg) (((reg) >> UART_CPR_ADDITIONAL_FEAT_POS) & 0x1)

#define UART_CPR_SIR_LP_MODE_POS      7
#define UART_CPR_SIR_LP_MODE_MASK     (0x1 << UART_CPR_SIR_LP_MODE_POS)
#define UART_CPR_SIR_LP_MODE(val)     (((val)&0x1) << UART_CPR_SIR_LP_MODE_POS)
#define UART_CPR_SIR_LP_MODE_GET(reg) (((reg) >> UART_CPR_SIR_LP_MODE_POS) & 0x1)

#define UART_CPR_SIR_MODE_POS	   6
#define UART_CPR_SIR_MODE_MASK	   (0x1 << UART_CPR_SIR_MODE_POS)
#define UART_CPR_SIR_MODE(val)	   (((val)&0x1) << UART_CPR_SIR_MODE_POS)
#define UART_CPR_SIR_MODE_GET(reg) (((reg) >> UART_CPR_SIR_MODE_POS) & 0x1)

#define UART_CPR_THRE_MODE_POS	    5
#define UART_CPR_THRE_MODE_MASK	    (0x1 << UART_CPR_THRE_MODE_POS)
#define UART_CPR_THRE_MODE(val)	    (((val)&0x1) << UART_CPR_THRE_MODE_POS)
#define UART_CPR_THRE_MODE_GET(reg) (((reg) >> UART_CPR_THRE_MODE_POS) & 0x1)

#define UART_CPR_AFCE_MODE_POS	    4
#define UART_CPR_AFCE_MODE_MASK	    (0x1 << UART_CPR_AFCE_MODE_POS)
#define UART_CPR_AFCE_MODE(val)	    (((val)&0x1) << UART_CPR_AFCE_MODE_POS)
#define UART_CPR_AFCE_MODE_GET(reg) (((reg) >> UART_CPR_AFCE_MODE_POS) & 0x1)

#define UART_CPR_APB_DATA_WIDTH_POS	 0
#define UART_CPR_APB_DATA_WIDTH_MASK	 (0x3 << UART_CPR_APB_DATA_WIDTH_POS)
#define UART_CPR_APB_DATA_WIDTH(val)	 (((val)&0x3) << UART_CPR_APB_DATA_WIDTH_POS)
#define UART_CPR_APB_DATA_WIDTH_GET(reg) (((reg) >> UART_CPR_APB_DATA_WIDTH_POS) & 0x3)

/*------- UCV ( reset value = 0 , offset 0xF8 )  ( Read only ) Component Version -------*/
#define UART_UCV_OFFSET	    0xF8
#define UART_UCV_RESETVALUE 0
#define UART_UCV_MASK	    0xFFFFFFFF
// bits [31:0]              UART_COMPONENT_VERSION

// ASCII string. Each version number is followed by a sign *. For example: 32_30_31_2A presented in the form. 2.01*.

/*------- CTR ( reset value = 0 , offset 0xF  ) ( Read only ) Component Type Register -------*/
#define UART_CTR_OFFSET	    0xFC
#define UART_CTR_RESETVALUE 0
#define UART_CTR_MASK	    0xFFFFFFFF
// bits [31:0]              PERIPHERAL_ID       Peripheral ID

// Line control register
#define MC_LCR_5BITS 0x00 /* character length: 5 bits */
#define MC_LCR_6BITS 0x01 /* character length: 6 bits */
#define MC_LCR_7BITS 0x02 /* character length: 7 bits */
#define MC_LCR_8BITS 0x03 /* character length: 8 bits */

#define MC_LCR_STOPB  0x04 /* use 2 stop bits */
#define MC_LCR_PENAB  0x08 /* parity enable */
#define MC_LCR_PEVEN  0x10 /* even parity */
#define MC_LCR_PFORCE 0x20 /* force parity */
#define MC_LCR_SBREAK 0x40 /* break control */
#define MC_LCR_DLAB   0x80 /* divisor latch access bit */

// Line status register
#define MC_LSR_RXRDY 0x01 /* receiver ready */
#define MC_LSR_OE    0x02 /* overrun error */
#define MC_LSR_PE    0x04 /* parity error */
#define MC_LSR_FE    0x08 /* framing error */
#define MC_LSR_BI    0x10 /* break interrupt */
#define MC_LSR_TXRDY 0x20 /* transmitter holding register empty */
#define MC_LSR_TEMT  0x40 /* transmitter empty */
#define MC_LSR_FIFOE 0x80 /* error in receive FIFO */

// FIFO control register
#define MC_FCR_ENABLE  0x01 /* enable FIFO */
#define MC_FCR_RCV_RST 0x02 /* clear receive FIFO */
#define MC_FCR_XMT_RST 0x04 /* clear transmit FIFO */

#define MC_FCR_TRIGGER_1  0x00 /* receive FIFO level: 1/4 byte */
#define MC_FCR_TRIGGER_4  0x40 /* receive FIFO level: 4/16 bytes */
#define MC_FCR_TRIGGER_8  0x80 /* receive FIFO level: 8/56 bytes */
#define MC_FCR_TRIGGER_14 0xc0 /* receive FIFO level: 14/60 bytes */

// Interrupt enable register
#define MC_IER_ERXRDY 0x01 /* enable receive data/timeout intr */
#define MC_IER_ETXRDY 0x02 /* enable transmitter interrupts */
#define MC_IER_ERLS   0x04 /* enable receive line status intr */
#define MC_IER_EMSC   0x08 /* enable modem status interrupts */

// Interrupt identification register
#define MC_IIR_NOPEND 0x01 /* no interrupt pending */
#define MC_IIR_IMASK  0x0e /* interrupt type mask */
#define MC_IIR_FENAB  0xc0 /* set if FIFOs are enabled */

#define MC_IIR_RLS    0x06 /* receiver line status */
#define MC_IIR_RXRDY  0x04 /* receiver data available */
#define MC_IIR_RXTOUT 0x0c /* character timeout indication */
#define MC_IIR_TXRDY  0x02 /* transmitter holding reg empty */
#define MC_IIR_MLSC   0x00 /* modem status */

// Modem control register
#define MC_MCR_DTR	0x01 /* control DTR output */
#define MC_MCR_RTS	0x02 /* control RTS output */
#define MC_MCR_OUT1	0x04 /* control OUT1 output */
#define MC_MCR_OUT2	0x08 /* control OUT2 output, used as global interrupt enable in PCs */
#define MC_MCR_LOOPBACK 0x10 /* set local loopback mode */

// Modem status register
#define MC_MSR_DCTS 0x01 /* CTS changed */
#define MC_MSR_DDSR 0x02 /* DSR changed */
#define MC_MSR_TERI 0x04 /* RI changed from 0 to 1 */
#define MC_MSR_DDCD 0x08 /* DCD changed */
#define MC_MSR_CTS  0x10 /* CTS input */
#define MC_MSR_DSR  0x20 /* DSR input */
#define MC_MSR_RI   0x40 /* RI input */
#define MC_MSR_DCD  0x80 /* DCD input */

#define LINE_FEED    0xA
#define CARET_RETURN 0xD

// ERRORS
#define OK		      0
#define ERROR_BUSY	      1
#define ERROR_NO_DATA	      4
#define ERROR_BUFFER_OVERFLOW 0x20
#define ERROR_DEVICE_NUMBER   0x40
#define UART_ERROR_RECIEVER   0x50

// mode
#define BLOCKING      1
#define NON_BLOCKING  0
#define HARD_FIFO_ON  0x10
#define HARD_FIFO_OFF 0
#define INTERRUPT     2
#define RING_BUFF     4
#define DMA	      8

#define UART_PORT_COUNT		4 //Max active count of uart ports
#define UART_HARD_FIFO_LENGTH	128 /* length hard fifo */
#define UART_SOFT_BUFFER_LENGTH 512 /* soft buffer length */
//#define UART_SOFT_BUFFER_LENGTH         4096
#define UART_FIFO_LENGTH 16
#define UART_CARET_FIX	 1 //Generate '\r' after '\n'

//Interrupts IDs  register IIR. Value of register
#define UART_INT_MODEM_STATUS	      (0) /* Modem status interrupt */
#define UART_INT_THR_EMPTY	      (1) /* THRE interrupt (THRE Transmitter holding register empty)   */
#define UART_INT_DATA_RECIEVE	      (2) /* Received Data Available Interrupt */
#define UART_INT_RECEIVER_LINE_STATUS (3) /* receiver line status   */
#define UART_INT_CHAR_TIMEOUT	      (6) /* timeout waiting receiver   */

#define UART_TIMEOUT		 10
#define UART_TRANSMIT_BUSY	 11
#define UART_DATA_NOT_READY	 12
#define UART_PARITY_ERROR	 13
#define UART_FRAME_ERROR	 14
#define UART_BREAK_ERROR	 15
#define UART_HARD_FIFO_NOT_EMPTY 16
#define UART_NOT_BUSY		 17

#define UART_BUFFER_FULL	1U
#define UART_BUFFER_OVERFLOW	2U
#define UART_WRONG_FUNC_POINTER 3U
#define UART_WRONG_DEV_NUMBER	4U
#define BUFFER_LOCK		5U

// clang-format off
typedef struct
{
    volatile uint32_t RBR_THR_DLL;    /**< \brief offset 0 ( R/W 32 ) receiver buffer register, transmite data register, lower register of divider */
    volatile uint32_t DLH_IER;        /**< \brief offset 0x4 ( R/W 32 ) high divider register/ interrupt enable register */
    union
    {
        volatile uint32_t IIR;        //  interrupr identification register read only
        volatile uint32_t FCR;        //  Fifo control register write only
    };                                /**< \brief offset 0x8  ( R/W 32 )  */
    volatile uint32_t LCR;                     /**< \brief offset 0xC  ( R/W 32 ) line control register */
    volatile uint32_t MCR;            /**< \brief offset 0x10 ( R/W 32 )  modem control register */
    volatile uint32_t LSR;            /**< \brief offset 0x14 ( R/W 32 )  line state register */
    volatile uint32_t MSR;            /**< \brief offset 0x18 ( R/W 32 )  modem state register  */
    volatile uint32_t SCR;            /**< \brief offset 0x1c ( R/W 32 )  general register  */
    volatile uint32_t reserved0[4];
    volatile uint32_t SRBR[16];       /**< \brief offset 0x30 ( R/W 32 )  receiver buffer register  */
    volatile uint32_t FAR;            /**< \brief offset 0x70 ( R/W 32 )  Fifo access register  */
    volatile uint32_t TFR;            /**< \brief offset 0x74 ( R/W 32 )  Transmitte  fifo register */
    volatile uint32_t RFW;            /**< \brief offset 0x78 ( R/W 32 )  receiver fifo write register */
    volatile uint32_t USR;            /**< \brief offset 0x7c ( R/W 32 )  uart status register  */
    volatile uint32_t TFL;            /**< \brief offset 0x80 ( R/W 32 )  Transmitter fifo level register */
    volatile uint32_t RFL;            /**< \brief offset 0x84 ( R/W 32 )  receiver fifo level register  */
    volatile uint32_t SRR;            /**< \brief offset 0x88 ( R/W 32 )  software reset register*/
    volatile uint32_t SRTS;           /**< \brief offset 0x8c ( R/W 32 )  Supplementary request transfer register */
    volatile uint32_t SBCR;           /**< \brief offset 0x90 ( R/W 32 )  Supplementary block control register*/
    volatile uint32_t SDMAM;          /**< \brief offset 0x94 ( R/W 32 )  Supplementary DMA mode register*/
    volatile uint32_t SFE;            /**< \brief offset 0x98 ( R/W 32 )  Supplementary FIFO enable register*/
    volatile uint32_t SRT;            /**< \brief offset 0x9c ( R/W 32 )  Supplementary receiver interrupt trigger register*/
    volatile uint32_t STET;           /**< \brief offset 0xa0( R/W 32 )  Supplementary transmitter interrupt trigger register*/
    volatile uint32_t HTX;            /**< \brief offset 0xa4 ( R/W 32 )  Halt transmite register */
    volatile uint32_t DMASA;          /**< \brief offset 0xa8 ( R/W 32 )  DMA software announcementy register */
    volatile uint32_t TCR;            /**< \brief offset 0xac ( R/W 32 )  Transmitte control regster */
    volatile uint32_t DE_EN;          /**< \brief offset 0xb0 ( R/W 32 )  DE enabele register */
    volatile uint32_t RE_EN;          /**< \brief offset 0xb4 ( R/W 32 )  RE enable register */
    volatile uint32_t DET;            /**< \brief offset 0xb8 ( R/W 32 )  DE enable register */
    volatile uint32_t TAT;            /**< \brief offset 0xbc ( R/W 32 )  Timer register */
    volatile uint32_t DLF;            /**< \brief offset 0xc0 ( R/W 32 )  Frequency divider fractional register*/
    volatile uint32_t reservrd1[12];
    volatile uint32_t CPR;            /**< \brief offset 0xf4 ( R/W 32 )  configuration parameter register */
    volatile uint32_t UCV;            /**< \brief offset 0xf8 ( R/W 32 )  UART component register */
    volatile uint32_t CTR;            /**< \brief offset 0xfc ( R/W 32 )  component  register */
} uart_reg_t;
// clang-format on

typedef enum {
	UART_5BIT = 0, /**< \brief  character length 5 bit*/
	UART_6BIT = 1, /**< \brief  character length 6 bit*/
	UART_7BIT = 2, /**< \brief  character length 7 bit*/
	UART_8BIT = 3 /**< \brief  character length 8 bit*/
} uart_data_len_t;

typedef enum {
	UART_STOPBIT1 = 0, /**< \brief one stop bit */
	UART_STOPBIT2 = 1, /**< \brief two stop bits */
} uart_stop_bit;

typedef enum {
	UART_NOPARITY = 0, /**< \brief  No parity control flow*/
	UART_ODDPARITY = 1, /**< \brief  The odd parity control */
	UART_EVENPARITY = 2 /**< \brief  The even parity control*/
} uart_parity_t;

typedef struct {
	unsigned int read_position;
	unsigned int write_position;
	int overflow;
	int lock;
	unsigned char buffer[UART_SOFT_BUFFER_LENGTH];
} uart_buffer_t;

typedef struct {
	uart_reg_t *uart_ptr; /**< \brief The pointer for UART controller registers*/
	uint32_t uartNum; /**< \brief The number of UART in system*/
	uint32_t baudrate; /**< \brief The transmission speed*/
	uart_data_len_t bits; /**< \brief The data length*/
	uart_stop_bit stopBit; /**< \brief The number stop bits*/
	uart_parity_t parity; /**< \brief The parity control */
	uint32_t mode; /**< \brief The mode of work functions uart deriver*/
	int linesplit; /**< \brief The mode splite lines text flow*/
	uint32_t timeout; /**< \brief The timeout cycles*/
	uint32_t error_receive; /**< \brief Errors of reciever */
} uart_param_t;

/**
 * @brief The function return pointer into uart controller
 *
 * @param uartNum Number of uart controller
 * @param uart_ptr The pointer to registers UART controller
 * @return mcom_err_t Error code
 */
mcom_err_t uart_drv_get_handler(uint32_t uartNum, uart_reg_t **uart_ptr);

/*!
 * \brief The function set UART controller configuration
 * \param uart The pointer to structure uart_param_t. Uart_context describes of parametrs uart controller
 * \return mcom_err_t Error code
 */
mcom_err_t uart_drv_config(uart_param_t *uart);

/*!
 * \brief The function set default configuration of UART controller
 * \param huart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
 * \return mcom_err_t Error code
 */
mcom_err_t uart_drv_config_default(uart_param_t *huart);

/*!
 * \brief The function disable transfering, receiving, interrupts
 * \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
 * \return mcom_err_t Error code
 */
mcom_err_t uart_drv_deinit(uart_param_t *uart);

/*!
* \brief The function get char from rx buffer UART controller. This function receive text strings.
* \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
* \param c Pointer for reading from rx buffer
* \return mcom_err_t Error code
*/
mcom_err_t uart_drv_getchar(uart_param_t *uart, int *c);

/*!
* \brief The function put char direct to transmit register UART controller
* \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
* \param c char for transmit
* \return mcom_err_t Error code
*/
mcom_err_t uart_drv_putchar(uart_param_t *uart, char c);

/*!
* \brief The function used to flush the tx buffer immediately
* \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
* \return mcom_err_t Error code
*/
mcom_err_t uart_drv_flush(uart_param_t *uart);

#endif /* UART_H */
