CC		= gcc
PROG	= scanner 

$(PROG): driveScanner.o scanner.o
	$(CC) -o $(PROG) driveScanner.o scanner.o

driveScanner.o : driveScanner.c token.h scanner.h
	$(CC) -c driveScanner.c

scanner.o : scanner.c token.h scanner.h symdef.h
	$(CC) -c scanner.c

.PHONY: clean

clean:
	/usr/bin/rm -rf core $(PROG) *.o
