#pragma once
#include "stdafx.h"
#include <sstream>

class DebugStreamBuffer : public std::wstringbuf {
public:
	virtual int sync();
};

class DebugStream : public std::wostream {
public:
	DebugStream() : std::wostream{ new DebugStreamBuffer() }{}
	~DebugStream() { delete rdbuf(); }
};

extern DebugStream dbg;