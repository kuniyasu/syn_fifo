/*
 * TOP.h
 *
 *  Created on: 2014/01/04
 *      Author: Kuniyasu
 */

#ifndef TOP_H_
#define TOP_H_

#include <systemc.h>
#include "syn_fifo.h"

class TOP:public sc_module{
public:
	sc_clock clk;
	sc_signal<bool> nrst;

	SC_HAS_PROCESS(TOP);

	TOP(const sc_module_name name):sc_module(name),clk("CLK", 25, SC_NS, 0.5, 5, SC_NS, true ),nrst("nrst"){


		SC_THREAD(timeout_thread);
		end_module();
	}

	void timeout_thread(){
		nrst = false;
		wait(100,SC_NS);

		nrst = true;
		wait(10,SC_MS);

		sc_stop();
	}
};


#endif /* TOP_H_ */
