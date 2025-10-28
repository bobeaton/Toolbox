#pragma once
#include <sstream>

// Backward-compatible replacement for the old 'strstream_d255'.
// Inherit from std::stringstream so it can be used anywhere an istream/ostream is expected.

class strstream_d255 : public std::stringstream
{
public:
    strstream_d255();
    ~strstream_d255();

    // Optional: mimic the old interface
    std::string str() const { return std::stringstream::str(); }

    void clearBuffer() { std::stringstream::str(""); std::stringstream::clear(); }
};
