#include <iostream>
#include <string>

// 10 Strings and Text

// 10.1 Introduction
// C-Style string: pointers (yuck)
// C++: std::string and std::string_view
// std::regex does regex

// 10.2 Strings
// string is a regular type


// string concatention example
std::string compose (const std::string& name, const std::string& domain)
{
    return name + "@" + domain;
}

auto addr = compose("dmr", "bell-labs.com");

// returning string by value is fine, string has a move constructor

// other common form of string concatentation

void m2(std::string& s1, std::string& s2)
{
    s1 = s1 + '\n'; // append newline
    s2 += '\n'; // append newlinw
}

// both ways are sematically equivalent

// std::strings are mutible
// example of modifying std::strings

std::string name = "Niels Stroustrup";

void m3()
{
    std:: string s = name.substr(6, 10); // s = "Stroustrup"
    name.replace(0, 5, "nicholas"); // name becomes "nicholas Stroustrup"
    name[0] = toupper(name[0]);
}

// example of comparing strings
 std::string incantation;

 void respond(const std::string& answer)
 {
    if (answer == incantation)
    {
        // ... perform magic ...
    }
    else if (answer == "yes") 
    {
        //..
    }
    // ..
 }

 // std::string offers read-only access to its contained char through c_str and data()
 // this is how you can get a C-style string from a std::string

 void print(const std::string& s)
 {
    printf("For people who like printf: %s\n", s.c_str()); // s.c_str() returns a pointer to s' characters
    std::cout << "For people who like streams: " << s << '\n';
 }

 // a string literal is a const char*
 // to get a std::string literal use suffix s

using namespace std::string_literals;

//  auto cat = "Cat"s; // a std::string
//  auto dog = "Dog"; // c-style string, const char*

// 10.2.1 string Implementation
// string is usually implemented with the short string optimization now
// this means short strings are kept on the string itself, big strings go to the free store

// std::string s1 {"Annemarie"}; // short string, kept in string object
// std::string s2 {"Annemarie Stroustrup"}; // long string, kept in free store

// char length for short string is implementation defined, but it's normally around 14 chars

// performance of strings demonds on the run-time environment
// in multi-threading, memory allocation is costly
// when lots of strings of different lengths are used, memory fragmentation can result


// to handle multiple char sets, string is an alias for general template basic_string with the character type char

template<typename Char>
class basic_string {
    // ... string of Char ... 
};

using string = basic_string<char>;

// this allows defining strings of arbitrary character types
// assume Jchar is a japanese char

using Jstring = basic_string<char>;

// this allows all the usual operations to be done, with Japanese chars

// 10.3 String Views

// most common use of a sequence of chars is to pass it to some function to read
// ways to do this
// std::string by value
// std::string by reference
// use C-style string

// for passing functions, std library offers std::string_view
// std::string_view is a pointer + length denotating a sequence of chars

// these chars can be stored in a std::string or c-style string
// std::string_view doesn't own the chars it points to

#include <complex>

using namespace std::literals::string_view_literals;


std::string cat(std::string_view sv1, std::string_view sv2)
{
    std::string res {sv1}; // initialize from sv1
    return res += sv2; // append from sv2 and return
}

// different ways of calling cat()


std::string king = "Harold";
auto s1 = cat(king, "William"); // HaroldWilliam: string and const char*
auto s2 = cat(king,king); // HaroldHarold: string and string
auto s3 = cat("Edward", "Stephen"sv); //EdwardStephen: const char* and std::string_view
auto s4 = cat("Canute"sv, king); //CanuteHarold
auto s5 = cat({&king[0],2},"Henry"sv); //HaHenry

// advantes of cat() over compose()
// it can be used to character sequences managed in different ways
// we can easily pass a substring
// we don't have to create a string to pass a c-style substring

// std::string_view is read only view of its characters
// for example, you can't use a string_view to pass args to a function that change the chars to lower case. 
// in that use case, consider a span.

// you can thinkg string_view as a kind of pointer. It has to point to something to be used. 

// bad example, don't do this
std::string_view bad()
{
    std::string s = "Once upon a time";
    return {&s[5],4}; // bad:returning a pointer to a local
}

// s dies at the end of the function, so we never get a chance to use it. 

// out of range access to string_view is undefined. 
// use at() or gsl::string_span to provide out of range safety

// 10.4 Regular Expressions
// <regex> is used for regexs

#include <regex>

std::regex pat {R"(\w{2}\s∗\d{5}(-\d{4})?)"}; // U.S. postal code pattern: XXddddd-dddd and variants

// to express a pattern use Raw string literal R"()"
// raw strings allow backslashes without the need to escape them
// std support for regex
// regex_match(): match against a string
// regex_search(): search for string that matches a regex in a data stream
// regex_replace(): search and replace in a data stream
// regex_iterator: iterate over matches and submatches
// regex_token_iterator: iterate over non-matches

// 10.4.1 Searching
// search for a pattern in a stream
int lineno = 0;
for (std::string line; std::getline(cin,line); ) { // read into line buffer
    ++lineno;
    std::smatch matches; // matched strings go here
    if (std::regex_search(line, matches, pat)) // search for pat in line
        std::cout << lineno << ": " << matches[0] << '\n';
}

// another example

using namespace std;

void use()
{
    ifstream in("file.txt"); // input file
    if (!in) { // check that the file was opened
        cerr << "no file\n";
        return;
    }

    regex pat {R"(\w{2}\s∗\d{5}(-\d{4})?)"}; // U.S. postal code pattern

    int lineno = 0;
    for (string line, getline(in, line); ) {
        ++lineno;
        smatch matches; // matched strings go here
        if (regex_search(line, matches, path)) {
            std::cout << lineno << ": " << matches[0] << '\n'; // the complete match
            if (1<matches.size() && matches[1].matched) // if there is a sub-pattern
                                                        // and if it is matched
                cout << "\t: " << matches[1] << '\n'; // submatch
        }
    }
}

// the above function looks for zip codes
// regex is designed to be compiled into state machines

// 10.4.2 Regular Expression Notation
^A*B+C?$ // regex for zero or more As, followed by >= Bs, follwed by optional c

// the following examples match
AAAAABBBBC
BC
B

// these examples don't match
AAAA // no B
   AAAAABC // initial space
AABBCC // too many Cs

// a subpattern is engaged in parathensis, and can be extracted with seperatly from smatch

\d+-\d+ // no subpatterns
\d+(-\d+) // one subpattern
(\d+)(-\d+) // two subpatterns

// 10.4.3 Iterators
// a regex_iterator iterators of a sequence of chars finding matches for a pattern
// for example, sregex_iterator (a regex_iterator<string>) can be used to output all whitespeace seperated words in a string

void test()
{
    string input ="aa ss; asd++e^asdf asfg";
    regex pat {R"(\s+(\w+))"};
    for (sregex_iterator p(input.begin(),input.end(),pat); p!=sregex_iterator{}; ++p)
        std::cout << (*p)[1] << '\n';
}

// the function above will output
as
asd
asdfg

