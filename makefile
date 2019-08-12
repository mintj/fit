cc = g++
flag += $(shell root-config --cflags)
libs += $(shell root-config --libs)

%: %.cpp %.h
	$(cc) $(flag) $(libs) $< -c -o $*.o
