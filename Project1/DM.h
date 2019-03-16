/**
 * Author:    Anubhav Tomar
 * 
 * Data Memory Definition
 **/

#ifndef DM_H
#define DM_H

#include<systemc.h>

template <class _addrSize , class _dataSize , unsigned _dmCount>
class _dataMemory : public sc_module {
	public:
		// Inputs
		sc_in<bool> _clock;
		sc_in<_addrSize> _addr;
		sc_in<_dataSize> _dataIn;
		sc_in<bool> _dmRead;
		sc_in<bool> _dmWrite;

		// Outputs
		sc_out<_dataSize> _dataOut;

		SC_HAS_PROCESS(_dataMemory);

		_dataMemory(sc_module_name name) : sc_module(name) {
			SC_METHOD(_read);
			dont_initialize();
			sensitive<<_dmRead.pos();

			SC_METHOD(_write);
			dont_initialize();
			sensitive<<_dmWrite.pos();
		};

		void _read () {
			_dataOut.write(_memory[_addr]) ;
		};

		void _write () {
			_memory[_addr] = _dataIn.read();
		};

	private:
		_dataSize _memory[_dmCount];
};

#endif