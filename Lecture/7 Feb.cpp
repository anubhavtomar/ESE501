#include"systemc.h"

sc.module(driver) {
	dc_out<bool> a,b,c;
	void pre_driver();
	sc_int<unsigned int> temp = 0;
	SC_CTOR(driver){
		SC_THREAD(driver);
	}
}

void driver::pre_driver{
	while(1){
		a=temp[0];
		b=temp[1];
		c=temp[2];
		temp++;
		wait(1 , SC_NS);
	}
}


/*
	Driver ->a 		a->Adder
		   ->b      b->
		   ->c		cin->

*/

/*
main program 
include everything
*/

#include"out.h"
#include"monitor.h"

int sc_main(int argc , char* argv[]){
	// PORT DECLARATION
	sc_signal<bool> a,b,c;
	driver my_driver("d");
	adder my_adder("a");
	sc_trace_file *tfile = sc_create_vcd_trace_file(); // same as FILE *fp = open(".txt");
	sc_trace(tfile , a);
	sc_trace(tfile , b);
	my_driver.a(a);
	my_driver.b(b);
	my_driver.c(c);
	my_adder.a(a);
	my_adder.b(b);
	my_adder.cin(c);

	sc_start(-1);
	sc_start(100,SC_NS);
	sc_close_vcd_trace_file(tfile);
	return 0;
}


/*
Time Unit
*/
SC_FS , SC_PS , SC_NS , SC_US , SC_MS , SC_SEC

wait(5);
wait(5, SC_PS);

sc_time t1,t2(21 , SC_NS);
sc_set_time_resolution(t2);
sc_time y = sc_get_time_resolition();

sc_clock mc("mc" , 20 , SC_NS , 0.5 , 2 , SC_NS , FALSE);

sc_start(mc);