#include <gmock/gmock.h>

#include "shellwords/shellwords.hxx"

using namespace shellwords;

TEST(shellwords, shellsplit) {
  EXPECT_THAT(shellsplit("ruby -i'.bak' -pe \"sub /foo/, '\\\\&bar'\" foobar\\ me.txt\n"),
              ::testing::ElementsAre("ruby", "-i.bak", "-pe", "sub /foo/, '\\&bar'", "foobar me.txt"));

  // shellwords does not interpret meta-characters
  EXPECT_THAT(shellsplit("ruby my_prog.rb | less"),
              ::testing::ElementsAre("ruby", "my_prog.rb", "|", "less"));

  EXPECT_THROW(shellsplit("one two 'three"), std::runtime_error);
  EXPECT_THROW(shellsplit("one two \"three"), std::runtime_error);
  EXPECT_THROW(shellsplit(R"(one '"'"''""'"")"), std::runtime_error);
}

TEST(shellwords, shellsplit_backslashes) {
  EXPECT_THAT(shellsplit(R"(\a\\b\\\c\\\\d\\\\\e\ "\a\\b\\\c\\\\d\\\\\e\ "'\a\\b\\\c\\\\d\\\\\e\ '\a\\b\\\c\\\\d\\\\\e\ )"),
              ::testing::ElementsAre("a\\b\\c\\\\d\\\\e \\a\\b\\\\c\\\\d\\\\\\e\\ \\a\\\\b\\\\\\c\\\\\\\\d\\\\\\\\\\e\\ a\\b\\c\\\\d\\\\e "));

  EXPECT_THAT(shellsplit(R"(printf %s \"\$\`\\\"\r\n)"),
              ::testing::ElementsAre("printf", "%s", R"("$`\"rn)"));

  EXPECT_THAT(shellsplit(R"(printf %s "\"\$\`\\\"\r\n")"),
              ::testing::ElementsAre("printf", "%s", R"("$`\"\r\n)"));
}

TEST(shellwords, shellescape) {
  EXPECT_EQ(shellescape("3"), "3");
  EXPECT_EQ(shellescape(""), "''");
  EXPECT_EQ(shellescape("^AZaz09_\\-.,:/@\n+'\""), "\\^AZaz09_\\\\-.,:/@'\n'+\\'\\\"");
}

TEST(shellwords, whitespaces) {
  EXPECT_THAT(shellsplit(shellescape("")),
              ::testing::ElementsAre(""));
  EXPECT_THAT(shellsplit(shellescape(" ")),
              ::testing::ElementsAre(" "));
  EXPECT_THAT(shellsplit(shellescape("  ")),
              ::testing::ElementsAre("  "));
  EXPECT_THAT(shellsplit(shellescape("\n")),
              ::testing::ElementsAre("\n"));
  EXPECT_THAT(shellsplit(shellescape("\n\n")),
              ::testing::ElementsAre("\n\n"));
  EXPECT_THAT(shellsplit(shellescape("\t")),
              ::testing::ElementsAre("\t"));
  EXPECT_THAT(shellsplit(shellescape("\t\t")),
              ::testing::ElementsAre("\t\t"));
  EXPECT_THAT(shellsplit(shellescape(" \n\t")),
              ::testing::ElementsAre(" \n\t"));
}

TEST(shellwords, shelljoin) {
  EXPECT_EQ(shelljoin({"ps", "-p", "1234"}), "ps -p 1234");
}

TEST(shellwords, shell_splitter) {
  std::vector<std::string> suffixes;
  const std::string cmd = "ps -p 1234";
  shell_splitter splitter(cmd.begin(), cmd.end());

  while(splitter.read_next()) {
    suffixes.emplace_back(splitter.suffix(), cmd.end());
  }

  EXPECT_THAT(suffixes, ::testing::ElementsAre("-p 1234", "1234", ""));
}
