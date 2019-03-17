/**
 * Author:    Anubhav Tomar
 * 
 * Program Memory Definition
 **/

#ifndef PM_H
#define PM_H

#include<systemc.h>

template <class _addrSize>
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
			sensitive<<_pmRead.pos();
		};

		void write(std::vector<sc_uint<16> >  _program) {
			cout<<"@ "<<sc_time_stamp()<<"------Start _programLoad--------"<<endl<<endl<<endl;
			sc_uint<5> _len = _program.size();
			for(sc_uint<5> i ; i < _len ; i++) {
				_programStore.push_back(_program[i]);
				cout<<"/**===================================PROGRAM MEMORY LOG===================================**/"<<endl;
				cout<<"       Address Input : "<<i<<endl;
				cout<<"       Instruction Output : "<<_program[i]<<endl;
				cout<<"/**===================================PROGRAM MEMORY LOG===================================**/"<<endl<<endl<<endl;
			}
			cout<<"@ "<<sc_time_stamp()<<"------End _programLoad--------"<<endl<<endl<<endl;
		}

	private:
		std::vector<_addrSize> _programStore;

		void _read () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _read--------"<<endl<<endl<<endl;
			_instructionOut.write(_programStore[_addrPC.read()]);

			cout<<"/**===================================PROGRAM MEMORY LOG===================================**/"<<endl;
			cout<<"       Address Input : "<<_addrPC.read()<<endl;
			cout<<"       Instruction Output : "<<_programStore[_addrPC.read()]<<endl;
			cout<<"/**===================================PROGRAM MEMORY LOG===================================**/"<<endl<<endl<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------End _read--------"<<endl<<endl<<endl;
		};
};

#endif