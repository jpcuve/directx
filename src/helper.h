#pragma once

#include "stdafx.h"
#include <stdexcept>

#define THROW_IF_FAILED(code) if (FAILED(code)){ throw std::runtime_error(""); }

