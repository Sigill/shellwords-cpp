#include <string>
#include <vector>

namespace shellwords {

std::vector<std::string> shellsplit(const std::string& command);
std::vector<std::wstring> shellsplit(const std::wstring& command);

std::string shellescape(std::string value);
std::wstring shellescape(std::wstring value);

std::string shelljoin(const std::vector<std::string>& values);
std::wstring shelljoin(const std::vector<std::wstring>& values);

} // namespace shellwords
