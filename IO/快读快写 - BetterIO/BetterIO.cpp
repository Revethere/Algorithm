#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <new>
#include <type_traits>

namespace BetterIO {
    namespace ExtraType {
        inline bool isDigit(const char &ch) {
            return '0' <= ch && ch <= '9';
        }
        inline bool isLetter(const char &ch) {
            return 'A' <= ch && ch <= 'Z'
                || 'a' <= ch && ch <= 'z';
        }
        inline bool isChar(const char &ch) {
            return isDigit(ch) || isLetter(ch);
        }
        inline bool isVaild(const char &ch) {
            return '!' <= ch && ch <= '~';
        }
    }
    using std::string, std::is_same_v;
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1 << 16
#endif
    class betterInput {
        FILE *stream;
        char *buffer, *iter;
        int buffer_size, volume;
        void input() {
            volume = fread(buffer, 1, buffer_size, stream);
            iter = buffer;
        }
        inline void getChar(void) {
            if(iter - buffer == volume) input();
            else ++iter;
        }
    public:
        betterInput(FILE *stream = stdin) {
            this -> stream = stream;
            buffer_size = BUFFER_SIZE;
            buffer = new char [buffer_size];
            iter = buffer;
            volume = 0;
        }
        template <typename _Tp> betterInput& operator >> (_Tp &var) {
            if constexpr(is_same_v<_Tp, char> || is_same_v<_Tp, string>) return inputCharType(var);
            else {
                static bool neg = false;
                while(!ExtraType::isDigit(*iter)) {
                    neg ^= *iter == '-';
                    getChar();
                }
                var = 0;
                if constexpr(is_same_v<_Tp, float> || is_same_v<_Tp, double>) return inputFloat(var, neg);
                else {
                    while(ExtraType::isDigit(*iter)) {
                        var = (var << 1) + (var << 3) + (*iter ^ '0');
                        getChar();
                    }
                    if(neg) var = ~var + 1;
                }
            }
            return *this;
        }
        template <typename _Tp> betterInput& inputCharType(_Tp &var) {
            while(!ExtraType::isVaild(*iter)) getChar();
            if constexpr(is_same_v<_Tp, char>) var = *iter, getChar();
            else {
                var.clear();
                while(ExtraType::isVaild(*iter)) {
                    var.push_back(*iter);
                    getChar();
                }
            }
            return *this;
        }
        template <typename _Tp> betterInput& inputFloat(_Tp &var, const bool &neg) {
            while(ExtraType::isDigit(*iter)) {
                var = var * 10 + (*iter ^ '0');
                getChar();
            }
            if(*iter == '.') {
                getChar(); 
                _Tp dec = 0.1;
                while(ExtraType::isDigit(*iter)) {
                    var += dec * (*iter ^ '0');
                    dec /= 10;
                    getChar();
                }
            } else getChar();
            if(neg) var = -var;
            return *this;
        }
        ~betterInput() { delete[] buffer; }
    };
    class betterOutput {
        FILE *stream;
        char *buffer;
        int buffer_size, volume;
        inline void putChar(const char &ch) {
            if(buffer_size == volume) flush();
            buffer[volume++] = ch;
        }
    public:
        betterOutput(FILE *stream = stdout) {
            this -> stream = stream;
            buffer_size = BUFFER_SIZE;
            buffer = new char [buffer_size];
            volume = 0;
        }
        void flush(void) {
            fwrite(buffer, 1, volume, stream);
            volume = 0;
        }
        template <typename _Tp> betterOutput& operator << (_Tp var) {
            if(var < 0) putChar('-'), var = -var;
            if constexpr(is_same_v<_Tp, float> || is_same_v<_Tp, double>) return outputFloat(var);
            else {
                static char digit[25];
                static short cur = 0;
                do digit[cur++] = static_cast<char> (var % 10 + '0'); while(var /= 10);
                while(cur) putChar(digit[--cur]);
            }
            return *this;
        }
        betterOutput& operator << (const char &var) {
            putChar(var);
            return *this;
        }
        betterOutput& operator << (const char *var) {
            while(*var) putChar(*var++);
            return *this;
        }
        betterOutput& operator << (const string &var) {
            for(auto cur: var) putChar(cur);
            return *this;
        }
        template <typename _Tp> betterOutput& outputFloat(_Tp &var) {
            const string str_var = std::to_string(var);
            for(auto cur: str_var) putChar(cur);
            return *this;
        }
        ~betterOutput() { flush(); delete[] buffer; }
    };
#undef BUFFER_SIZE
}

signed main() {

    return 0;
}