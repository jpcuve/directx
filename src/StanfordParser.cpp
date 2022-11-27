//
// Created by jpc on 24/11/2022.
//

#include "StanfordParser.h"
#include "helper.h"
#include <iostream>
#include <cstdlib>
#include <charconv>
#include <map>

size_t StanfordParser::NextNewline() {
    auto i = m_position;
    while (i < m_data.size()){
        switch (m_data[i]){
            case 0xD: {
                if (i < m_data.size() - 1 && m_data[i + 1] == 0xA){
                    return i + 1;
                }
                return i;
            }
            case 0xA: {
                return i;
            }
        }
        i++;
    }
    throw parse_exception();
}

size_t StanfordParser::NextCount(size_t byteCount) {
    size_t count = 0;
    for (size_t i = 0; i < byteCount; i++){
        auto next = static_cast<size_t>(m_data[m_position + i]);
        if (m_binaryBigEndian){
            count = (count << 8) + next;
        } else {
            count += next << (8 * i);
        }
    }
    return count;
}

std::map<std::string_view, size_t> DATA_SIZES {
        {"char", 1},
        {"uint8", 1},
        {"uchar", 1},
        {"short", 2},
        {"ushort", 2},
        {"int", 4},
        {"uint", 4},
        {"int32", 4},
        {"uint32", 4},
        {"float", 4},
        {"float32", 4},
        {"double", 8},
};

void StanfordParser::Parse(StanfordHandler& handler) {
    while (true){
        if (m_inData){
            if (m_descriptorIndex > m_descriptors.size() - 1){
                break;
            }
            auto descriptor = m_descriptors.at(m_descriptorIndex);
            if (descriptor.countSize == 0){ // fixed
                m_position += descriptor.dataSize;
                handler.DataFixed(descriptor.name, m_elementIndex, m_data, m_position - descriptor.dataSize, m_position);
            } else { // variable
                auto count = NextCount(descriptor.countSize);
                m_position += descriptor.countSize;
                auto totalLength = count * descriptor.dataSize;
                m_position += totalLength;
                handler.DataVariable(descriptor.name, m_elementIndex, count, m_data, m_position - totalLength, m_position);
            }
            m_elementIndex++;
            if (m_elementIndex == descriptor.count){
                m_descriptorIndex++;
                m_elementIndex = 0;
            }
        } else {
            auto i = NextNewline();
            auto line = std::string_view(reinterpret_cast<const char *>(&m_data[m_position]), i - m_position);
            auto bracket = line.find_first_of('{', 0);
            if (bracket != std::string::npos){
                line = std::string_view(reinterpret_cast<const char *>(&m_data[m_position]), bracket);
            }
            auto tokens = split(line, ' ');
            if (tokens.empty()){
                throw parse_exception();
            } else {
                if (tokens[0] == "ply"){
                    handler.Ply();
                } else if (tokens[0] == "format"){
                    if (tokens.size() < 3){
                        throw parse_exception();
                    }
                    m_binaryBigEndian = tokens[2] != "binary_little_endian";
                    handler.Format(tokens[1], tokens[2]);
                } else if (tokens[0] == "element"){
                    if (tokens.size() < 3){
                        throw parse_exception();
                    }
                    int count;
                    std::from_chars(tokens[2].data(), tokens[2].data() + tokens[2].size(), count);
                    m_descriptors.push_back(Descriptor{
                        .name {std::string(tokens[1])},
                        .count = count,
                        .dataSize = 0,
                        .countSize = 0,
                    });
                    handler.Element(tokens[1], count);
                } else if (tokens[0] == "property"){
                    if (m_descriptors.empty() || tokens.size() < 2){
                        throw parse_exception();
                    }
                    auto& descriptor = m_descriptors[m_descriptors.size() - 1];
                    if (tokens[1] == "list"){
                        if (tokens.size() < 4){
                            throw parse_exception();
                        }
                        descriptor.countSize = DATA_SIZES[tokens[2]];
                        descriptor.dataSize = DATA_SIZES[tokens[3]];
                        handler.PropertyList(tokens[2], tokens[3], tokens[4]);
                    } else {
                        descriptor.dataSize += DATA_SIZES[tokens[1]];
                        handler.Property(tokens[1], tokens[2]);
                    }
                } else if (tokens[0] == "end_header"){
                    m_inData = true;
                    handler.EndHeader();
                } else {
                    throw parse_exception();
                }
            }
            m_position = i + 1;
        }
    }
}

void DefaultStanfordHandler::Invalid() {
    std::cout << "invalid" << std::endl;
}

void DefaultStanfordHandler::Ply() {
    std::cout << "ply" << std::endl;
}

void DefaultStanfordHandler::Format(std::string_view &form, std::string_view &version) {
    std::cout << "format " << form << " " << version << std::endl;
}

void DefaultStanfordHandler::Element(std::string_view &name, int count) {
    std::cout << "element " << name << " " << count << std::endl;
}

void DefaultStanfordHandler::Property(std::string_view &dataType, std::string_view &name) {
    std::cout << "property " << dataType << " " << name << std::endl;
}

void DefaultStanfordHandler::PropertyList(std::string_view &countType, std::string_view &dataType, std::string_view &name) {
    std::cout << "property list " << countType << " " << dataType << " " << name << std::endl;
}

void DefaultStanfordHandler::EndHeader() {
    std::cout << "end_header" << std::endl;
}

void DefaultStanfordHandler::DataFixed(std::string &name, size_t index, const std::vector<byte> &data, size_t lo, size_t hi) {
    std::cout << "data fixed (" << lo << ", " << hi << ")" << std::endl;
}

void DefaultStanfordHandler::DataVariable(std::string &name, size_t index, size_t count, const std::vector<byte> &data, size_t lo, size_t hi) {
    std::cout << "data variable " << count << " (" << lo << ", " << hi << ")" << std::endl;
}
