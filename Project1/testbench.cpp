/**
 * Author:    Anubhav Tomar
 * 
 * RISC Processor & Testbench
 **/

#include<systemc.h>
#include<RF.h> //Register File
#include<DM.h> //Data Memory
#include<PM.h> //Program Memory
#include<ALU.h> //ALU
#include<controller.cpp> //Controller

template <class _rfAddrSize , class _pmAddrSize , class _dmAddrSize , class _dataSize , unsigned _regCount , unsigned _dmCount>
class _riscProcessor : public sc_module {
	public:
		// Signals for Module Interconnections
		sc_signal<bool> _clock , _aluEnable , _pmRead , _rfRead , _rfWrite , _dmRead , _dmWrite;
		sc_signal<sc_uint<1> > _isImmData , _isImmDataALU;
		sc_signal<sc_uint<4> >_aluControlSig , _condControlSig;
		sc_signal<_rfAddrSize > _rfAddr1Out , _rfAddr2Out;
		sc_signal<_dmAddrSize > _dmAddrOut;
		sc_signal<_dataSize > _instruction , _rfData1Out , _rfData2Out , _dmDataIn , _aluResultOut , _PCOut , _rfDataOut , _operand1Out , _operand2Out , _dmDataOut;

		_registerFile<_rfAddrSize , _dataSize , _regCount>* _rf;
		_dataMemory<_dmAddrSize , _dataSize , _dmCount>* _dm;
		_programMemory<_pmAddrSize>* _pm;
		_aluBlock<_dataSize>* _alu;
		_controllerBlock<_rfAddrSize , _dataSize>* _controller;

		SC_HAS_PROCESS(_riscProcessor);
		_riscProcessor(sc_module_name name) : sc_module(name) {

			/**
			 * 
			 * Module Instances Created
			 *
			 **/
			_rf = new _registerFile<_rfAddrSize , _dataSize , _regCount> ("Register File");
			_dm = new _dataMemory<_dmAddrSize , _dataSize , _dmCount> ("Date Memory");
			_pm = new _programMemory<_pmAddrSize> ("Program Memory");
			_alu = new _aluBlock<_dataSize> ("ALU Block");
			_controller = new _controllerBlock<_rfAddrSize , _dataSize> ("Controller");

			/**
			 * 
			 * Module Interconnectinos using Signals
			 *
			 **/

			_controller->_clock(_clock);

			/**
			 * 
			 * Controller -> Data Memory 
			 *
			 **/
			_controller->_dmRead(_dmRead);
			_dm->_dmRead(_dmRead);

			_controller->_dmAddrOut(_dmAddrOut);
			_dm->_addr(_dmAddrOut);

			_controller->_dmWrite(_dmWrite);
			_dm->_dmWrite(_dmWrite);

			_controller->_dmDataOut(_dmDataOut);
			_dm->_dataIn(_dmDataOut);


			/**
			 * 
			 * Program Memory -> Controller
			 *
			 **/
			_pm->_instructionOut(_instruction);
			_controller->_instructionIn(_instruction);


			/**
			 * 
			 * Register File -> Controller
			 *
			 **/
			_rf->_dataOut1(_rfData1Out);
			_controller->_operand1In(_rfData1Out);

			_rf->_dataOut2(_rfData2Out);
			_controller->_operand2In(_rfData2Out);


			/**
			 * 
			 * Data Memory -> Controller
			 *
			 **/
			_dm->_dataOut(_dmDataIn);
			_controller->_dmDataIn(_dmDataIn);


			/**
			 * 
			 * ALU -> Controller
			 *
			 **/
			_alu->_resultOut(_aluResultOut);
			_controller->_aluResultIn(_aluResultOut);


			/**
			 * 
			 * Controller -> Program Memory 
			 *
			 **/
			_controller->_PCOut(_PCOut);
			_pm->_addrPC(_PCOut);

			_controller->_pmRead(_pmRead);
			_pm->_pmRead(_pmRead);
			

			/**
			 * 
			 * Controller -> Register File
			 *
			 **/
			_controller->_rfRead(_rfRead);
			_rf->_rfRead(_rfRead);

			_controller->_rfWrite(_rfWrite);
			_rf->_rfWrite(_rfWrite);

			_controller->_isImmData(_isImmData);
			_rf->_isImmData(_isImmData);

			_controller->_rfAddr1Out(_rfAddr1Out);
			_rf->_addr1(_rfAddr1Out);

			_controller->_rfAddr2Out(_rfAddr2Out);
			_rf->_addr2(_rfAddr2Out);

			_controller->_rfDataOut(_rfDataOut);
			_rf->_data(_rfDataOut);


			/**
			 * 
			 * Controller -> ALU
			 *
			 **/
			_controller->_aluEnable(_aluEnable);
			_alu->_aluEnable(_aluEnable);

			_controller->_isImmControlSig(_isImmDataALU);
			_alu->_isImmControlSig(_isImmDataALU);

			_controller->_aluControlSig(_aluControlSig);
			_alu->_aluControlSig(_aluControlSig);

			_controller->_condControlSig(_condControlSig);
			_alu->_condControlSig(_condControlSig);

			_controller->_operand1Out(_operand1Out);
			_alu->_operand1(_operand1Out);

			_controller->_operand2Out(_operand2Out);
			_alu->_operand2(_operand2Out);

			SC_THREAD(clockSignal);
		};

		void clockSignal() {
			while (true){
		        wait(1 , SC_NS);
		    	_clock = false;
		        wait(1 , SC_NS);
		    	_clock = true;
			}
		};

};

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl<<endl<<endl;
	_riscProcessor<sc_uint<4> , sc_uint<16> , sc_uint<4> , sc_uint<16> , 256 , 256> _processor("RISC Processor");
	std::vector<sc_uint<16> >  _program;

	// Program to Load
	_program.push_back(0b1101000000000000); // MVI 0 , A
	_program.push_back(0b1101000100000001); // MVI 1 , B
	_program.push_back(0b1101001000001010); // MVI 9 , C
	_program.push_back(0b0000000001010001); // ADD B , A
	_program.push_back(0b0101000100000001); // ADI 1 , B
	_program.push_back(0b1001001000000001); // SUBI 1 , C
	_program.push_back(0b1011001000000000); // CMPI 0 , C
	_program.push_back(0b0100000111000011); // JNZ 3
	_program.push_back(0b0000000000000000); // NOP

	// Write Loaded Program to Program Memory
	_processor._pm->write(_program);

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(500, SC_NS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}