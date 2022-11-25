#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include <exception>

struct Descriptor {
    std::string_view& name;
    int length = 0;
    int count = 0;
};

class parse_exception: std::exception{};

class StanfordHandler {
public:
    virtual void Invalid() = 0;
    virtual void Ply() = 0;
    virtual void Format(std::string_view& form, std::string_view& version) = 0;
    virtual void Element(std::string_view& name, int quantity) = 0;
    virtual void Property(std::string_view& dataType, std::string_view& name) = 0;
    virtual void PropertyList(std::string_view& countType, std::string_view& dataType, std::string_view& name) = 0;
    virtual void EndHeader() = 0;
    virtual void DataFixed(std::string_view& name, size_t index) = 0;
    virtual void DataVariable(std::string_view& name, size_t index, size_t count) = 0;
};

class DefaultStanfordHandler: public StanfordHandler {
    void Invalid() override;

    void Ply() override;

    void Format(std::string_view &form, std::string_view &version) override;

    void Element(std::string_view &name, int count) override;

    void Property(std::string_view &dataType, std::string_view &name) override;

    void PropertyList(std::string_view &countType, std::string_view &dataType, std::string_view &name) override;

    void EndHeader() override;

    void DataFixed(std::string_view &name, size_t index) override;

    void DataVariable(std::string_view &name, size_t index, size_t count) override;
};

class StanfordParser {
public:
    StanfordParser(const std::vector<byte>& d): m_data{d}{};
    void Parse(StanfordHandler& handler);
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
