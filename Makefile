CFLAGS = -Wall -g -O0 -I ${TBB20_INSTALL_DIR}/include
TESTCFLAGS = -D_TEST
LIBS = -L ${TBB20_INSTALL_DIR}/ia32/cc4.1.0_libc2.4_kernel2.6.16.21/lib -ltbb -lm 

all: clean tags NashTest Nash runtest

test: NashTest

clean:
	-rm *.o Nash ./Test/NashTest tags

tags:
	ctags -R .

runtest:
	./Test/NashTest

Nash: Nash.o
	g++ ${CFLAGS} Nash.o -o Nash ${LIBS}

NashTest: ./Test/NashTest.o
	g++ ${CFLAGS} ${TESTCFLAGS} ./Test/NashTest.o -o ./Test/NashTest ${LIBS} -l cppunit

Nash.o:
	g++ ${CFLAGS} Nash.cpp -c

./Test/NashTest.o:
	g++ ${CFLAGS} ${TESTFLAGS} ./Test/NashTest.cpp -c
