#include <Client.hpp>

int main() {
  srand(time(0));

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
  size_t max_clients = 8;

  error_code error;

  while (clients.size() < max_clients) {
    clients.emplace_back(io_service, endpoint,
                         "User_" + std::to_string(rand() % 1000));
    clients.back().login(error);
  }

  std::cout << "-------[ USERS LOGGED IN ]-------" << std::endl;

  while (std::count_if(clients.begin(), clients.end(),
                       [](Client const& c) { return c.is_opened(); })) {
    for (auto& client : clients) {
      if (NOW - client.last_query_time() >= client.delay() &&
          client.is_opened()) {
        std::cout << "delay = "
                  << client.delay().count()
                  << " sec" << std::endl;
        client.query("ping\n", error);
      }
    }
  }

  return 0;
}