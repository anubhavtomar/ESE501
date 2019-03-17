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
		sc_out<sc_uint<1> > _isImmControlSig;
		sc_out<_dataSize> _operand1Out;
		sc_out<_dataSize> _operand2Out;

		enum states {S0 , S1 , S2 , S3 , S4 , S5 , S6};
		sc_signal<states> _currentState;
		
		SC_HAS_PROCESS(_controllerBlock);

		_controllerBlock(sc_module_name name) : sc_module(name) {
			PC = 0x0000;
			_currentState.write(S0);

			SC_THREAD(_runControl);
			sensitive<<_clock;
		};

	private:
		std::vector<sc_uint<49> > _instructionControlSigQ;
		sc_uint<16> _IR;
		sc_uint<16> PC;

		void _runControl () {
			while(true) {
				wait();
				if(_clock.read() == 1) {
					cout<<"@ "<<sc_time_stamp()<<"------Start _runControlRising--------"<<endl<<endl<<endl;

					switch(_currentState.read()) {
						case S0 :
							_currentState.write(S1);
							_runFetch();
							break;
						case S2 : 
			   	        	_currentState.write(S3);
			   	        	_runRFRead();
			   	        	_runFetch();
					        break;
						case S3 : 
			               	_currentState.write(S4);
			               	_runExecution();
			   	        	_runDecode();
				           	break;
			           	case S4 : 
			               	_currentState.write(S5);
			               	_runMemAccess();
				           	break;
						default:
							break;
					}
					
					cout<<"@ "<<sc_time_stamp()<<"------End _runControlRising--------"<<endl<<endl<<endl;
				} else {
					cout<<"@ "<<sc_time_stamp()<<"------Start _runControlFalling--------"<<endl<<endl<<endl;

					switch(_currentState.read()) {
						case S1 :
							_currentState.write(S2);
							_runDecode();
							break;
						case S5 : 
			            	_currentState.write(S6);
			            	_runRFWriteBack();
			            	_currentState.write(S0);
				       		break;
						default:
							break;
					}

					cout<<"@ "<<sc_time_stamp()<<"------End _runControlFalling--------"<<endl<<endl<<endl;
				}
			}
		}

		void _runFetch () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runFetch from PM--------"<<endl<<endl<<endl;
			if(_currentState.read() != S0 && _IR == 0) { // NOP
				return;
			}
			_PCOut.write(PC);
			_pmRead.write(1);
			wait(10 , SC_PS);
			_IR = _instructionIn.read();
			wait(10 , SC_PS);
			_pmRead.write(0);
			if(_IR == 0) { // NOP
				return;
			}
			PC += 1;

			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
			cout<<"       Stage : Instruction Fetch"<<endl;
			cout<<"       Instruction : "<<_IR<<endl;
			cout<<"       Current PC Value : "<<PC<<endl;
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------End _runFetch from PM--------"<<endl<<endl<<endl;
		}

		sc_uint<49> _prepareInstructionControlSig (sc_uint<4> _aluControlSig , sc_uint<1> _isImmData , sc_uint<4> _rDestCond , sc_uint<4> _opCodeExtImmH , sc_uint<4> _rSrcImmL) {
			cout<<"@ "<<sc_time_stamp()<<"------Start _prepareInstructionControlSig--------"<<endl;

			sc_uint<49> _instructionControlSig;
			_instructionControlSig.range(16 , 13) = _aluControlSig; // _aluControlSig
			_instructionControlSig.range(12 , 12) = _isImmData; // _isImmData
			_instructionControlSig.range(11 , 8) = _rDestCond; // _condControlSig
			_instructionControlSig.range(7 , 4) = _opCodeExtImmH; // Rdest/ImmHi
			_instructionControlSig.range(3 , 0) = _rSrcImmL; // Rsrc/ImmLo
			cout<<"       _aluControlSig : "<<_aluControlSig<<endl;
			cout<<"       _isImmData : "<<_isImmData<<endl;
			cout<<"       _rDestCond : "<<_rDestCond<<endl;
			cout<<"       _opCodeExtImmH : "<<_opCodeExtImmH<<endl;
			cout<<"       _rSrcImmL : "<<_rSrcImmL<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------End _prepareInstructionControlSig--------"<<endl;
			return _instructionControlSig;
		}

		void _runDecode () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runDecode--------"<<endl<<endl<<endl;
			if(_IR == 0) { // NOP
				cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
					cout<<"       Stage : Instruction Decode"<<endl;
					cout<<"       Instruction Opcode : NOP"<<endl;
				cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;
				return;
			}
			sc_uint<4> _opcode = _IR.range(15 , 12);
			sc_uint<4> _rDestCond = _IR.range(11 , 8);
			sc_uint<4> _opCodeExtImmH = _IR.range(7 , 4);
			sc_uint<4> _rSrcImmL = _IR.range(3 , 0);
			sc_uint<49> _instructionControlSig;

			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
			cout<<"       Stage : Instruction Decode"<<endl;
			cout<<"       Instruction Opcode : "<<_IR<<endl;
			switch(_opcode) {
				case 0b0000 : // ADD , SUB , CMP , AND , OR , XOR , MOV
					switch(_opCodeExtImmH) {
						case 0b0101 : // ADD
							cout<<"       Instruction : ADD"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0000 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1001 : // SUB
							cout<<"       Instruction : SUB"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0001 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1011 : // CMP
							cout<<"       Instruction : CMP"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0010 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0001 : // AND
							cout<<"       Instruction : AND"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0011 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0010 : // OR
							cout<<"       Instruction : OR"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0100 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0011 : // XOR
							cout<<"       Instruction : XOR"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0101 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1101 : // MOV
							cout<<"       Instruction : MOV"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0110 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
					}
					break;
				case 0b0101 : // ADDI
					cout<<"       Instruction : ADDI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0000 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1001 : // SUBI
					cout<<"       Instruction : SUBI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0001 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1011 : // CMPI
					cout<<"       Instruction : CMPI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0010 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0001 : // ANDI
					cout<<"       Instruction : ANDI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0011 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0010 : // ORI
					cout<<"       Instruction : ORI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0100 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0011 : // XORI	
					cout<<"       Instruction : XORI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0101 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1101 : // MOVI
					cout<<"       Instruction : MOVI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b0110 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b1000 : // LSH , LSHI , ASH , ASHI
					switch(_opCodeExtImmH) {
						case 0b0100 : // LSH
							cout<<"       Instruction : LSH"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0111 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0000 : // LSHI
							cout<<"       Instruction : LSHI"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b0111 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0110 : // ASH
							cout<<"       Instruction : ASH"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b1000 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0001 : // ASHI
							cout<<"       Instruction : ASHI"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b1000 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
					}
					break;
				case 0b1111 : // LUI
					cout<<"       Instruction : LUI"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b1001 , 1 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
				case 0b0100 : // LOAD , STOR , Jcond , JAL
					switch(_opCodeExtImmH) {
						case 0b0000 : // LOAD
							cout<<"       Instruction : LOAD"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b1001 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b0100 : // STOR
							cout<<"       Instruction : STOR"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b1010 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1100 : // Jcond
							cout<<"       Instruction : Jcond"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b1100 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
						case 0b1000 : // JAL
							cout<<"       Instruction : JAL"<<endl;
							_instructionControlSig = _prepareInstructionControlSig(0b1101 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
							break;
					}
					break;
				case 0b1100 : // Bcond
					cout<<"       Instruction : Bcond"<<endl;
					_instructionControlSig = _prepareInstructionControlSig(0b1011 , 0 , _rDestCond , _opCodeExtImmH , _rSrcImmL);
					break;
			}
			_instructionControlSigQ.push_back(_instructionControlSig);

			cout<<"       _instructionControlSig : "<<_instructionControlSig<<endl;
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _runDecode--------"<<endl<<endl<<endl;
		}

		void _runRFRead () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runRFRead--------"<<endl<<endl<<endl;
			if(!_instructionControlSigQ.size()) {
				return;
			}
			sc_uint<48> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(16 , 13);
			_dataSize _op1 , _op2;
			if(_currentInstruction.range(12 , 12)) { // _isImmData = 1
				_rfAddr1Out.write(_currentInstruction.range(11 , 8)); // Rdst
				sc_uint<16> parseData = _currentInstruction.range(7 , 4); // ImmHi<<4 + ImmLo
				parseData = parseData<<4;
				parseData += _currentInstruction.range(3 , 0);
				_rfDataOut.write(parseData);
				_isImmData.write(1);
				_rfRead.write(true);
				wait(10 , SC_PS);
				_op1 = _operand1In.read();
				_op2 = _operand2In.read();
				wait(10 , SC_PS);
				_rfRead.write(false);
			} else {
				_rfAddr1Out.write(_currentInstruction.range(11 , 8)); // Rdst
				_rfAddr2Out.write(_currentInstruction.range(3 , 0)); // Rsrc
				_isImmData.write(0);
				_rfRead.write(true);
				wait(10 , SC_PS);
				_op1 = _operand1In.read();
				_op2 = _operand2In.read();
				wait(10 , SC_PS);
				_rfRead.write(false);
			}

			_instructionControlSigQ[0].range(32 , 17) = _op1;
			_instructionControlSigQ[0].range(48 , 33) = _op2;
			if(_aluSig == 0b1100 || _aluSig == 0b1011) { // Jcond , Bcond
				_instructionControlSigQ[0].range(32 , 17) = _currentInstruction.range(3 , 0);
				_instructionControlSigQ[0].range(48 , 33) = PC; // Store PC as _operand2Out
			}

			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
			cout<<"       Stage : RF Access"<<endl;
			cout<<"       _instructionControlSig : "<<_instructionControlSigQ[0]<<endl;
			cout<<"       Operand1 : "<<_op1<<endl;
			cout<<"       Operand2 : "<<_op2<<endl;
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _runRFRead--------"<<endl<<endl<<endl;
		}

		void _runExecution () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runExecution--------"<<endl<<endl<<endl;
			if(!_instructionControlSigQ.size()) {
				return;
			}
			sc_uint<49> _currentInstruction = _instructionControlSigQ.front();
			_aluControlSig.write(_currentInstruction.range(16 , 13));
			_condControlSig.write(_currentInstruction.range(11 , 8));
			_isImmControlSig.write(_currentInstruction.range(12 , 12));
			_operand1Out.write(_currentInstruction.range(32 , 17));
			_operand2Out.write(_currentInstruction.range(48 , 33));
			_aluEnable.write(1);
			wait(10 , SC_PS);
			_dataSize _res = _aluResultIn.read();
			wait(10 , SC_PS);
			_aluEnable.write(0);

			_instructionControlSigQ[0].range(48 , 33) = _res; // Overwrite operand2. Only operand1 is required in _memAccess & _runRFWriteBack

			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
			cout<<"       Stage : Execution"<<endl;
			cout<<"       _instructionControlSig : "<<_instructionControlSigQ[0]<<endl;
			cout<<"       Result : "<<_res<<endl;
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _runExecution--------"<<endl<<endl<<endl;
		}

		void _runMemAccess () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runMemAccess--------"<<endl<<endl<<endl;
			if(!_instructionControlSigQ.size()) {
				return;
			}
			sc_uint<49> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(16 , 13);
			if(_aluSig == 0b1001) { // LOAD
				_dmAddrOut.write(_currentInstruction.range(48 , 33)); // Raddr value
				_dmRead.write(1);
				wait(10 , SC_PS);
				_dataSize _res = _dmDataIn.read();
				wait(10 , SC_PS);
				_dmRead.write(0);
				_currentInstruction.range(48 , 33) = _res; // Overwrite operand2. Raddr value not required
			} else if(_aluSig ==  0b1010) { // STOR
				_dmAddrOut.write(_currentInstruction.range(32 , 17)); // Raddr value
				_dmDataOut.write(_currentInstruction.range(48 , 33)); // Rsrc value
				_dmWrite.write(1);
				wait(10 , SC_PS);
				wait(10 , SC_PS);
				_dmWrite.write(0);
			} else if(_aluSig == 0b1011 || _aluSig == 0b1100) { // Jcond , Bcond
				PC = _currentInstruction.range(48 , 33);
			}

			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
			cout<<"       Stage : Memory Access"<<endl;
			cout<<"       _instructionControlSig : "<<_instructionControlSigQ[0]<<endl;
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _runMemAccess--------"<<endl<<endl<<endl;
		} 

		void _runRFWriteBack() {
			cout<<"@ "<<sc_time_stamp()<<"------Start _runRFWrite--------"<<endl<<endl<<endl;
			if(!_instructionControlSigQ.size()) {
				return;
			}
			sc_uint<49> _currentInstruction = _instructionControlSigQ.front();
			sc_uint<4> _aluSig = _currentInstruction.range(16 , 13);
			if(_aluSig != 0b0010 && _aluSig != 0b1010 && _aluSig != 0b1011 && _aluSig != 0b1100) { // All except CMP , CMPI , STOR , Jcond , Bcond
				_rfAddr1Out.write(_currentInstruction.range(11 , 8)); // Rdst
				_rfDataOut.write(_currentInstruction.range(48 , 33)); // Result
				_rfWrite.write(1);
				wait(10 , SC_PS);
				wait(10 , SC_PS);
				_rfWrite.write(0);
			}

			_instructionControlSigQ.erase(_instructionControlSigQ.begin());
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl;
			cout<<"       Stage : RF Write Back"<<endl;
			cout<<"       _instructionControlSigQ Length : "<<_instructionControlSigQ.size()<<endl;
			cout<<"/**===================================CONTROLLER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _runRFWrite--------"<<endl<<endl<<endl;
		}
};
