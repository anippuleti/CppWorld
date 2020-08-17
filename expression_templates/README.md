Source code discusses concepts and implementation details of Expression
templates. Please refer to Functional programming in C++ for theory. We
have extended the capabilities to support char and std::string_view types.

Future enhancement is to hold a reference on lvalue references but copy
over actual data on rvalue references and pass by value to improve
performance.

References
 1.  Functional programming in C++