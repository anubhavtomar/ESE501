/*
Phase2
	|
Control Path	Data Path
		PC	-->	PM 
				DM <- RF/EXE(ALU)










*/

// RF Module
// Addresses are coming from Control Path
// port definitions
clock
address1	4 bits	sc_uint<4>
address2	4 bits
address2	4 bits
sc_out	data1 , data2	16 bits
sc_in	data3	16 bits

// ALU

// inputs
control signals

// output
results
PSR


// Project hints end
// ------------------

// Parameterizing Module

#DEFINE A 3
template <int size>
SC_MODULE(generic_add){
	sc_in<sc_uint<size>> a;
	sc_out<bool> z
	void pre_generic_add();
	SC_CTOR(generic_add) {
		SC_METHOD(pre_generic_add);
		sensitive<<a;
	}
}

// differnet style of defining modules

SC_MODULE(name) {
	// ports
	SC_HASP_ROCESS(name);

	name(sc_module_name name , int parameters  : sc_module(name) , _var){

	}
}

name myname("module" , 32); //creating instance of name


// EX: FIR Filter design (# of taps , coefficient values are not known)


template <class T , unsigned N> class fir : public sc_module // => tmeplate<class T , unsigned N>
															//     SC_MODULE(fir){}

public:
	sc_in<bool> clock;
	sc_in<T> in;
	sc_out<T> out;
	SC_HAS_PROCESS(fir);

	fir(SC_MODULE_NAME name , const T* coeffs : sc_module(name) , _coeffs(coeffs)) {
		SC_METHOD(prc_fir);
		sensitive<<clock.pos();

		for(i = 0 ; i < N ; i++) {
			_delay_line[i] = 0;
		}
	}

private:
	T _delay_line[N];
	const T* _coeffs;
	void prc_fir(){}


// Example: instantiating fir

int sc_main(int argc , char* argv[]) {
	typefef double fir_T; // in project 1 use unsigned int instead of double
	const fir_T coeffs[ = {11 , 22 , 33 ......}]
	const unsinged taps = sizeof(coeffs)/sozeof(coeffs[0]);

	fir<fir_T , taps> myfir("fir" , &coeffs[0]); // in project 1 use program memory  data memory instead of coeffs like pm<size> mypm("name" , &pm[0])
}


// example next_trigger() -> on;y used in only SC_METHOD

sc_event e1 , e2;
SC_MODULE(example) {
	//ports
	...
		SC_METHOD(prc)
		sensitive<<clock.pos(); // 1 nsec clock
	...
}

void example::prc() {
	...
	next_trigger(e1);
	...
	next_trigger(10 , SC_NS);
}

