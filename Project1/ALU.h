/**
 * Author:    Anubhav Tomar
 * 
 * ALU Definition
 **/

#ifndef ALU_H
#define ALU_H

#include<systemc.h>

template <class _dataSize> 
class _aluBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _aluEnable;
		sc_in<sc_uint<4> > _aluControlSig;
		sc_in<sc_uint<4> > _condControlSig;
		sc_in<sc_uint<1> > _isImmControlSig;
		sc_in<_dataSize> _operand1;
		sc_in<_dataSize> _operand2;

		// Outputs
		sc_out<_dataSize> _resultOut;

		SC_HAS_PROCESS(_aluBlock);

		_aluBlock(sc_module_name name) : sc_module(name) {
			SC_METHOD(_executions);
			sensitive<<_aluEnable.pos();
		};

	private:
		sc_uint<16> const _dataMAX = 0xFFFF;
		sc_uint<16> const _dataZero = 0x0000;
		sc_uint<3> const _overflowBit = 0x0007;
		sc_uint<3> const _carryBit = 0x0005;
		sc_uint<3> const _zeroBit = 0x0004;
		sc_uint<3> const _negativeBit = 0x0001;
		sc_uint<8> _PSR;

		bool _checkCondition (std::string instruction) {
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkCondition--------"<<endl<<endl<<endl;
			bool res = false;
			switch(_condControlSig.read()) {
				case 0b0000: // EQ
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : EQ"<<endl;
					if(_PSR.range(_zeroBit , _zeroBit) == 1) {
						res = true;
					}
					break;
				case 0b0001: // NE
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : NE"<<endl;
					if(_PSR.range(_zeroBit , _zeroBit) ==  0) {
						res = true;
					}
					break;
				case 0b0010: // GE
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : GE"<<endl;
					if(_PSR.range(_zeroBit , _zeroBit) ==  1 && _PSR.range(_negativeBit , _negativeBit) ==  1) {
						res = true;
					}
					break;
				case 0b0011: // CS
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : CS"<<endl;
					if(_PSR.range(_carryBit , _carryBit) ==  1) {
						res = true;
					}
					break;
				case 0b0100: // CC
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : CC"<<endl;
					if(_PSR.range(_carryBit , _carryBit) ==  0) {
						res = true;
					}
					break;
				case 0b0101: // GT
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : GT"<<endl;
					if(_PSR.range(_negativeBit , _negativeBit) ==  1) {
						res = true;
					}
					break;
				case 0b0110: // LE
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : LE"<<endl;
					if(_PSR.range(_negativeBit , _negativeBit) ==  0) {
						res = true;
					}
					break;
				case 0b0111: // FS
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : FS"<<endl;
					if(_PSR.range(_overflowBit , _overflowBit) ==  1) {
						res = true;
					}
					break;
				case 0b1000: // FC
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : FC"<<endl;
					if(_PSR.range(_overflowBit , _overflowBit) ==  0) {
						res = true;
					}
					break;
				case 0b1001: // LT
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : LT"<<endl;
					if(_PSR.range(_zeroBit , _zeroBit) ==  0 && _PSR.range(_negativeBit , _negativeBit) ==  0) {
						res = true;
					}
					break;
				case 0b1010: // US
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<instruction<<endl;
					cout<<"       Condition : US"<<endl;
					res = true;
					break;
			}
			cout<<"@ "<<sc_time_stamp()<<"------End _checkCondition--------"<<endl<<endl<<endl;
			return res;
		}

		void _executions () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _executions--------"<<endl<<endl<<endl;
			sc_uint<16> _op1 = _operand1.read();
			sc_uint<16> _op2 = _operand2.read();
			_dataSize _out;
			switch(_aluControlSig.read()) {
				case 0b0000 : // ADD & ADDI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 + _op2;
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}

					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : ADD/ADI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
 					break;
				case 0b0001 : // SUB & SUBI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_op1 < _op2) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_out = abs(_op1 - _op2);
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : SUB/SUBI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break;
				case 0b0010 : // CMP & CMPI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_op1 < _op2) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_out = abs(_op1 - _op2);
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_resultOut == _dataZero) {
						_PSR.range(_zeroBit , _zeroBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : CMP/CMPI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b0011 : // AND & ANDI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 & _op2;
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : AND/ANDI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b0100 : // OR & ORI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 | _op2;
					_resultOut.write(_out);
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : OR/ORI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b0101 : // XOR & XORI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 ^ _op2;
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : XOR/XORI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b0110 : // MOV & MOVI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op2;
					_resultOut.write(_out);
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : MOV/MOVI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b0111 : // LSH & LSHI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 << _op2;
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : LSH/LSHI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b1000 : // ASH & ASHI
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 >> _op2;
					_resultOut.write(_out);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : ASH/ASHI"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b1001 : // LUI & LOAD
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op2;
					_resultOut.write(_out);
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : LUI/LOAD"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b1010: // STOR
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1;
					_resultOut.write(_out);
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : STOR"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b1011 : // Bcond
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1 + _op2;
					if(_checkCondition("       Instruction : Bcond")) {
						_resultOut.write(_out);
					}
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break;
				case 0b1100 : // Jcond
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_checkCondition("       Instruction : Jcond")) {
						_out = _op1;
					} else {
						_out = _op2;
					}
					_resultOut.write(_out);
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
				case 0b1101 : // JAL 
					if(_op1 >= _dataMAX || _op2 >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_out = _op1;
					cout<<"/**===================================ALU LOG===================================**/"<<endl;
					cout<<"       Instruction : JAL"<<endl;
					cout<<"       _Immediate : "<<_isImmControlSig.read()<<endl;
					cout<<"       _operand1 :  "<<_op1<<endl;
					cout<<"       _operand2 :  "<<_op2<<endl;
					cout<<"       _result :  "<<_out<<endl;
					cout<<"       _PSR :  "<<_PSR<<endl;
					cout<<"/**===================================ALU LOG===================================**/"<<endl<<endl<<endl;
					break; 
			}
			cout<<"@ "<<sc_time_stamp()<<"------End _executions--------"<<endl<<endl<<endl;
		}
};

#endif