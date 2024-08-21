all:
	gcc -c -I${INSTALL_HDR_PATH}/include -Wall -Werror -fpic utimers.c
	gcc -shared -o libutimers.so utimers.o
debug:
	gcc -c -DDEBUG -I${INSTALL_HDR_PATH}/include -Wall -Werror -fpic utimers.c
	gcc -shared -o libutimers.so utimers.o
tests: all
	gcc -L${PWD}/ -Wall -I${INSTALL_HDR_PATH}/include -o tests/test tests/test.c -lutimers
clean:
	rm -rf *.o *.so
