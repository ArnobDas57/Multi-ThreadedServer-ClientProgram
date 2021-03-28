
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

// This thread handles the connection to the server
class ClientThread : public Thread
{
   private:
   // Reference to our connected socket
   Socket& socket;
   
   // Data to send to server
   ByteArray data;
   std::string data_str;
   
   public:
   bool flag = false;
   
   ClientThread(Socket& socket):socket(socket)
   {}
   
   ~ClientThread()
   {}
   
   virtual long ThreadMain()
   {
   	try{		
   	       while(data_str != "done")
   	       {
		   	std::cout << "Please enter some text (to exit, enter done): ";
		   	
		   	// Get the data
		   	std::getline(std::cin, data_str);
		   	
		  	if(data_str != "done")
		  	{
		  		data = ByteArray(data_str);
			  	// Write to the server
			   	socket.Write(data);
			   	
			   	// Get the response
			   	int flag2 = socket.Read(data);
			 
			 	if(flag2 != 0)
			 	{
			 
			   	data_str = data.ToString();
			   	std::cout << "Server Response: " << data_str << std::endl; 
			   	}
			   	
			   	else
			   	{
			   	   break;	
			   	}
		  	}  	
	       } 
	       
	       flag = true;
	}
	   
	catch(...){}

	return 0; 
   }
};
   	
int main(void)
{
	// Welcome the user 
	std::cout << "SE3313 Lab 3 Client" << std::endl;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
	socket.Open();
	
	ClientThread clientThread(socket);
	
	while(true)
	{
	   sleep(1);
	   
	   if(clientThread.flag)
	   {
	      break;
	   }
	}
	
	socket.Close();
	
	return 0;
}
