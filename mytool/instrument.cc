/** File:       instrument.cc
 ** Author:     Dongli Zhang
 ** Contact:    dongli.zhang0129@gmail.com
 **
 ** Copyright (C) Dongli Zhang 2013
 **
 ** This program is free software;  you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY;  without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 ** the GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program;  if not, write to the Free Software 
 ** Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "bochs.h"

#define USE_FILE	// Comment this line if you want to use stdio (not file)
#define USE_PHYS	// Comment this line if you want to get both linear and physical address

static int log_flag;                    // flag - whether logging is on/off
FILE       *fp;
char       *file = "/home/zhang/mylog"; // path to output log

#if BX_INSTRUMENTATION

void bx_instr_init_env(void) 
{
	// initialize flag to zero
	log_flag = 0;
}
void bx_instr_exit_env(void) {}

void bx_instr_initialize(unsigned cpu) {}
void bx_instr_exit(unsigned cpu) {}
void bx_instr_reset(unsigned cpu, unsigned type) {}
void bx_instr_hlt(unsigned cpu) {}
void bx_instr_mwait(unsigned cpu, bx_phy_address addr, unsigned len, Bit32u flags) {}

void bx_instr_debug_promt() {}
void bx_instr_debug_cmd(const char *cmd) 
{
	if(strcmp(cmd, "start") == 0) // start logging
	{
		log_flag = 1;
#ifdef USE_FILE
		// if you want to output to file
		fp = fopen(file, "a");
		if(fp == NULL)
		{
			printf("Cannot open the log file\n");
			log_flag = 0;
		}
#endif
	}
	if(strcmp(cmd, "stop") == 0) // stop logging
	{
		log_flag = 0;
#ifdef USE_FILE
		// if you want to output to file
		fclose(fp);
#endif
	}
}

void bx_instr_cnear_branch_taken(unsigned cpu, bx_address branch_eip, bx_address new_eip) {}
void bx_instr_cnear_branch_not_taken(unsigned cpu, bx_address branch_eip) {}
void bx_instr_ucnear_branch(unsigned cpu, unsigned what, bx_address branch_eip, bx_address new_eip) {}
void bx_instr_far_branch(unsigned cpu, unsigned what, Bit16u new_cs, bx_address new_eip) {}

 void bx_instr_opcode(unsigned int cop, bxInstruction_c *i, unsigned char const *opcode, unsigned int len, unsigned int is32, unsigned int is64) {}

void bx_instr_interrupt(unsigned cpu, unsigned vector) {}
void bx_instr_exception(unsigned cpu, unsigned vector, unsigned error_code) {}
void bx_instr_hwinterrupt(unsigned cpu, unsigned vector, Bit16u cs, bx_address eip) {}

void bx_instr_tlb_cntrl(unsigned cpu, unsigned what, bx_phy_address new_cr3) {}
void bx_instr_clflush(unsigned cpu, bx_address laddr, bx_phy_address paddr) {}
void bx_instr_cache_cntrl(unsigned cpu, unsigned what) {}
void bx_instr_prefetch_hint(unsigned cpu, unsigned what, unsigned seg, bx_address offset) {}

void bx_instr_before_execution(unsigned cpu, bxInstruction_c *i) {}
void bx_instr_after_execution(unsigned cpu, bxInstruction_c *i) {}
void bx_instr_repeat_iteration(unsigned cpu, bxInstruction_c *i) {}

void bx_instr_inp(Bit16u addr, unsigned len) {}
void bx_instr_inp2(Bit16u addr, unsigned len, unsigned val) {}
void bx_instr_outp(Bit16u addr, unsigned len, unsigned val) {}

// log both linear and physical address
void bx_instr_lin_access(unsigned cpu, bx_address lin, bx_address phy, unsigned len, unsigned rw) 
{
#ifdef USE_PHYS
	return;
#endif

	if(log_flag == 1)
	{
#ifndef USE_FILE
		//first col is linear, second col is phys addr
		printf("0x%x, 0x%x, %u\n", lin, phy, len);
#else
		fprintf(fp, "0x%x, 0x%x, %u\n", lin, phy, len);	
#endif
	}
}

// log physical address
void bx_instr_phy_access(unsigned cpu, bx_address phy, unsigned len, unsigned rw) 
{
#ifndef USE_PHYS
	return;
#endif

	if(log_flag == 1)
	{
#ifndef USE_FILE
		printf("0x%x, %u\n", phy, len);
#else
		fprintf(fp, "0x%x, %u\n", phy, len);	
#endif
	}
}

void bx_instr_wrmsr(unsigned cpu, unsigned addr, Bit64u value) {}

#endif
