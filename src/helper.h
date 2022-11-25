#pragma once

#include "stdafx.h"
#include <stdexcept>
#include <vector>
#include <string>

#define THROW_IF_FAILED(code) if (FAILED(code)){ throw std::runtime_error(""); }

void loadBinaryFile(const std::string& fileName, std::vector<byte>& buffer);
std::vector<std::string_view> split(std::string_view& s, char delimiter);