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
		// sc_in<_addrSize> _addr3;
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
			sensitive<<_rfRead.pos();

			SC_METHOD(_write);
			sensitive<<_rfWrite.pos();
		};

	private:
		sc_uint<16> const _dataZero = 0x0000;
		_dataSize _registers[_regCount];
		_dataSize _immRegisters[_regCount];

		void _read () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _read--------"<<endl<<endl<<endl;
			sc_uint<1> _isImm = _isImmData.read();
			_addrSize _adr1 = _addr1.read();
			_addrSize _adr2 = _addr2.read();
			_dataSize _value = _data.read();
			if(_isImm) {
				_immRegisters[_adr2] = _value;
			}
			sc_uint<16> _out1 = _registers[_adr1];
			sc_uint<16> _out2 = _isImm == 1 ? _immRegisters[_adr2] : _registers[_adr2];

			_dataOut1.write(_out1);
			_dataOut2.write(_out2) ;

			cout<<"/**===================================REGISTER FILE LOG===================================**/"<<endl;
			cout<<"       Address1 Input : "<<_adr1<<endl;
			cout<<"       Address2 Input : "<<_adr2<<endl;
			cout<<"       Is Immediate Value : "<<_isImm<<endl;
			if(_isImm) {
				cout<<"       Immediate Value : "<<_value<<endl;
			}
			cout<<"       Data1 Output : "<<_out1<<endl;
			cout<<"       Data2 Output : "<<_out2<<endl;
			cout<<"/**===================================REGISTER FILE LOG===================================**/"<<endl<<endl<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------End _read--------"<<endl<<endl<<endl;
		};

		void _write () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _write--------"<<endl<<endl<<endl;
			_addrSize _addr = _addr1.read();
			_dataSize _value = _data.read();
			_registers[_addr] = _value;

			cout<<"/**===================================REGISTER FILE LOG===================================**/"<<endl;
			cout<<"       Address Input : "<<_addr<<endl;
			cout<<"       Value Input : "<<_value<<endl;
			cout<<"/**===================================REGISTER FILE LOG===================================**/"<<endl<<endl<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------End _write--------"<<endl<<endl<<endl;
		};
};

#endif