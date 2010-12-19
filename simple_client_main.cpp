#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

extern "C" int calculate(const char *, char*);

int main ( int argc, int argv[] ) {
  std::string client_ip = "127.0.0.1";
  int client_port = 9876;
  std::string client_number;

  try 
  {
    std::string reply;
    std::stringstream client_port_string;
    std::string buff;

    ClientSocket client_socket ( "localhost", 5555 );

    client_port_string << client_port;
    std::cout << "Update?\n";
    client_socket << "REGISTER " << client_ip << " " << client_port_string.str() << " vector_add";
    client_socket >> reply;

    client_number = reply;
    std::cout << "We received this response from the server:\n" << reply << "\n";;

    std::cout << "Starting listener" << std::endl;
    ServerSocket server (client_port);
    std::cout << "Listener started" << std::endl;
    while ( true )
    {
      ServerSocket new_sock;
      std::cout << "Socket accepted\n";
      try 
      {
        while ( true ) 
        {
          server.accept ( new_sock );
          std::string data = "";
          char result[10];
          //std::cout << "1\n";
          new_sock >> data;
          std::cout << data.length() << "\n";
          //std::cout << "2\n";
          new_sock << "OK";
          new_sock.close();
          //std::cout << "3\n";
          std::cout << "Processing.\n";
          calculate(data.c_str(), result);
          //result[0] = 'd';
          std::cout << "Done.\n";
          buff = "RESPONSE " + client_number + " " + result;
          client_socket << buff;
          //std::cout << "4\n";
          client_socket >> reply;
          //std::cout << "5\n";
        }
      }
      catch ( SocketException& e) 
      {
        std::cout << "Exception was caught:" << e.description() << "\n";
      }
    }
  }
  catch ( SocketException& e)
  {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }
  return 0;
}
