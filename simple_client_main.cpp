#include "Client.h"

#define REMOTE2

extern "C" int calculate(char *, char*);

int main ( int argc, int argv[] ) {
#ifndef REMOTE
  char a[1],b[1];
  return calculate(a,b);
#endif
  std::string client_ip = "127.0.0.1";
  int client_port = 9876;
  std::string client_number;

  try 
  {

    std::string reply;
    std::stringstream client_port_string;

    client simple_client(client_ip, client_port, "limes");
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
          //std::cout << data.length() << "\n";

          //Temporary
          std::cout << "Processing.\n";
          char result[10000];
          calculate(const_cast<char *>(data.c_str()), result);
          std::cout << "Done.\n";

          simple_client.send_data(result);
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
