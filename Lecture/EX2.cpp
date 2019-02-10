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