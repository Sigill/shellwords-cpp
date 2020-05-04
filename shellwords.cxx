#include "shellwords.hxx"

#include <regex>
#include <numeric>
#include <iterator>
#include <stdexcept>

namespace shellwords {

std::vector<std::string> shellsplit(const std::string& command) {
  std::vector<std::string> words;

  std::string field;

  auto re = std::regex(R"EOR(\s*(?:([^\s\\'"]+)|'([^\']*)'|"((?:[^\"\\]|\\.)*)"|(\\.?)|(\S))(\s|$)?)EOR");

  auto i = std::sregex_iterator(command.begin(), command.end(), re);
  auto words_end = std::sregex_iterator();

  for (; i != words_end; ++i) {
    std::smatch match = *i;

    if (match[5].matched) // garbage
      throw std::runtime_error("Unmatched double quote: " + command);

    if (match[1].matched) // word
      field += match[1].str();
    else if (match[2].matched) // sq
      field += match[2].str();
    else if (match[3].matched) { // qd
      auto re2 = std::regex(R"EOR(\\([$`"\\\n]))EOR");
      field += std::regex_replace(match[3].str(), re2, "$1");
    } else { // esc
      auto re3 = std::regex(R"EOR(\\(.))EOR");
      field += std::regex_replace(match[4].str(), re3, "$1");
    }

    if (match[6].matched) {
      words.push_back(field);
      field.clear();
    }
  }

  return words;
}

std::vector<std::wstring> shellsplit(const std::wstring& command) {
  std::vector<std::wstring> words;

  std::wstring field;

  auto re = std::wregex(LR"EOR(\s*(?:([^\s\\'"]+)|'([^\']*)'|"((?:[^\"\\]|\\.)*)"|(\\.?)|(\S))(\s|$)?)EOR");

  auto i = std::wsregex_iterator(command.begin(), command.end(), re);
  auto words_end = std::wsregex_iterator();

  for (; i != words_end; ++i) {
    std::wsmatch match = *i;

    if (match[5].matched) // garbage
      throw std::runtime_error("Unmatched double quote");

    if (match[1].matched) // word
      field += match[1].str();
    else if (match[2].matched) // sq
      field += match[2].str();
    else if (match[3].matched) { // qd
      auto re2 = std::wregex(LR"EOR(\\([$`"\\\n]))EOR");
      field += std::regex_replace(match[3].str(), re2, L"$1");
    } else { // esc
      auto re3 = std::wregex(LR"EOR(\\(.))EOR");
      field += std::regex_replace(match[4].str(), re3, L"$1");
    }

    if (match[6].matched) {
      words.push_back(field);
      field.clear();
    }
  }

  return words;
}

std::string shellescape(std::string value) {
  if (value.empty()) {
    value = "''";
  } else {
    std::regex re1 = std::regex("[^A-Za-z0-9_\\-.,:+\\/@\n]");
    value = std::regex_replace(value, re1, "\\$&");
    std::regex re2 = std::regex("\n");
    value = std::regex_replace(value, re2, "'\n'");
  }

  return value;
}

std::wstring shellescape(std::wstring value) {
  if (value.empty()) {
    value = L"''";
  } else {
    auto re1 = std::wregex(L"[^A-Za-z0-9_\\-.,:+\\/@\n]");
    value = std::regex_replace(value, re1, L"\\$&");
    auto re2 = std::wregex(L"\n");
    value = std::regex_replace(value, re2, L"'\n'");
  }

  return value;
}

std::string shelljoin(const std::vector<std::string>& values) {
  std::string result;

  if (!values.empty())
    result = std::accumulate(std::next(values.begin()),
                             values.end(),
                             values.front(),
                             [](std::string accum, const std::string& value){ return std::move(accum) + " " + shellescape(value); });

  return result;
}

std::wstring shelljoin(const std::vector<std::wstring>& values) {
  std::wstring result;

  if (!values.empty())
    result = std::accumulate(std::next(values.begin()),
                             values.end(),
                             values.front(),
                             [](std::wstring accum, const std::wstring& value){ return std::move(accum) + L" " + shellescape(value); });

  return result;
}

} // namespace shellwords
