/**
 * Author:    Anubhav Tomar
 * 
 * Controller Definition
 **/

#include<systemc.h>

template <class _dataSize , unsigned N>
class _controller : public sc_module {
	public:
		sc_in<> _IR;
		
		
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