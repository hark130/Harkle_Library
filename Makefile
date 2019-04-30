CC = gcc
HL_HDR = hdr/
HL_SRC = src/
HL_BLD = build/

Harklecurse:
	$(CC) -c -o $(HL_BLD)Harklecurse.o -I $(HL_HDR) -c $(HL_SRC)Harklecurse.c

Harklemath:
	$(CC) -c -o $(HL_BLD)Harklemath.o -I $(HL_HDR) -c $(HL_SRC)Harklemath.c

Randoroad:
	$(CC) -c -o $(HL_BLD)Randoroad.o -I $(HL_HDR) -c $(HL_SRC)Randoroad.c

all:
	$(MAKE) Harklecurse
	$(MAKE) Harklemath
	$(MAKE) Randoroad

clean: 
	rm -f *.o *.exe *.so
	rm -f $(HL_BLD)*.o $(HL_BLD)*.exe $(HL_BLD)*.so