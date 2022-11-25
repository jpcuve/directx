#include "helper.h"
#include <fstream>

void loadBinaryFile(const std::string& fileName, std::vector<byte>& buffer) {
    std::fstream f{fileName, std::ios::in|std::ios::binary};
    constexpr std::streamsize count = 32768;
    auto pBuf = std::unique_ptr<byte[]>(new byte[count]);
    if (f.is_open()) {
        while (true) {
            f.read(reinterpret_cast<char*>(pBuf.get()), count);
            buffer.insert(buffer.end(), pBuf.get(), pBuf.get() + f.gcount());
            if (f.eof()) {
                break;
            }
        }
        f.close();
    }
    else {
        throw std::runtime_error("File not found");
    }
}

std::vector<std::string_view> split(std::string_view& s, char delimiter){
    std::vector<std::string_view> retVal {};
    if (!s.empty()){
        size_t init {s.find_first_not_of(delimiter)};
        while (init != std::string_view::npos){
            size_t done = s.find(delimiter, init);
            if (done == std::string_view::npos){
                done = s.length();
            }
            retVal.push_back(s.substr(init, done - init));
            init = s.find_first_not_of(delimiter, done);
        }
    }
    return retVal;
}

