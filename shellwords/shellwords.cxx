#include "shellwords/shellwords.hxx"

#include <regex>
#include <numeric>
#include <iterator>
#include <stdexcept>

namespace shellwords {

namespace details {

class shell_splitter_impl {
private:
  std::regex re ;
  std::string::const_iterator mBegin, mEnd;
  std::sregex_iterator mTokIt, mTokEnd;
  std::string mArg;
  std::string::const_iterator mSuffix;

public:
  shell_splitter_impl(std::string::const_iterator begin, std::string::const_iterator end)
    : re(R"EOR(\s*(?:([^\s\\'"]+)|'([^\']*)'|"((?:[^\"\\]|\\.)*)"|(\\.?)|(\S))(\s|$)?)EOR")
    , mBegin(begin)
    , mEnd(end)
    , mTokIt(mBegin, mEnd, re)
    , mTokEnd()
  {}

  bool read_next() {
    mArg.clear();

    if (mTokIt == mTokEnd)
      return false;

    bool read_next_word = true;

    while(read_next_word && mTokIt != mTokEnd) {
      const std::smatch& match = *mTokIt;

      if (match[5].matched) // garbage
        throw std::runtime_error("Unmatched double quote: " + std::string(mBegin, mEnd));

      if (match[1].matched) // word
        mArg += match[1].str();
      else if (match[2].matched) // sq
        mArg += match[2].str();
      else if (match[3].matched) { // qd
        std::regex re2 = std::regex(R"EOR(\\([$`"\\\n]))EOR");
        mArg += std::regex_replace(match[3].str(), re2, "$1");
      } else { // esc
        std::regex re3 = std::regex(R"EOR(\\(.))EOR");
        mArg += std::regex_replace(match[4].str(), re3, "$1");
      }

      read_next_word = match[6].matched == false;

      if (!read_next_word)
        mSuffix = match.suffix().first;

      ++mTokIt;
    }

    return true;
  }

  bool done() { return mTokIt == mTokEnd; }

  const std::string& arg() const { return mArg; }

  std::string::const_iterator suffix() const { return mSuffix; }
};

} // namespace details


shell_splitter::shell_splitter(std::string::const_iterator begin, std::string::const_iterator end)
  : parser_impl(new details::shell_splitter_impl(begin, end))
{}

shell_splitter::~shell_splitter() = default;

bool shell_splitter::read_next() { return parser_impl->read_next(); }

bool shell_splitter::done() const { return parser_impl->done(); }

const std::string& shell_splitter::arg() const { return parser_impl->arg(); }

std::string::const_iterator shell_splitter::suffix() const { return parser_impl->suffix(); }


std::vector<std::string> shellsplit(const std::string& command) {
  std::vector<std::string> words;

  details::shell_splitter_impl parser(command.begin(), command.end());
  while(parser.read_next()) {
    words.push_back(parser.arg());
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

std::string shelljoin(const std::vector<std::string>& values) {
  std::string result;

  if (!values.empty())
    result = std::accumulate(std::next(values.begin()),
                             values.end(),
                             values.front(),
                             [](std::string accum, const std::string& value){ return std::move(accum) + ' ' + shellescape(value); });

  return result;
}

} // namespace shellwords
