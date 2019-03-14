/**
 * Author:    Anubhav Tomar
 * 
 * Program Memory Definition
 **/

#ifndef PM_H
#define PM_H

#include<systemc.h>

template <class _addrSize , unsigned N>
class _programMemory : public sc_module {
	public:
		sc_in<bool> _clock;
		sc_in<_addrSize > _addr;
		sc_in<sc_uint<1> > _pmRead;
		sc_out<T> _insruction;

		SC_HAS_PROCESS(ProgramMemory);

		ProgramMemory(SC_MODULE_NAME name : sc_module(name)) { 
			SC_METHOD(_read);
			sensitive<<_pmRead.pos();
		};

	private:
		_addrSize _programAddr[N];

		void _read() { 
			
		};
};

#endif