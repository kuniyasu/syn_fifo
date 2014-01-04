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

	SC_HAS_PROCESS(TOP);

	TOP(const sc_module_name name):sc_module(name){
		end_module();
	}
};


#endif /* TOP_H_ */
