//
// Created by jpc on 24/11/2022.
//

#include "StanfordParser.h"
#include "helper.h"
#include <iostream>
#include <cstdlib>
#include <charconv>

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

size_t StanfordParser::NextCount(size_t byteCount, bool bigEndian) {
    size_t count = 0;
    for (size_t i = 0; i < byteCount; i++){
        auto next = static_cast<size_t>(m_data[m_position + i]);
        if (bigEndian){
            count = (count << 8) + next;
        } else {
            count += next << (8 * i);
        }
    }
    return count;
}

void StanfordParser::Parse(StanfordHandler& handler) {
    while (true){
        if (m_inData){
            break;
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
                    handler.Format(tokens[1], tokens[2]);
                } else if (tokens[0] == "element"){
                    if (tokens.size() < 3){
                        throw parse_exception();
                    }
                    int count;
                    std::from_chars(tokens[2].data(), tokens[2].data() + tokens[2].size(), count);
                    m_descriptors.push_back(Descriptor{
                        .name = tokens[1],
                        .count = count,
                    });
                    handler.Element(tokens[1], count);
                } else if (tokens[0] == "property"){

                } else if (tokens[0] == "end_header"){
                    m_inData = true;
                } else {
                    throw parse_exception();
                }
            }
            m_position = i + 1;
        }
    }
}

void DefaultStanfordHandler::Invalid() {
}

void DefaultStanfordHandler::Ply() {
}

void DefaultStanfordHandler::Format(std::string_view &form, std::string_view &version) {
}

void DefaultStanfordHandler::Element(std::string_view &name, int count) {
}

void DefaultStanfordHandler::Property(std::string_view &dataType, std::string_view &name) {
}

void DefaultStanfordHandler::PropertyList(std::string_view &countType, std::string_view &dataType, std::string_view &name) {
}

void DefaultStanfordHandler::EndHeader() {
}

void DefaultStanfordHandler::DataFixed(std::string_view &name, size_t index) {
}

void DefaultStanfordHandler::DataVariable(std::string_view &name, size_t index, size_t count) {
}
