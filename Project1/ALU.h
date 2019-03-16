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
		sc_in<_dataSize> _operand1;
		sc_in<_dataSize> _operand2;

		// Outputs
		sc_out<_dataSize> _resultOut;
		sc_out<sc_uint<8> > _PSROut;

		SC_HAS_PROCESS(_aluBlock);

		_aluBlock(sc_module_name name) : sc_module(name) {
			SC_METHOD(_executions);
			dont_initialize();
			sensitive<<_aluEnable.pos();
		};

	private:
		sc_uint<16> const _dataMAX = 0xFFFF;
		sc_uint<3> const _overflowBit = 0x0007;
		sc_uint<3> const _carryBit = 0x0005;
		sc_uint<3> const _zeroBit = 0x0004;
		sc_uint<3> const _negativeBit = 0x0001;
		sc_uint<8> _PSR;

				bool _checkCondition () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkCondition--------"<<endl;
			bool res = false;
			switch(_condControlSig.read()) {
				case 0b0000: // EQ
					if(_PSR.range(_zeroBit , _zeroBit) == 1) {
						res = true;
					}
					break;
				case 0b0001: // NE
					if(_PSR.range(_zeroBit , _zeroBit) ==  0) {
						res = true;
					}
					break;
				case 0b0010: // GE
					if(_PSR.range(_zeroBit , _zeroBit) ==  1 && _PSR.range(_negativeBit , _negativeBit) ==  1) {
						res = true;
					}
					break;
				case 0b0011: // CS
					if(_PSR.range(_carryBit , _carryBit) ==  1) {
						res = true;
					}
					break;
				case 0b0100: // CC
					if(_PSR.range(_carryBit , _carryBit) ==  0) {
						res = true;
					}
					break;
				case 0b0101: // GT
					if(_PSR.range(_negativeBit , _negativeBit) ==  1) {
						res = true;
					}
					break;
				case 0b0110: // LE
					if(_PSR.range(_negativeBit , _negativeBit) ==  0) {
						res = true;
					}
					break;
				case 0b0111: // FS
					if(_PSR.range(_overflowBit , _overflowBit) ==  1) {
						res = true;
					}
					break;
				case 0b1000: // FC
					if(_PSR.range(_overflowBit , _overflowBit) ==  0) {
						res = true;
					}
					break;
				case 0b1001: // LT
					if(_PSR.range(_zeroBit , _zeroBit) ==  0 && _PSR.range(_negativeBit , _negativeBit) ==  0) {
						res = true;
					}
					break;
				case 0b1010: // US
					res = true;
					break;
			}
			cout<<"@ "<<sc_time_stamp()<<"------End _checkCondition--------"<<endl;
			return res;
		}

		void _executions () { 
			cout<<"@ "<<sc_time_stamp()<<"------Start _executions--------"<<endl;
			switch(_aluControlSig.read()) {
				case 0b0000 : // ADD & ADDI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1 + _operand2);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
 					break;
				case 0b0001 : // SUB & SUBI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_operand1 < _operand2) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_resultOut.write(abs(_operand1 - _operand2));
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break;
				case 0b0010 : // CMP & CMPI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_operand1 < _operand2) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_resultOut.write(abs(_operand1 - _operand2));
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(!_resultOut) {
						_PSR.range(_zeroBit , _zeroBit) = 1;
					}
					break; 
				case 0b0011 : // AND & ANDI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1 & _operand2);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0b0100 : // OR & ORI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1 | _operand2);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0b0101 : // XOR & XORI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1 ^ _operand2);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0b0110 : // MOV & MOVI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1);
					break; 
				case 0b0111 : // LSH & LSHI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1 << _operand2);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0b1000 : // ASH & ASHI
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1 >> _operand2);
					if(_resultOut >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0b1001 : // LUI & LOAD
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand2);
					break; 
				case 0b1010: // STOR
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_resultOut.write(_operand1);
					break; 
				case 0b1011 : // Bcond
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_checkCondition()) {
						_resultOut.write(_operand1 + _operand2);
					}
					break; 
				case 0b1100 : // Jcond
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_checkCondition()) {
						_resultOut.write(_operand1);
					}
					break; 
				case 0b1101 : // JAL 
					if(_operand1.read() >= _dataMAX || _operand2.read() >= _dataMAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
			}
			cout<<"@ "<<sc_time_stamp()<<"------End _executions--------"<<endl;
		}
};

#endif