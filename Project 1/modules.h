/*
	Modules Definition

	Submitted By: Anubhav Tomar
*/

#ifndef MODULES_H
#define MODULES_H

#include<systemc.h>

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

template <class _addrSize , class _dataSize , unsigned N>
class _registerFile : public sc_module {
	public:
		sc_in<bool> clock;
		sc_in<_addrSize > _addr1;
		sc_in<_addrSize > _addr2;
		sc_in<_addrSize > _addr3;
		sc_in<_dataSize > _data1;
		sc_in<sc_uint<1> > _rfRead;
		sc_in<sc_uint<1> > _rfWrite;
		sc_in<sc_uint<1> > _isImmData;
		sc_out<_dataSize > _data1;
		sc_out<_dataSize > _data2;

		SC_HAS_PROCESS(RegisterFile);

		RegisterFile(SC_MODULE_NAME name : sc_module(name)) { };

	private:
		_dataSize _registers[N];
		_dataSize _immRegisters[N];
		void _read() { };
		void _write() { };
};

template <class _dataSize , unsigned N>
class _dataMemory : public sc_module {
	public:
		sc_in<bool> _clock;
		sc_out<_dataSize> _data;

		SC_HAS_PROCESS(DataMemory);

		DataMemory(SC_MODULE_NAME name : sc_module(name)) { };

	private:
		_dataSize _memory[N];
		void _read() { };
		void _write() { };
};

template <class _addrSize , unsigned N>
class _programMemory : public sc_module {
	public:
		sc_in<bool> _clock;
		sc_in<_addrSize > _addr;
		sc_in<sc_uint<1> > _pmRead;
		sc_out<T> _insruction;

		SC_HAS_PROCESS(ProgramMemory);

		ProgramMemory(SC_MODULE_NAME name : sc_module(name)) { };

	private:
		_addrSize _programAddr[N];
		void _read() { };
};

template <class _statusRegSize , class _dataSize> 
class _aluBlock : public sc_module {
	public:
		sc_in<bool> _clock;
		sc_in<sc_uint<5> > _aluControlSig;
		sc_in<sc_uint<1> > _rfRead;
		sc_in<sc_uint<1> > _rfWrite;
		sc_in<sc_uint<1> > _pcWrite;
		sc_in<sc_uint<1> > _operand1;
		sc_in<sc_uint<1> > _operand2;
		sc_out<_dataSize> _result;
		sc_out<_statusRegSize> _PSROut;

		SC_HAS_PROCESS(AluBlock);

		AluBlock(SC_MODULE_NAME name : sc_module(name)) { };

	private:
		sc_uint<_statusRegSize> _PSR;
		void executions() { };
};

#endif