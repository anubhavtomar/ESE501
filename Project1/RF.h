/**
 * Author:    Anubhav Tomar
 * 
 * Register File Definition
 **/

#ifndef RF_H
#define RF_H

#include<systemc.h>

template <class _addrSize , class _dataSize , unsigned _regCount>
class _registerFile : public sc_module {
	public:
		// Inputs
		sc_in<_addrSize> _addr1;
		sc_in<_addrSize> _addr2;
		sc_in<_addrSize> _addr3;
		sc_in<_dataSize> _data;
		sc_in<bool> _rfRead;
		sc_in<bool> _rfWrite;
		sc_in<sc_uint<1> > _isImmData;

		// Outputs
		sc_out<_dataSize> _dataOut1;
		sc_out<_dataSize> _dataOut2;

		SC_HAS_PROCESS(_registerFile);

		_registerFile(sc_module_name name) : sc_module(name) { 
			SC_METHOD(_read);
			dont_initialize();
			sensitive<<_rfRead.pos();

			SC_METHOD(_write);
			dont_initialize();
			sensitive<<_rfWrite.pos();
		};

		void _read () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _read--------"<<endl;
			sc_uint<1> _isImm = _isImmData.read();
			sc_uint<4> _adr1 = _addr1.read();
			sc_uint<4> _adr2 = _addr2.read();
			_dataOut1.write(_isImm ? 0 : _registers[_adr1].read());
			_dataOut2.write(_isImm ? _immRegisters[_adr2] : _registers[_adr2]) ;
			cout<<"@ "<<sc_time_stamp()<<"------End _read--------"<<endl;
		};

		void _write () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _write--------"<<endl;
			_immRegisters[_addr1.read()] = _data.read();
			cout<<"@ "<<sc_time_stamp()<<"------End _write--------"<<endl;
		};

	private:
		_dataSize _registers[_regCount];
		_dataSize _immRegisters[_regCount];
};

#endif