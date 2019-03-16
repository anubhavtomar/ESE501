/**
 * Author:    Anubhav Tomar
 * 
 * Program Memory Definition
 **/

#ifndef PM_H
#define PM_H

#include<systemc.h>

template <class _addrSize , unsigned _pmCount>
class _programMemory : public sc_module {
	public:
		// Inputs
		sc_in<_addrSize> _addrPC;
		sc_in<bool> _pmRead;

		// Outputs
		sc_out<sc_uint<16> > _instructionOut;

		SC_HAS_PROCESS(_programMemory);

		_programMemory(sc_module_name name) : sc_module(name) { 
			SC_METHOD(_read);
			dont_initialize();
			sensitive<<_pmRead.pos();
		};

		void write(std::vector<sc_uint<16> >  _program) {
			sc_uint<5> _len = _program.size();
			for(sc_uint<5> i ; i < _len ; i++) {
				_programStore.push_back(_program[i]);
			}
		}

		void _read () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _read--------"<<endl;
			_instructionOut.write(_programStore[_addrPC]);
			cout<<"@ "<<sc_time_stamp()<<"------End _read--------"<<endl;
		};

	private:
		std::vector<_addrSize> _programStore;
};

#endif