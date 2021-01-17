// Copyright 2020 Your Name <your_email>

#include <Client.hpp>
#include <utility>
Client::Client(asio::io_service& service, const tcp::endpoint& endpoint,
               std::string  username)
    : socket_(service), username_(std::move(username)) {
  socket_.connect(endpoint);
}

void Client::login(error_code& error)  {
  std::string query = "login " + username_ + ENDLINE;
  socket_.write_some(asio::buffer(query));
  last_query_time_ = NOW;

  asio::streambuf reply;
  asio::read_until(socket_, reply, ENDLINE, error);

  std::string reply_str;
  std::istream input(&reply);
  std::getline(input, reply_str);

  console_log(username_, query, reply_str);
}

void Client::query(std::string const& query, error_code& error) {
  socket_.write_some(asio::buffer(query));
  last_query_time_ = NOW;

  asio::streambuf reply;
  asio::read_until(socket_, reply, ENDLINE, error);

  std::string reply_str;
  std::istream input(&reply);
  std::getline(input, reply_str);

  console_log(username_, query, reply_str);

  if (reply_str == "Disconnected by timeout"){
    close();
    return;
  }

  if (reply_str == "client_list_changed"){
    this->query("clients\n", error);
  }
}
void Client::close() {
  socket_.close();
}
