/**
 * Author:    Anubhav Tomar
 * 
 * ALU Definition
 **/

#ifndef ALU_H
#define ALU_H

#include<systemc.h>

template <class _statusRegSize , class _dataSize> 
class _aluBlock : public sc_module {
	public:
		sc_in<bool> _clock;
		sc_in<sc_uint<5> > _aluControlSig;
		sc_in<sc_uint<4> > _condControlSig;
		sc_in<sc_uint<1> > _rfRead;
		sc_in<sc_uint<1> > _rfWrite;
		sc_in<sc_uint<1> > _pcWrite;
		sc_in<sc_uint<1> > _operand1;
		sc_in<sc_uint<1> > _operand2;
		sc_out<_dataSize> _result;
		sc_out<_statusRegSize> _PSROut;

		SC_HAS_PROCESS(AluBlock);

		AluBlock(SC_MODULE_NAME name : sc_module(name)) { };

	private:
		sc_uint<16> UINT_MAX = 0xFFFF;
		sc_uint<3> _overflowBit = 0x8;
		sc_uint<3> _carryBit = 0x6;
		sc_uint<3> _zeroBit = 0x4;
		sc_uint<3> _negativeBit = 0x2;
		sc_uint<_statusRegSize> _PSR;

		bool checkCondition() {
			bool res = false;
			switch(_condControlSig) {
				case 0x0000: // EQ
					if(_PSR.range(_zeroBit , _zeroBit) == 1) {
						res = true;
					}
					break;
				case 0x0001: // NE
					if(_PSR.range(_zeroBit , _zeroBit) ==  0) {
						res = true;
					}
					break;
				case 0x0010: // GE
					if(_PSR.range(_zeroBit , _zeroBit) ==  1 && _PSR.range(_negativeBit , _negativeBit) ==  1) {
						res = true;
					}
					break;
				case 0x0011: // CS
					if(_PSR.range(_carryBit , _carryBit) ==  1) {
						res = true;
					}
					break;
				case 0x0100: // CC
					if(_PSR.range(_carryBit , _carryBit) ==  0) {
						res = true;
					}
					break;
				case 0x0101: // GT
					if(_PSR.range(_negativeBit , _negativeBit) ==  1) {
						res = true;
					}
					break;
				case 0x0110: // LE
					if(_PSR.range(_negativeBit , _negativeBit) ==  0) {
						res = true;
					}
					break;
				case 0x0111: // FS
					if(_PSR.range(_overflowBit , _overflowBit) ==  1) {
						res = true;
					}
					break;
				case 0x1000: // FC
					if(_PSR.range(_overflowBit , _overflowBit) ==  0) {
						res = true;
					}
					break;
				case 0x1001: // LT
					if(_PSR.range(_zeroBit , _zeroBit) ==  0 && _PSR.range(_negativeBit , _negativeBit) ==  0) {
						res = true;
					}
					break;
				case 0x1010: // US
					res = true;
					break;
			}
			return res;
		}

		void executions() { 
			switch(_aluControlSig) {
				case 0x00000 : // ADD
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 + _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
 					break;
				case 0x00001 : //ADDI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 + _operand1;
					if(_result >= UINT_MAX) {
						_PSR |= _overflowBit;
					}
					break; 
				case 0x00010 : // SUB
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_operand2 < _operand1) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_result = abs(_operand2 - _operand1);
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break;
				case 0x00011 : //SUBI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_operand2 < _operand1) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_result = abs(_operand2 - _operand1);
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x00100 : // CMP
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_operand2 < _operand1) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_result = abs(_operand2 - _operand1);
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(!_result) {
						_PSR.range(_zeroBit , _zeroBit) = 1;
					}
					break; 
				case 0x00101 : // CMPI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_operand2 < _operand1) {
						_PSR.range(_carryBit , _carryBit) = 1;
					} 
					_result = abs(_operand2 - _operand1);
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(!_result) {
						_PSR.range(_zeroBit , _zeroBit) = 1;
					}
					break; 
				case 0x00110 : // AND
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 & _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x00111 : // ANDI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 & _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x01000 : // OR
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 | _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x01001 : // ORI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 | _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x01010 : // XOR
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 ^ _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x01011 : // XORI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 ^ _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x01100 : // MOV
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2;
					break; 
				case 0x01101 : // MOVI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2;
					break; 
				case 0x01110 : // LSH
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 << _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x01111 : // LSHI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 << _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x10000 : // ASH
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 >> _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x10001 : // ASHI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2 >> _operand1;
					if(_result >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
				case 0x10010 : // LUI
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2;
					break; 
				case 0x10011 : // LOAD
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2;
					break;
				case 0x10100 : // STOR
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					_result = _operand2;
					break; 
				case 0x10101 : // Bcond
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_checkCondition()) {
						pc += _operand1;
					}
					break; 
				case 0x10110 : // Jcond
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					if(_checkCondition()) {
						pc = _operand1;
					}
					break; 
				case 0x10111 : // JAL
					if(_operand1 >= UINT_MAX || _operand2 >= UINT_MAX) {
						_PSR.range(_overflowBit , _overflowBit) = 1;
					}
					break; 
			}
		};
};

#endif