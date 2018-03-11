#CMPE207_Lab01Q3

The client sends a connection request to server1 and server2 to connect and obtain the TIME of those servers.
After the connection is established both the servers return TIME in network byte order.
The client converts the received TIME to host byte order which is then displayed.
It then calculates the difference between both the values and displays it.
Follow the below steps to run the TCP client program.
1: Use command -> cd <Directory name> ;to go to the directory where the file is stored.
2: Run the makefile using command -> make.
3: Run the command -> ./Client_Time.o to run the client side.

Test Case:

Senario: Two Daytime Servers and their corresponding time 
	 and the difference in time will be displayed by 
	 our client program. 

Actual Output: Server1 Time: Tue Oct  3 20:44:01 2017
	       Server2 Time: Tue Oct  3 20:44:01 2017
	       Difference is: 0.000000

Expected Output: Server1 Time: Tue Oct  3 20:44:01 2017
		 Server2 Time: Tue Oct  3 20:44:01 2017
		 Difference is: 0.000000

Result: Pass


