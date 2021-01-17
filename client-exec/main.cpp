#include <Client.hpp>

int main(int argc, char** argv) {
  asio::io_service io_service;
  asio::ip::tcp::resolver::query query("localhost", "8080");
  asio::ip::tcp::resolver resolver(io_service);
  asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);
  asio::ip::tcp::resolver::iterator end;
  asio::ip::tcp::endpoint endpoint;

  while (destination != end) {
    endpoint = *destination++;
    std::cout << endpoint << std::endl;
  }

  std::vector<Client> clients;
  size_t max_clients = 5;
  if (argc > 100) {
    max_clients = (std::stoi(argv[1]));
  }

  error_code error;

  while (clients.size() < max_clients) {
    clients.emplace_back(io_service, endpoint,
                         "User" + std::to_string(rand() % 100));
    clients.back().login(error);
  }

  std::cout << "USERS LOGGED IN" << std::endl;
  std::chrono::seconds delay;
  while (std::count_if(clients.begin(), clients.end(),
                       [](Client const& c) { return c.is_opened(); })) {
    for (auto& client : clients) {
      delay = std::chrono::seconds(2 + rand() % 10);
      if (NOW - client.last_query_time() >= delay) {
        client.query("ping\n", error);
      }
    }
  }

  return 0;
}