/**
 * Author:    Anubhav Tomar
 * 
 * Modules Definition
 **/

#ifndef MODULES_H
#define MODULES_H

#include<systemc.h>
#include<RF.h> //Register File
#include<DM.h> //Data Memory
#include<PM.h> //Program Memory
#include<ALU.h> 

template <class T , unsigned N>
class _modulesTop : public sc_module {
	public:
		sc_in<bool> clock;
		_registerFile<sc_uint<4> , sc_uint<16> , 256> _rF("Register File");
		_dataMemory<sc_uint<16> , 256> _dM("Date Memory");
		_programMemory<sc_uint<4> , 256> _pM("Program Memory");
		_aluBlock<sc_uint<8> , sc_uint<16> > _alu("ALU Block");

		SC_HAS_PROCESS(ModulesTop);

		ModulesTop(SC_MODULE_NAME name : sc_module(name)) { };
};

#endif