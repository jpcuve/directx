#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include <exception>

struct Descriptor {
    std::string name;
    size_t count = 0;
};

class parse_exception: std::exception{};

class StanfordHandler {
public:
    virtual void Invalid() = 0;
    virtual void Ply() = 0;
    virtual void Format(std::string& form, std::string& version) = 0;
    virtual void Element(std::string& name, size_t quantity) = 0;
    virtual void Property(std::string& dataType, std::string& name) = 0;
    virtual void PropertyList(std::string& countType, std::string& dataType, std::string& name) = 0;
    virtual void EndHeader() = 0;
    virtual void DataFixed(std::string& name, size_t index) = 0;
    virtual void DataVariable(std::string& name, size_t index, size_t count) = 0;
};

class StanfordParser {
public:
    StanfordParser(const std::vector<byte>& d): m_data{d}{};
    void Parse();
private:
    const std::vector<byte>& m_data;
    size_t m_position = 0;
    bool m_inData = false;
    std::vector<Descriptor> m_descriptors = {};
    size_t m_descriptorIndex = 0;
    size_t m_elementIndex = 0;
    size_t NextNewline();
    size_t NextCount(size_t byteCount, bool bigEndian);
};
