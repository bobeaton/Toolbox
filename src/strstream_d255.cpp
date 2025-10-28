#include "stdafx.h"
#include "strstream_d255.h"

strstream_d255::strstream_d255()
    : std::stringstream(std::ios::in | std::ios::out)
{
}

strstream_d255::~strstream_d255() = default;
