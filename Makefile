all: client server

client: utils.h myqueue.h client.c myqueue
	gcc -Wall client.c myqueue.o -oclient -D_SVID_SOURCE -std=c99

server: utils.h myqueue.h server.c
	gcc -Wall server.c myqueue.o -oserver -D_SVID_SOURCE -std=c99

myqueue: utils.h myqueue.h myqueue.c
	gcc -Wall -c myqueue.c -omyqueue.o -D_SVID_SOURCE -std=c99

clean:
	rm client
	rm server
	rm myqueue.o
