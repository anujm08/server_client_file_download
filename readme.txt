Experimental Setup :
	We used two laptops connected via ethernet cables through a D-Link router to act as server and client.
	The specifications of the systems and network link is mentioned in the report.

Compiling the code :
	We have provided a makefile to compile the server and client executables
	Writing 'make' in the submitted directory like compile the code and 
	generate the executables 'server-mp' and 'multi-client'

Running the server :
	./server-mp <port>

Running the client :
	./multi-client <server-ip> <port> <num-threads> <run-time> <sleep-time> <mode>
