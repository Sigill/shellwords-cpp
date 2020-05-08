#include <string>
#include <vector>
#include <memory>

namespace shellwords {

namespace details {

class shell_splitter_impl;

} // namespace details

class shell_splitter {
private:
  std::unique_ptr<details::shell_splitter_impl> parser_impl;

public:
  shell_splitter(std::string::const_iterator begin, std::string::const_iterator end);
  ~shell_splitter();

  bool read_next();

  bool done() const;

  const std::string& arg() const;

  std::string::const_iterator suffix() const;
};

std::vector<std::string> shellsplit(const std::string& command);

std::string shellescape(std::string value);

std::string shelljoin(const std::vector<std::string>& values);

} // namespace shellwords
