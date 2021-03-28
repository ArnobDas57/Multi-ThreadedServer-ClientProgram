#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace Sync;

// THis thread handles each client connection
class SocketThread : public Thread
{
   private:
   // Reference to our connected socket
   Socket& socket;
   
   // The data that will be received
   ByteArray data;
 	
   public:
   SocketThread(Socket& socket)
   : socket(socket){}
   
   ~SocketThread(){}
   
   Socket& GetSocket()
   {
   	return socket;
   }
   
   virtual long ThreadMain()
   {
   	while(1)
   	{
   	   try
   	   {
   	   	// Wait for data
   	   	socket.Read(data);
   	   	
   	   	// Perform operations on the data
   	   	std::string data_str = data.ToString();
   	   	std::reverse(data_str.begin(), data_str.end());
   	   	data = ByteArray(data_str);
   	   	
   	   	// Send it back
     		socket.Write(data);
     	   }
 		
 	   catch (...){}
	}

	return 0;
   }
};


// This thread handles the server operations
class ServerThread : public Thread
{
private:
    SocketServer& server;
    bool terminate = false;
public:
    SocketThread* socketThread;

    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {}

    virtual long ThreadMain()
    {
    	while(true)
    	{
        // Wait for a client socket connection
        Socket* newConnection = new Socket(server.Accept());

        // A reference to this pointer 
        Socket& socketReference = *newConnection;
        socketThread = new SocketThread(socketReference);
	}
	return 1;
    }
};


int main(void)
{
    std::cout << "I am a server." << std::endl;
    std::cout << "Press enter to terminate the server..." << std::endl;
    
    // Create our server
    SocketServer server(3000);    

    // Need a thread to perform server operations
    ServerThread serverThread(server);
    
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    std::cin.get();

    // Shut down and clean up the server
    server.Shutdown();
}
