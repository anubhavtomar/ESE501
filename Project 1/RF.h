/**
 * Author:    Anubhav Tomar
 * 
 * Register File Definition
 **/

#ifndef RF_H
#define RF_H

#include<systemc.h>

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
		sc_out<_dataSize > _dataOut1;
		sc_out<_dataSize > _dataOut2;

		SC_HAS_PROCESS(RegisterFile);

		RegisterFile(SC_MODULE_NAME name : sc_module(name)) { 
			SC_METHOD(_read);
			sensitive<<_rfRead.pos();

			SC_METHOD(_write);
			sensitive<<_rfWrite.pos();
		};

	private:
		_dataSize _registers[N];
		_dataSize _immRegisters[N];

		_dataSize _read() { 
			_dataOut1.write(_isImmData ? _immRegisters[_addr1] : _registers[_addr1]) ;
		};

		bool _write() { 
			if(_isImmData.read()) {
				_immRegisters[_addr1] = _data1.read();
			} else {
				_registers[_addr1] = _data1.read();
			}
		};
};

#endif