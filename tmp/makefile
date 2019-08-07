cc = g++
obj += dataset.o variable.o function_base.o pdf_base.o nll.o mn_helper.o gaus_func.o gaus_pdf.o
#obj += dataset.o variable.o function_base.o pdf_base.o nll.o gaus_func.o gaus_pdf.o
flag += $(shell root-config --cflags)
libs += $(shell root-config --libs)
#libs += -lMathCore

test: test.cpp $(obj)
	$(cc) $(flag) $(libs) $(obj) test.cpp -o test

dataset.o: dataset.h dataset.cpp
	$(cc) $(flag) $(libs) dataset.cpp -c -o dataset.o

variable.o: variable.h variable.cpp
	$(cc) $(flag) $(libs) variable.cpp -c -o variable.o

function_base.o: function_base.h function_base.cpp
	$(cc) $(flag) $(libs) function_base.cpp -c -o function_base.o

pdf_base.o: pdf_base.h pdf_base.cpp
	$(cc) $(flag) $(libs) pdf_base.cpp -c -o pdf_base.o

nll.o: nll.h nll.cpp
	$(cc) $(flag) $(libs) nll.cpp -c -o nll.o

mn_helper.o: mn_helper.h mn_helper.cpp
	$(cc) $(flag) $(libs) mn_helper.cpp -c -o mn_helper.o

gaus_func.o: gaus_func.h gaus_func.cpp
	$(cc) $(flag) $(libs) gaus_func.cpp -c -o gaus_func.o

gaus_pdf.o: gaus_pdf.h gaus_pdf.cpp
	$(cc) $(flag) $(libs) gaus_pdf.cpp -c -o gaus_pdf.o
