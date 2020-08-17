#include <iostream>
#include "string_concat.h"

///Appending Raw strings
void append_raw_str()
{
  std::string cname = "Xilinx";
  std::string lname = "Inc";
  std::string addrs = "2100 Logic Dr, San Jose, CA 95124";

  nan::String_concat<> club;
  std::string const f = club + cname + ' ' + lname + '\n' + addrs;
  std::cout << f << std::endl;
}

void append_str_views()
{
  std::string cname = "Xilinx";
  std::string lname = "Inc";
  std::string addrs = "2100 Logic Dr, San Jose, CA 95124";

  auto str_view = [](std::string const& str) {
    return std::string_view{str};
  };

  nan::String_concat<> club;

  std::string const f = club + str_view(cname) + ' ' +
      str_view(lname) + '\n' + str_view(addrs);

  std::cout << f << std::endl;
}

int main()
{
  append_raw_str();
  append_str_views();
  return 0;
}
