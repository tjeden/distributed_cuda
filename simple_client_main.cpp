#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main ( int argc, int argv[] ) {
  std::string client_ip = "127.0.0.1";
  std::string client_port = "9999";
  std::string client_number;

  try {
    ServerSocket server ( 30000 );
      while ( true ){
        ServerSocket new_sock;
        server.accept ( new_sock );
        try {
          while ( true ) {
            std::string data;
            new_sock >> data;
            new_sock << data;
          }
        }
        catch ( SocketException& ) {}
      }

      ClientSocket client_socket ( "localhost", 5555 );

      std::string reply;

      try
        {
          client_socket << "REGISTER " << client_ip << " " << client_port << " foo";
          client_socket >> reply;
        }
      catch ( SocketException& ) {}

      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";;
      client_number = reply;

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}
