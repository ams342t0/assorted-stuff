# VS C/C++ Compiler
CL=cl
LINK = link

# Optimization
FLAGS =   /O2 

# Code generation
FLAGS += /fp:fast
FLAGS += /c /FAs
FLAGS += /EHsc

MVS     =  C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.23.28105
WINKIT  =  C:\Program Files (x86)\Windows Kits\10
INCLUDE =  /I"$(MVS)\INCLUDE"
INCLUDE += /I"$(WINKIT)\INCLUDE\10.0.18362.0\ucrt"
INCLUDE += /I"$(WINKIT)\INCLUDE\10.0.18362.0\um"
INCLUDE += /I"$(WINKIT)\INCLUDE\10.0.18362.0\shared"
LIB     =  /LIBPATH:"$(MVS)\lib\onecore\x64"
LIB     += /LIBPATH:"$(WINKIT)\LIB\10.0.18362.0\um\x64"
LIB		+= /LIBPATH:"$(WINKIT)\LIB\10.0.18362.0\ucrt\x64"
LIBF    =  winmm.lib
DEFS 	=  /D UNICODE /D _WINDOWS


all: main.obj timer2.exe dlg.o


dlg.o : winres.rc
		windres winres.rc -o dlg.o

main.obj : main.cpp
	$(CL) $^ $(DEFS) $(FLAGS) $(INCLUDE)
	
timer2.exe : main.obj
	$(LINK) $(LIB) $(LIBF) dlg.o $^ /machine:x64 /out:$@