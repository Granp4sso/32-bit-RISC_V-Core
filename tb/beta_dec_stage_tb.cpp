//*** Verilator testbench template ***

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vbeta_dec_stage.h"
#include "verilated.h"

#include "verilated_vcd_c.h"

#define CLK_NS 10
#define CYCLES 700

void tick(int tickcount, Vbeta_dec_stage *tb, VerilatedVcdC *tfp);
void trace_init(Vbeta_dec_stage *tb, VerilatedVcdC * tfp);

int main(int argc, char **argv){
	
	Vbeta_dec_stage * tb = new Vbeta_dec_stage;
	VerilatedVcdC * tfp = new VerilatedVcdC;

	unsigned tickcount = 0;
	
	//START: Generate a trace
	trace_init(tb,tfp);
	//END: Generate a trace

	//START: Logic
	
	//Init core

	for(int i = 0; i < CYCLES; i++){
	
		(i == 0) ? tb->rstn_i = 0 : tb->rstn_i = 1; //N Reset for first clk cycle
		if(i == 1) tb->instr_i = 0x00500313; //ADDI t1,zero,5
		else if(i == 2) tb->instr_i = 0x006e2023; //SW t1,t3,result
		else if(i == 3) tb->instr_i = 0x01d37e33; //AND t3,t1,t4
		else if(i == 4) tb->instr_i = 0x00000863; //BEQ zero,zero,main (80 - 90)
		else if(i == 5) tb->instr_i = 0x0000006f; //JAL zero,0

		tb->rd_wdata_i = 0;

		tick(++tickcount,tb,tfp);
	}

	//END: Logic		
	
	delete tb;
	delete tfp;

}

void tick(int tickcount, Vbeta_dec_stage *tb, VerilatedVcdC *tfp){
	tb->eval();
	if(tfp) //dump 2ns before the tick
		tfp->dump(tickcount*CLK_NS - 2);
	tb->clk_i = 1;
	tb->eval();
	if(tfp) //Tick every CLK_NS
		tfp->dump(tickcount*CLK_NS);
	tb->clk_i = 0;
	tb->eval();
	if(tfp){ //Trailing edge dump
		tfp->dump(tickcount*CLK_NS+5);
		tfp->flush();
	}
}

void trace_init(Vbeta_dec_stage *tb, VerilatedVcdC * tfp){

	Verilated::traceEverOn(true);
	
	tb->trace(tfp,99);
	tfp->open("trace.vcd");
}

