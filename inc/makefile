cc = g++
flag += $(shell root-config --cflags)
libs += $(shell root-config --libs)
libs += -lMinuit2

%: %.cpp %.h
	$(cc) $(flag) $(libs) $< -c -o $*.o
