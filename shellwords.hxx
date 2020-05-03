#include <string>
#include <vector>

namespace shellwords {

std::vector<std::string> shellsplit(const std::string& command);

std::string shellescape(std::string value);

std::string shelljoin(const std::vector<std::string>& values);

} // namespace shellwords
