*********************************************************
**************    Connection Oriented    ****************
*********************************************************

-------Iterative server------

1.The client connects to the server and sends name of the file it wants.
2.Server receives the file name and checks if the file is available.
3.If the file is available, server will read the contents of the file in a buffer and send it in response to the client.
4.Client saves the received content in a locally created "newfile.txt".
5.Server repeatedly reads a request from the client, formulates response and serves requests of the client.

Steps to execute the program:

On Terminal window 1:

Execute Makefile using make command.
This will generate executable files TCP_iterative_server.o and TCP_iterative_client.o.
Execute TCP_iterative_server.o file using:

./TCP_iterative_server.o

On Terminal window 2

Execute TCP_iterative_client.o file using:

$./TCP_iterative_client.o  <filename>




-----------Concurrent Multiprocessing server with one process per request---------

1.The client sends connection request to the server along with the file name it wants to read from.
2.Server accepts the connection request and creates a child process to serve the client.
3.The child process checks if the requested file is available on the server.
4.If file is available, it will read the contents of the file in a buffer and send it as a response to the client.
5.Client saves the received content in a locally created "newfile.txt".
6.Child processes are created concurrently as server receives requests from the client.
7.Server repeatedly receives a request from the client, creates a child process to formulate responses and serves request of the client.


Steps to execute the program:
On Terminal window 1

Execute Makefile using make command.
This will generate executable files TCP_fork_server.o and TCP_fork_client.o.
Execute TCP_fork_server.o file using 

./TCP_fork_server.o

On Terminal window 2,
1.Execute TCP_fork_client.o file using 

./TCP_fork_client.o <filename>



-------------Concurrent Multithreading server with one thread per request------------

1.The client sends connection request to the server along with the filename it wants to read from.
2.Server accepts the connection request and creates a thread to serve the client.
3.The newly created thread checks if the requested file is available on the server.
4.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
5.Client saves the received content in a locally created "newfile.txt".
6.Threads are created concurrently as server receives requests from the client.
7.Server repeatedly reads a request from the client, creates a thread to formulate responses and serve the request of the client.

Steps to execute the program:

On Terminal window 1

Execute Makefile using make command.
This will generate executable files TCP_multithread_server.o and TCP_multithread_client.o.
Execute TCP_multithread_server.o file using:

./TCP_multithread_server.o

On Terminal window 2

1.Execute TCP_multithread_client.o file using:

./TCP_multithread_client.o <filename>



----------Concurrent Pre-forked Multiprocessing server-----------

1.The server creates pool of child processes when it starts.
2.The number of child processes to be created is passed as a command line argument to the server. Number of process created will be equal to 2^n, where n is the number of command line argument.
3.The client sends connection request to the server along with the file name it wants to read from.
4.Server accepts the connection request and checks if it has a process available to serve the client.
5.The assigned process then checks if the requested file is available.
6.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
7.Client saves the received content in a locally created "newfile.txt".
9.Server repeatedly reads a request from the client, formulates responses and serve the request of the client unless it runs out of processes.

Steps to execute the program:

On Terminal window 1,
Execute Makefile using make command.
This will generate executable files TCP_preforked_server.o and TCP_preforked_client.o
Execute TCP_preforked_server.o file using:

./TCP_preforked_server.o <no of process>

On Terminal window 2,
   1.Execute TCP_preforked_client.o file using 

./ TCP_preforked_client.o <filename>



----------Concurrent Pre-threaded Multithreading server----------

1.The server creates pool of thread when it starts.
2.The number of threads to be created is passed as a command line argument to the server.
3.The client sends connection request to the server along with the file name it wants to read from.
4.The main thread at server side accepts the connection and passes each client connection to one of the available thread.
5.The assigned thread then checks if the requested file is available.
6.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
7.Client saves the received content in a locally created "newfile.txt".
9.Server repeatedly reads a request from the client, formulates responses and serve the request of the client unless it runs out of threads.

Steps to execute the program:

On Terminal window 1

Execute Makefile using make command.
This will generate executable files TCP_prethreaded_server.o and TCP_prethreaded_client.o
Execute TCP_prethreaded_server.o file using 

./ TCP_prethreaded_server.o <no_of_threads>

On Terminal window 2

1.Execute TCP_prethreaded_client.o file using

 ./ TCP_prethreaded_client.o <filename>



**************************************************
*************    Connectionless    ***************
**************************************************

----------Iterative server------------
1.The client connects to the server and sends name of the file it wants to read from.
2.Server receives the file name and checks if the file is available.
3.If the file is available, server will read the contents of the file in a buffer and send it in response to the client.
4.Client saves the received content in a locally created "newfile.txt".
5.Server repeatedly reads a request from the client, formulates response and serves requests of the client.

Steps to execute the program:

On Terminal window 1

Execute Makefile using make command.
This will generate executable files UDP_iterative_server.o and UDP_iterative_client.o
Execute UDP_iterative_server.o file using 

./ UDP_iterative_server.o

On Terminal window 2

1.Execute UDP_iterative_client.o file using 

./ UDP_iterative_client.o <filename> 



----------Concurrent Multiprocessing server with one process per request-----------

1.The client sends connection request to the server along with the file name it wants to read from.
2.Server accepts the connection request and creates a child process to serve the client.
3.The child process checks if the requested file is available on the server.
4.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
5.Client saves the received content in a locally created "newfile.txt".
6.Child processes are created concurrently as server receives requests from the client.
7.Server repeatedly reads a request from the client, creates a child process to formulate responses and serves request of the client.

Steps to execute the program:

On Terminal window 1
Execute Makefile using make command.
This will generate executable files UDP_fork_server.o and UDP_fork_client.o
Execute UDP_fork_server.o file using 

./ UDP_fork_server.o

On Terminal window 2

1.Execute UDP_fork_client.o file using

./ UDP_fork_client.o <filename>



---------Concurrent Multithreading server with one thread per request----------

1.The client sends connection request to the server along with the file name it wants to read from.
2.Server accepts the connection request and creates a thread to serve the client.
3.The newly created thread checks if the requested file is available on the server.
4.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
5.Client saves the received content in a locally created "newfile.txt".
6.Threads are created concurrently as server receives requests from the client.
7.Server repeatedly reads a request from the client, creates a thread to formulate responses and serve the request of the client.

Steps to execute the program:

On Terminal window 1

Execute Makefile using make command.
This will generate executable files UDP_multithread_server.o and UDP_multithread_client.o
Execute multithread_server.o file using 

./ multithread_server.o 

On Terminal window 2

1.Execute UDP_multithread_client.o file using

 ./ UDP_multithread_client.o <filename>



------------Concurrent Pre-forked Multiprocessing server-------------

1.The server creates pool of child processes when it starts.
2.The number of child processes to be created is passed as a command line argument to the server.
3.The client sends connection request to the server along with the file name it wants to read from.
4.Server accepts the connection request and checks if it has a process available to serve the client.
5.The assigned process then checks if the requested file is available.
6.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
7.Client saves the received content in a locally created "newfile.txt".
9.Server repeatedly reads a request from the client, formulates responses and serve the request of the client unless it runs out of processes.

Steps to execute the program:

On Terminal window 1,
Execute Makefile using make command.
This will generate executable files UDP_preforked_server.o and UDP_preforked_client.o
Execute UDP_preforked_server.o file using

./ UDP_preforked_server.o <no_of_process>

On Terminal window 2

1.Execute UDP_preforked_client.o file using 

./ UDP_preforked_client.o <filename>



----------Concurrent Pre-threaded Multithreading server------------

1.The server creates pool of thread when it starts.
2.The number of threads to be created is passed as a command line argument to the server.
3.The client sends connection request to the server along with the file name it wants to read from.
4.The main thread at server side accepts the connection and passes each client connection to one of the available thread.
5.The assigned thread then checks if the requested file is available.
6.If file is available, it will read the contents of the file in a buffer and send it in response to the client.
7.Client saves the received content in a locally created "newfile.txt".
9.Server repeatedly reads a request from the client, formulates responses and serve the request of the client unless it runs out of threads.

Steps to execute the program:

On Terminal window 1

Execute Makefile using make command.
This will generate executable files UDP_prethreaded_server.o and UDP_prethreaded_client.o
Execute UDP_prethreaded_server.o file using

./ UDP_prethreaded_server.o  <no_of_threads>

On Terminal window 2

1.Execute UDP_prethreaded_client.o file using

./ UDP_prethreaded_client.o <filename>


