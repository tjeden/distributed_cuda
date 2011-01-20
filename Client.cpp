#include "Client.h"

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

  client_socket << "REGISTER " << ip << " " << "9876" << " " << task;
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
