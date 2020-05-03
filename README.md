# shellwords-cpp

shellwords-cpp is a library designed to manipulate strings according to the word parsing rules of the UNIX Bourne shell.

See [IEEE Std 1003.1-2008, 2016 Edition, the Shell & Utilities volume](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/contents.html) for more details.

It is a port of the [Shellwords Ruby module](https://ruby-doc.org/stdlib-2.4.1/libdoc/shellwords/rdoc/Shellwords.html).

## Usage

### Tokenization
```
// std::vector<std::string> shellsplit(const std::string& command);
shellsplit("ps -p 1234"); // ["ps", "-p", "1234"]
```

### Stringification
```
// std::string shelljoin(const std::vector<std::string>& values);
shelljoin({"ps", "-p", "1234"}); // "ps -p 1234"
```

## License

This tool is released under the terms of the MIT License. See the LICENSE.txt file for more details.
