// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <asio.hpp>
#include <iostream>

using asio::error_code;
using asio::ip::tcp;

#define NOW std::chrono::system_clock::now()

typedef std::chrono::system_clock::time_point timetype;

auto constexpr ENDLINE = '\n';

template <typename StrType1, typename StrType2, typename StrType3>
void console_log(const StrType1& username, const StrType2& query,
                 const StrType3& reply) {
  std::cout << "[USER] " << username << std::endl
            << "[QUERY] " << query << "[REPLY] " << reply << std::endl
            << std::endl;
}

class Client {
  tcp::socket socket_;
  const std::string username_;
  timetype last_query_time_;
  std::chrono::seconds delay_;
  inline void reset_delay();
 public:
  inline std::chrono::seconds delay() const { return delay_; }
  [[nodiscard]] inline bool is_opened() const { return socket_.is_open(); }

  Client(asio::io_service&, const tcp::endpoint& ep, std::string username);

  void login(error_code& error);

  void query(std::string const& query, error_code& error);

  void close();

  inline timetype last_query_time() { return last_query_time_; }
};

#endif  // INCLUDE_HEADER_HPP_
