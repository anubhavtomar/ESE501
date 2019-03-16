/**
 * Author:    Anubhav Tomar
 * 
 * Controller Definition
 **/

#include<systemc.h>

template <class _addrSize , class _dataSize>
class _controllerBlock : public sc_module { 
	public:
		// Inputs
		sc_in<bool> _clock;
		sc_in<sc_uint<16> > _instructionIn;
		sc_in<_dataSize> _operand1In;
		sc_in<_dataSize> _operand2In;
		sc_in<_dataSize> _dmDataIn;
		sc_in<_dataSize> _aluResultIn;

		// Outputs to PM
		sc_out<sc_uint<16> > _PCOut;
		sc_out<bool> _pmRead;

		// Outputs to DM
		sc_out<_addrSize> _dmAddrOut;
		sc_out<bool> _dmRead;
		sc_out<bool> _dmWrite;
		sc_out<_dataSize> _dmDataOut;

		// Outputs to RF
		sc_out<_addrSize> _rfAddr1Out;
		sc_out<_addrSize> _rfAddr2Out;
		sc_out<_dataSize> _rfDataOut;
		sc_out<bool> _rfRead;
		sc_out<bool> _rfWrite;
		sc_out<sc_uint<1> > _isImmData;

		// Outputs to ALU
		sc_out<bool> _aluEnable;
		sc_out<sc_uint<4> > _aluControlSig;
		sc_out<sc_uint<4> > _condControlSig;
		sc_out<_dataSize> _operand1Out;
		sc_out<_dataSize> _operand2Out;

		enum states {S0 , S1 , S2 , S3 , S4 , S5 , S6};
		sc_signal<states> _currentState;
		
		SC_HAS_PROCESS(_controllerBlock);

		_controllerBlock(sc_module_name name) : sc_module(name) {
			PC = 0x0000;
			_currentState.write(S0);

			SC_METHOD(_runControlRising);
			dont_initialize();
			sensitive<<_clock.pos();

			SC_METHOD(_runControlFalling);
			dont_initialize();
			sensitive<<_clock.neg();
		};

	private:
		std::vector<sc_uint<65> > _instructionControlSigQ;
		sc_uint<16> _IR;
		sc_uint<16> PC;

		void _runControlRising () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runControlRising--------"<<endl;

			switch(_currentState.read()) {
				case S0 :
					_currentState.write(S1);
					_fetch();
					break;
				case S2 : 
	   	        	_currentState.write(S3);
	   	        	_runRFRead();
	   	        	_fetch();
			        break;
				case S3 : 
	               	_currentState.write(S4);
	               	_execute();
	   	        	_decode();
		           	break;
	           	case S4 : 
	               	_currentState.write(S5);
	               	_memAccess();
		           	break;
				default:
					break;
			}
			
			cout<<"@ "<<sc_time_stamp()<<"------End _runControlRising--------"<<endl;
		}

		void _runControlFalling () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runControlRising--------"<<endl;

			switch(_currentState.read()) {
				case S1 :
					_currentState.write(S2);
					_decode();
					break;
				case S5 : 
	            	_currentState.write(S6);
	            	_runRFWriteBack();
	            	_currentState.write(S0);
		       		break;
				default:
					break;
			}

			cout<<"@ "<<sc_time_stamp()<<"------End _runControlRising--------"<<endl;
		}

		void _fetch () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _fetch from PM--------"<<endl;

			_PCOut.write(PC);
			_pmRead.write(1);
			wait(20 , SC_NS);
			_IR = _instructionIn.read();
			wait(20 , SC_NS);
			_pmRead.write(0);
			PC += 1;

			cout<<"@ "<<sc_time_stamp()<<"------End _fetch from PM--------"<<endl;
		}

		sc_uint<65> _prepareInstructionControlSig (sc_uint<4> _aluControlSig , sc_uint<1> _isImmData , sc_uint<4> _rDestCond , sc_uint<4> _opCodeExtImmH , sc_uint<4> _rSrcImmL) {
			cout<<"@ "<<sc_time_stamp()<<"------Start _prepareInstructionControlSig--------"<<endl;

			sc_uint<65> _instructionControlSig;
			_instructionControlSig.range(13 , 16) = _aluControlSig; // _aluControlSig
			_instructionControlSig.range(12 , 12) = _isImmData; // _isImmData
			_instructionControlSig.range(8 , 11) = _rDestCond; // _condControlSig
			_instructionControlSig.range(4 , 7) = _opCodeExtImmH; // Rdest/ImmHi
			_instructionControlSig.range(0 , 3) = _rSrcImmL; // Rsrc/ImmLo

			cout<<"@ "<<sc_time_stamp()<<"------End _prepareInstructionControlSig--------"<<endl;
			return _instructionControlSig;
		}

		void _decode () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _decode instruction--------"<<endl;

			sc_uint<4> _opcode = _IR.range(12 , 15);
			sc_uint<4> _rDestCond = _IR.range(8 , 11);
			sc_uint<4> _opCodeExtImmH = _IR.range(4 , 7);
			sc_uint<4> _rSrcImmL = _IR.range(0 , 3);
			sc_uint<65> _instructionControlSig;

			switch(_opcode) {
				case 0b0000 : // ADD , SUB , CMP , AND , OR , XOR , MOV
					switch(_opCodeExtImmH) {
						case 0b0101 : // ADD
							_instructionControlSig = _prepareInstructionControlSig(0b0000 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1001 : // SUB
							_instructionControlSig = _prepareInstructionControlSig(0b0001 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1011 : // CMP
							_instructionControlSig = _prepareInstructionControlSig(0b0010 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0001 : // AND
							_instructionControlSig = _prepareInstructionControlSig(0b0011 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0010 : // OR
							_instructionControlSig = _prepareInstructionControlSig(0b0100 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0011 : // XOR
							_instructionControlSig = _prepareInstructionControlSig(0b0101 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1101 : // MOV
							_instructionControlSig = _prepareInstructionControlSig(0b0110 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
					}
					break;
				case 0b0101 : // ADDI
					_instructionControlSig = _prepareInstructionControlSig(0b0000 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1001 : // SUBI
					_instructionControlSig = _prepareInstructionControlSig(0b0001 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1011 : // CMPI
					_instructionControlSig = _prepareInstructionControlSig(0b0010 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0001 : // ANDI
					_instructionControlSig = _prepareInstructionControlSig(0b0011 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0010 : // ORI
					_instructionControlSig = _prepareInstructionControlSig(0b0100 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0011 : // XORI	
					_instructionControlSig = _prepareInstructionControlSig(0b0101 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1101 : // MOVI
					_instructionControlSig = _prepareInstructionControlSig(0b0110 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1000 : // LSH , LSHI , ASH , ASHI
					switch(_opCodeExtImmH) {
						case 0b0100 : // LSH
							_instructionControlSig = _prepareInstructionControlSig(0b0111 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0000 : // LSHI
							_instructionControlSig = _prepareInstructionControlSig(0b0111 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0110 : // ASH
							_instructionControlSig = _prepareInstructionControlSig(0b1000 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0001 : // ASHI
							_instructionControlSig = _prepareInstructionControlSig(0b1000 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
					}
					break;
				case 0b1111 : // LUI
					_instructionControlSig = _prepareInstructionControlSig(0b1001 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0100 : // LOAD , STOR , Jcond , JAL
					switch(_opCodeExtImmH) {
						case 0b0000 : // LOAD
							_instructionControlSig = _prepareInstructionControlSig(0b1001 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0100 : // STOR
							_instructionControlSig = _prepareInstructionControlSig(0b1010 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1100 : // Jcond
							_instructionControlSig = _prepareInstructionControlSig(0b1100 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1000 : // JAL
							_instructionControlSig = _prepareInstructionControlSig(0b1101 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
					}
					break;
				case 0b1100 : // Bcond
					_instructionControlSig = _prepareInstructionControlSig(0b1011 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
			}
			_instructionControlSigQ.push_back(_instructionControlSig);

			cout<<"@ "<<sc_time_stamp()<<"------End _decode--------"<<endl;
		}

		void _runRFRead () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runRFRead--------"<<endl;

			sc_uint<48> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(13 , 16);
			_dataSize _op1 , _op2;
			if(_currentInstruction.range(12 , 12)) { // _isImmData = 1
				_rfAddr1Out.write(_currentInstruction.range(8 , 11)); // ImmHi
				_rfDataOut.write(_currentInstruction.range(4 , 7)<<4 + _currentInstruction.range(0 , 3)); // ImmHi<<4 + ImmLo

				_rfRead.write(1);
				wait(20 , SC_NS);
				_op1 = _operand1In.read();
				_op2 = _operand2In.read();
				wait(20 , SC_NS);
				_rfRead.write(0);
			} else {
				_rfAddr1Out.write(_currentInstruction.range(4 , 6)); // Rdst
				_rfAddr2Out.write(_currentInstruction.range(0 , 2)); // Rsrc

				_rfRead.write(1);
				wait(20 , SC_NS);
				_op1 = _operand1In.read();
				_op2 = _operand2In.read();
				wait(20 , SC_NS);
				_rfRead.write(0);
			}

			_instructionControlSigQ[0].range(18 , 32) = _op1;
			_instructionControlSigQ[0].range(33 , 48) = _op2;
			if(_aluSig == 0b1100 || _aluSig == 0b1011) { // Jcond , Bcond
				_instructionControlSigQ[0].range(33 , 48) = PC; // Store PC as _operand2Out
			}

			cout<<"@ "<<sc_time_stamp()<<"------End _runRFRead--------"<<endl;
		}

		void _execute () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _execute--------"<<endl;

			sc_uint<48> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(13 , 16);
			_aluControlSig.write(_currentInstruction.range(13 , 16));
			_condControlSig.write(_currentInstruction.range(8 , 11));
			_operand1Out.write(_currentInstruction.range(18 , 32));
			_operand2Out.write(_currentInstruction.range(33 , 48));
			_aluEnable.write(1);
			wait(20 , SC_NS);
			_dataSize _res = _aluResultIn.read();
			wait(20 , SC_NS);
			_aluEnable.write(0);

			_instructionControlSigQ[0].range(49 , 64) = _res;

			cout<<"@ "<<sc_time_stamp()<<"------End _execute--------"<<endl;
		}

		void _memAccess () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _memAccess--------"<<endl;

			sc_uint<48> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(13 , 16);
			if(_aluSig == 0b1001) { // LOAD
				_dmAddrOut.write(_currentInstruction.range(49 , 64)); // Raddr value
				_dmRead.write(1);
				wait(20 , SC_NS);
				_dataSize _res = _dmDataIn.read();
				wait(20 , SC_NS);
				_dmRead.write(0);
				_currentInstruction.range(49 , 64) = _res;
			} else if(_aluSig ==  0b1010) { // STOR
				_dmAddrOut.write(_currentInstruction.range(33 , 48)); // Raddr value
				_dmDataOut.write(_currentInstruction.range(49 , 64)); // Rsrc value
				_dmWrite.write(1);
				wait(20 , SC_NS);
				wait(20 , SC_NS);
				_dmWrite.write(0);
			} else if(_aluSig == 0b1011 || _aluSig == 0b1100) { // Jcond , Bcond
				PC = _currentInstruction.range(49 , 64);
			}

			cout<<"@ "<<sc_time_stamp()<<"------End _memAccess--------"<<endl;
		} 

		void _runRFWriteBack() {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runRFWrite--------"<<endl;

			sc_uint<48> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(13 , 16);
			if(_aluSig != 0b0010 && _aluSig != 0b1010 && _aluSig != 0b1011 && _aluSig != 0b1100) { // All except CMP , CMPI , STOR , Jcond , Bcond
				_rfAddr1Out.write(_currentInstruction.range(8 , 11)); // Rdst
				_rfAddr2Out.write(_currentInstruction.range(49 , 64)); // Result
				_rfWrite.write(1);
				wait(20 , SC_NS);
				wait(20 , SC_NS);
				_rfWrite.write(0);
			}

			_instructionControlSigQ.erase(_instructionControlSigQ.begin());

			cout<<"@ "<<sc_time_stamp()<<"------End _runRFWrite--------"<<endl;
		}
};