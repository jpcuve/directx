//
// Created by jpc on 24/11/2022.
//

#include "StanfordParser.h"
#include "helper.h"
#include <iostream>

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

void StanfordParser::Parse() {
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
            std::cout << line << std::endl;
            m_inData = line == "end_header";
            auto tokens = split(line, ' ');
            m_position = i + 1;
        }
    }
}
