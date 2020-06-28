psishell: psishell.o shellHistory.o rawmode.o
	gcc -o psishell psishell.o shellHistory.o rawmode.o

psishell.o: psishell.c psishell.h
	gcc -c psishell.c

shellHistory.o: shellHistory.c psishell.h
	gcc -c shellHistory.c

rawmode.o: rawmode.c psishell.h
	gcc -c rawmode.c

clean:
	rm -rvf shellHistory.o psishell.o rawmode.o

reset:
	rm -rvf shellHistory.o psishell.o rawmode.o psishell
