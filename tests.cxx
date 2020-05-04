#include <gmock/gmock.h>

#include "shellwords.hxx"

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

TEST(shellwords, shellsplit_w) {
  EXPECT_THAT(shellsplit(L"ruby -i'.bak' -pe \"sub /foo/, '\\\\&bar'\" foobar\\ me.txt\n"),
              ::testing::ElementsAre(L"ruby", L"-i.bak", L"-pe", L"sub /foo/, '\\&bar'", L"foobar me.txt"));

  // shellwords does not interpret meta-characters
  EXPECT_THAT(shellsplit(L"ruby my_prog.rb | less"),
              ::testing::ElementsAre(L"ruby", L"my_prog.rb", L"|", L"less"));

  EXPECT_THROW(shellsplit(L"one two 'three"), std::runtime_error);
  EXPECT_THROW(shellsplit(L"one two \"three"), std::runtime_error);
  EXPECT_THROW(shellsplit(LR"(one '"'"''""'"")"), std::runtime_error);

  EXPECT_THAT(shellsplit(L"echo あい"), 
              ::testing::ElementsAre(L"echo", L"あい"));
}

TEST(shellwords, shellsplit_backslashes) {
  EXPECT_THAT(shellsplit(R"(\a\\b\\\c\\\\d\\\\\e\ "\a\\b\\\c\\\\d\\\\\e\ "'\a\\b\\\c\\\\d\\\\\e\ '\a\\b\\\c\\\\d\\\\\e\ )"),
              ::testing::ElementsAre("a\\b\\c\\\\d\\\\e \\a\\b\\\\c\\\\d\\\\\\e\\ \\a\\\\b\\\\\\c\\\\\\\\d\\\\\\\\\\e\\ a\\b\\c\\\\d\\\\e "));

  EXPECT_THAT(shellsplit(R"(printf %s \"\$\`\\\"\r\n)"),
              ::testing::ElementsAre("printf", "%s", R"("$`\"rn)"));

  EXPECT_THAT(shellsplit(R"(printf %s "\"\$\`\\\"\r\n")"),
              ::testing::ElementsAre("printf", "%s", R"("$`\"\r\n)"));
}

TEST(shellwords, shellsplit_backslashes_w) {
  EXPECT_THAT(shellsplit(LR"(\a\\b\\\c\\\\d\\\\\e\ "\a\\b\\\c\\\\d\\\\\e\ "'\a\\b\\\c\\\\d\\\\\e\ '\a\\b\\\c\\\\d\\\\\e\ )"),
              ::testing::ElementsAre(L"a\\b\\c\\\\d\\\\e \\a\\b\\\\c\\\\d\\\\\\e\\ \\a\\\\b\\\\\\c\\\\\\\\d\\\\\\\\\\e\\ a\\b\\c\\\\d\\\\e "));

  EXPECT_THAT(shellsplit(LR"(printf %s \"\$\`\\\"\r\n)"),
              ::testing::ElementsAre(L"printf", L"%s", LR"("$`\"rn)"));

  EXPECT_THAT(shellsplit(LR"(printf %s "\"\$\`\\\"\r\n")"),
              ::testing::ElementsAre(L"printf", L"%s", LR"("$`\"\r\n)"));

  EXPECT_THAT(shellsplit(L"echo あい"), 
              ::testing::ElementsAre(L"echo", L"あい"));

  EXPECT_THAT(shellsplit(L"echo \\あ\\い"), 
              ::testing::ElementsAre(L"echo", L"あい"));
}

TEST(shellwords, shellescape) {
  EXPECT_EQ(shellescape("3"), "3");
  EXPECT_EQ(shellescape(""), "''");
  EXPECT_EQ(shellescape("^AZaz09_\\-.,:/@\n+'\""), "\\^AZaz09_\\\\-.,:/@'\n'+\\'\\\"");
}

TEST(shellwords, shellescape_w) {
  EXPECT_EQ(shellescape(L"3"), L"3");
  EXPECT_EQ(shellescape(L""), L"''");
  EXPECT_EQ(shellescape(L"^AZaz09_\\-.,:/@\n+'\""), L"\\^AZaz09_\\\\-.,:/@'\n'+\\'\\\"");

  // This is not a spec.  It describes the current behavior which may
  // be changed in future.  There would be no multibyte character
  // used as shell meta-character that needs to be escaped.
  EXPECT_EQ(shellescape(L"あい"), L"\\あ\\い");
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

TEST(shellwords, whitespaces_w) {
  EXPECT_THAT(shellsplit(shellescape(L"")),
              ::testing::ElementsAre(L""));
  EXPECT_THAT(shellsplit(shellescape(L" ")),
              ::testing::ElementsAre(L" "));
  EXPECT_THAT(shellsplit(shellescape(L"  ")),
              ::testing::ElementsAre(L"  "));
  EXPECT_THAT(shellsplit(shellescape(L"\n")),
              ::testing::ElementsAre(L"\n"));
  EXPECT_THAT(shellsplit(shellescape(L"\n\n")),
              ::testing::ElementsAre(L"\n\n"));
  EXPECT_THAT(shellsplit(shellescape(L"\t")),
              ::testing::ElementsAre(L"\t"));
  EXPECT_THAT(shellsplit(shellescape(L"\t\t")),
              ::testing::ElementsAre(L"\t\t"));
  EXPECT_THAT(shellsplit(shellescape(L" \n\t")),
              ::testing::ElementsAre(L" \n\t"));
}

TEST(shellwords, shelljoin) {
  EXPECT_EQ(shelljoin({"ps", "-p", "1234"}), "ps -p 1234");
}

TEST(shellwords, shelljoin_w) {
  EXPECT_EQ(shelljoin({L"ps", L"-p", L"1234"}), L"ps -p 1234");
}

