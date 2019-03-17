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
		sc_in<_addrSize> _addr;
		sc_in<_dataSize> _dataIn;
		sc_in<bool> _dmRead;
		sc_in<bool> _dmWrite;

		// Outputs
		sc_out<_dataSize> _dataOut;

		SC_HAS_PROCESS(_dataMemory);

		_dataMemory(sc_module_name name) : sc_module(name) {
			SC_METHOD(_read);
			sensitive<<_dmRead.pos();

			SC_METHOD(_write);
			sensitive<<_dmWrite.pos();
		};

	private:
		_dataSize _memory[_dmCount];

		void _read () {
			_dataOut.write(_memory[_addr.read()]) ;

			cout<<"/**===================================DATA MEMORY LOG===================================**/"<<endl;
			cout<<"       Address Input : "<<_addr.read()<<endl;
			cout<<"       Value Output : "<<_memory[_addr.read()]<<endl;
			cout<<"/**===================================DATA MEMORY LOG===================================**/"<<endl<<endl<<endl;
		};

		void _write () {
			_memory[_addr.read()] = _dataIn.read();

			cout<<"/**===================================DATA MEMORY LOG===================================**/"<<endl;
			cout<<"       Address Input : "<<_addr.read()<<endl;
			cout<<"       Value Input : "<<_dataIn.read()<<endl;
			cout<<"/**===================================DATA MEMORY LOG===================================**/"<<endl<<endl<<endl;
		};
};

#endif