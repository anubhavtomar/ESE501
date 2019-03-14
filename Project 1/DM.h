/**
 * Author:    Anubhav Tomar
 * 
 * Data Memory Definition
 **/

#ifndef DM_H
#define DM_H

#include<systemc.h>

template <class _dataSize , unsigned N>
class _dataMemory : public sc_module {
	public:
		sc_in<bool> _clock;
		sc_in<_addrSize > _addr1;
		sc_in<sc_uint<1> > _dmRead;
		sc_in<sc_uint<1> > _dmWrite;
		sc_out<_dataSize> _dataOut;

		SC_HAS_PROCESS(DataMemory);

		DataMemory(SC_MODULE_NAME name : sc_module(name)) {
			SC_METHOD(_read);
			sensitive<<_dmRead.pos();

			SC_METHOD(_write);
			sensitive<<_dmWrite.pos();
		};

	private:
		_dataSize _memory[N];

		void _read() {
			_dataOut.write(_registers[_addr1]) ;
		};

		void _write() {
			_immRegisters[_addr1] = _data1.read();
		};
};

#endif