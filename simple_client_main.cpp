#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

class client 
{
  public:
    client (std::string, int, std::string);

    std::string ip;
    std::string task;
    std::string number;
    int port;

    std::string register_in_server(void);
    std::string accept_data();
    std::string send_data(std::string);
};

client::client (std::string aIp, int aPort, std::string aTask)
{
  client::ip = aIp; 
  client::port = aPort;
  client::task = aTask;
};

std::string client::register_in_server(void)
{
  ClientSocket client_socket ( "localhost", 5555 );

  std::string reply;
  std::stringstream client_port_string;
  client_port_string << port;

  client_socket << "REGISTER " << ip << " " << client_port_string.str() << " " << task;
  client_socket >> reply;
  number = reply;

  return reply;
};

std::string client::accept_data()
{
  std::string data = "";
  ServerSocket server (port);
  ServerSocket new_sock;
  std::cout << "Socket accepted\n";
  server.accept ( new_sock );
  new_sock >> data;
  new_sock << "OK";
  new_sock.close();

  return data;
};

std::string client::send_data(std::string data)
{
  ClientSocket client_socket ( "localhost", 5555 );
  std::string reply;
  std::string buff;
  buff = "RESPONSE " + number + " " + data;
  client_socket << buff;
  client_socket >> reply;

  return reply;
}

extern "C" int calculate(const char *, char*);

int main ( int argc, int argv[] ) {
  std::string client_ip = "127.0.0.1";
  int client_port = 9876;
  std::string client_number;

  try 
  {

    std::string reply;
    std::stringstream client_port_string;

    client simple_client(client_ip, client_port, "foo");
    client_number = simple_client.register_in_server();

    std::cout << "We received this response from the server:\n" << client_number << "\n";;

    std::cout << "Starting listener" << std::endl;
    std::cout << "Listener started" << std::endl;
    while ( true )
    {
      try 
      {
        while ( true ) 
        {
          std::string data = "";
          data = simple_client.accept_data();
          std::cout << data.length() << "\n";

          //Temporary
          //std::cout << "Processing.\n";
          //char result[10];
          //calculate(data.c_str(), result);
          //result[0] = 'd';
          //std::cout << "Done.\n";

          simple_client.send_data(data);
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
