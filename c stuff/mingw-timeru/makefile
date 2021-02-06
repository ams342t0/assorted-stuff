CC    = g++
LIBS  = -lgdi32 -lcomctl32 -lcomdlg32 -lwinmm
FLAGS = -Wall -Wextra -O2 -fpermissive -mwindows
DEFS  = -DUNICODE

all: winres.o main.o timer.exe

winres.o : winres.rc
	windres $^ -o $@
	
main.o : main.cpp
	$(CC) $^ $(FLAGS) $(DEFS) -c -o $@

timer.exe : winres.o main.o
	$(CC) $^ $(LIBS) $(FLAGS) -o $@

clean:
	del /f main.o
	del /f winres.o
	del /f timer.exe