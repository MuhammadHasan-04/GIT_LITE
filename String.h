#pragma once

// Name: Muhammad Hasan Naveed
// Roll no: 23I-0797
// Assignment #2

#include <iostream>
using namespace std;

class String
{
    // Private data members
    char *s;
    int size;

public:
    // Constructor and destructor
    String();                // default constructor
    String(const char *str); // initializes the string with constant c-string
    String(const String &);  // copy constructor to initialize the string from the existing string
    String(int x);           // initializes a string of predefined size
    ~String();               // destructor

    char *getdata(); // returns the string inside the object

    // Subscript Operator
    const char operator[](int i) const; // returns the character at index [x]

    // Arithmetic Operators
    String operator+(const String &str);    // appends a String at the end of the String
    String operator+(const char &str);      // appends a char at the end of the String
    String operator+(const char *str);      // appends a String at the end of the String
    String operator-(const String &substr); // removes the substr from the String
    String operator-(const char &str);      // removes all occurrences of char from the String
    String operator-(const char *str);      // removes the str from the String
                                            // returns true if the string is empty
    // Assignment Operators
    String &operator=(const String &); // copies one String to another
    String &operator=(const char *);   // copies one c-string to another

    // Logical Operators
    bool operator==(const String &) const; // returns true if two Strings are equal
    bool operator==(const char *) const;   // returns true if the c-string is equal to the String

    // Unary Operators
    bool operator!(); // returns true if the String is empty
    bool empty();     // returns true if the String is empty
    // Function-Call Operators
    int operator()(char) const;           // returns the first index of the character being searched
    int operator()(const String &) const; // returns the first index of the String being searched
    int operator()(const char *) const;   // returns the index of the c-string being searched

    // Overload the < operator to compare two String objects lexicographically
    bool operator<(const String &other) const
    {
        int i = 0;
        while (s[i] != '\0' && other.s[i] != '\0')
        {
            if (s[i] < other.s[i])
                return true; // Current string is lexicographically smaller
            if (s[i] > other.s[i])
                return false; // Current string is lexicographically larger
            i++;
        }

        // If one string is a prefix of the other, the shorter one is considered smaller
        return s[i] == '\0' && other.s[i] != '\0';
    }

    // Overload the > operator to compare two String objects lexicographically
    bool operator>(const String &other) const
    {
        int i = 0;
        while (s[i] != '\0' && other.s[i] != '\0')
        {
            if (s[i] > other.s[i])
                return true; // Current string is lexicographically larger
            if (s[i] < other.s[i])
                return false; // Current string is lexicographically smaller
            i++;
        }

        // If one string is a prefix of the other, the longer one is considered larger
        return s[i] != '\0' && other.s[i] == '\0';
    }

    // Conversion Operator
    operator int() const; // returns the length of the string

    // Utility Functions
    int getLength(); // returns the length of the string
    bool operator!=(const char *other) const
    {
        // Compare the internal data with the other C-string
        int i = 0;
        while (s[i] != '\0' && other[i] != '\0')
        {
            if (s[i] != other[i])
            {
                return true; // Characters are different
            }
            i++;
        }

        // If one string ends before the other, they are not equal
        return s[i] != '\0' || other[i] != '\0';
    }

    // Friend functions for input/output
    friend std::ostream &operator<<(std::ostream &output, const String &str);

    // Input stream operator (>>)
    friend std::istream &operator>>(std::istream &input, String &str);
};

ostream &operator<<(ostream &output, const String &str)
{
    if (str.s == nullptr)
    {
        output << "\n";
    }
    else
    {
        output << str.s;
    }
    return output;
}

istream &operator>>(istream &input, String &str)
{
    char temp[100];
    input >> temp;
    delete[] str.s;
    str.size = 0;
    while (temp[str.size] != '\0')
    {
        str.size++;
    }
    str.s = new char[str.size + 1];
    for (int i = 0; i < str.size; i++)
    {
        str.s[i] = temp[i];
    }
    str.s[str.size] = '\0';
    return input;
}

String::String() : s(nullptr), size(0) {}

String::String(const char *str)
{
    size = 0;
    while (str[size] != '\0')
    {
        size++;
    }
    s = new char[size + 1];
    for (int i = 0; i < size; i++)
    {
        s[i] = str[i];
    }
    s[size] = '\0';
}

String::String(const String &cs)
{
    size = cs.size;
    s = new char[size + 1];
    for (int i = 0; i < size; i++)
    {
        s[i] = cs.s[i];
    }
    s[size] = '\0';
}

String::String(int x)
{
    size = x;
    s = new char[size + 1];
    for (int i = 0; i < size; i++)
    {
        s[i] = '\0';
    }
    s[size] = '\0';
}

String::~String()
{
    delete[] s;
}

char *String::getdata()
{
    return s;
}

const char String::operator[](int i) const
{
    if (i < 0)
    {
        i = size + i; // handle negative indices
    }
    if (i < 0 || i >= size)
    {
        throw std::out_of_range("Index out of range");
    }
    return s[i];
}

String String::operator+(const String &str)
{
    int newSize = size + str.size;
    char *temp = new char[newSize + 1];
    for (int i = 0; i < size; i++)
    {
        temp[i] = s[i];
    }
    for (int i = 0; i < str.size; i++)
    {
        temp[size + i] = str.s[i];
    }
    temp[newSize] = '\0';

    String result(temp);
    delete[] temp;
    return result;
}

String String::operator+(const char &str)
{
    char *temp = new char[size + 2];
    for (int i = 0; i < size; i++)
    {
        temp[i] = s[i];
    }
    temp[size] = str;
    temp[size + 1] = '\0';

    String result(temp);
    delete[] temp;
    return result;
}

String String::operator+(const char *str)
{
    int newSize = size;
    while (str[newSize - size] != '\0')
    {
        newSize++;
    }
    char *temp = new char[newSize + 1];
    for (int i = 0; i < size; i++)
    {
        temp[i] = s[i];
    }
    for (int i = size; i < newSize; i++)
    {
        temp[i] = str[i - size];
    }
    temp[newSize] = '\0';

    String result(temp);
    delete[] temp;
    return result;
}

String String::operator-(const String &substr)
{
    String result(*this);
    int pos = result(s);
    if (pos != -1)
    {
        for (int i = pos; i < size - substr.size; i++)
        {
            result.s[i] = result.s[i + substr.size];
        }
        result.s[size - substr.size] = '\0';
    }
    return result;
}

String String::operator-(const char &str)
{
    String result(*this);
    for (int i = 0; i < size; i++)
    {
        if (result.s[i] == str)
        {
            for (int j = i; j < size - 1; j++)
            {
                result.s[j] = result.s[j + 1];
            }
            result.s[size - 1] = '\0';
            size--;
            break;
        }
    }
    return result;
}

String String::operator-(const char *str)
{
    String result(*this);
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    int pos = result(str);
    if (pos != -1)
    {
        for (int i = pos; i < size - len; i++)
        {
            result.s[i] = result.s[i + len];
        }
        result.s[size - len] = '\0';
    }
    return result;
}

String &String::operator=(const String &cs)
{
    if (this != &cs)
    {
        delete[] s;
        size = cs.size;
        s = new char[size + 1];
        for (int i = 0; i < size; i++)
        {
            s[i] = cs.s[i];
        }
        s[size] = '\0';
    }
    return *this;
}

String &String::operator=(const char *cs)
{
    delete[] s;
    size = 0;
    while (cs[size] != '\0')
    {
        size++;
    }
    s = new char[size + 1];
    for (int i = 0; i < size; i++)
    {
        s[i] = cs[i];
    }
    s[size] = '\0';
    return *this;
}

bool String::operator!()
{
    return size == 0;
}

bool String::operator==(const String &cs) const
{
    if (size != cs.size)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if (s[i] != cs.s[i])
        {
            return false;
        }
    }
    return true;
}

bool String::operator==(const char *cs) const
{
    int i = 0;
    while (cs[i] != '\0' && s[i] != '\0')
    {
        if (cs[i] != s[i])
        {
            return false;
        }
        i++;
    }
    return cs[i] == s[i];
}

int String::operator()(char c) const
{
    for (int i = 0; i < size; i++)
    {
        if (s[i] == c)
        {
            return i;
        }
    }
    return -1;
}

int String::operator()(const String &str) const
{
    int len = str.size;
    for (int i = 0; i <= size - len; i++)
    {
        bool found = true;
        for (int j = 0; j < len; j++)
        {
            if (s[i + j] != str.s[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return i;
        }
    }
    return -1;
}

int String::operator()(const char *str) const
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    for (int i = 0; i <= size - len; i++)
    {
        bool found = true;
        for (int j = 0; j < len; j++)
        {
            if (s[i + j] != str[j])
            {
                found = false;
                break;
            }
        }
        if (found)
        {
            return i;
        }
    }
    return -1;
}

int String::getLength()
{
    return size;
}

String::operator int() const
{
    return size;
}
#include <cstring>