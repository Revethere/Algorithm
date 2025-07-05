/*
  Last Update: 23.9.6
  File Name: BetterFileStream
  Version: v1.0
  Wrote By: Rui-Ethereal
*/

#ifdef __cplusplus

#ifndef RUI_STDIO
#define RUI_STDIO
#include <iostream>
#include <cstdio>
#include <new>
#endif

#include <ctime>
#include <cstdlib>

#ifndef RUI_STRING_H
#define RUI_STRING_H
#include <string>
#include <cstring>
#endif

namespace Rui_Ethereal {
#if defined(RUI_STDIO) and __cplusplus >= 201103L
    namespace file_io {
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1 << 16
#elif BUFFER_SIZE > 1 << 26
#warning "BUFFER_SIZE is set to 2^16, MAX_SIZE that doesn't cause stack overflow is 2^26."
#endif
        namespace exio {
            inline auto IsDigit(const char &_) -> bool {
                return '0' <= _ and _ <= '9'; }
            inline auto IsLetter(const char &_) -> bool {
                return ('A' <= _ and _ <= 'Z')
                    or ('a' <= _ and _ <= 'z'); }
            inline auto IsValidChar(const char &_) -> bool {
                return IsDigit(_) or IsLetter(_); }
        }
        class BetterStreamReader {
            FILE *file_stream;
            char *buffer, *iter;
            long long int buffer_size, data_volume;
            inline auto GetChar(void) -> void {
                if(iter - buffer == data_volume) DataRead();
                else ++iter; }
        public:
            BetterStreamReader(FILE *file_stream = stdin) {
                buffer_size = BUFFER_SIZE;
                buffer = new char [buffer_size] {};
                iter = buffer;
                data_volume = 0;
                this->file_stream = file_stream;
            }
            inline auto DataRead(void) -> void {
                data_volume = fread(buffer, 1, buffer_size, file_stream);
                iter = buffer; }
            template <typename _Tp> BetterStreamReader& operator >> (_Tp &var) {
                bool negative = false;
                while(!exio::IsDigit(*iter)) {
                    negative ^= *iter == '-';
                    GetChar(); }
                var = 0;
                while(exio::IsDigit(*iter)) {
                    var = (var << 1) + (var << 3) + (*iter ^ '0');
                    GetChar(); }
                if(negative) var = ~var + 1;
                return *this; }
            BetterStreamReader& operator >> (char &var) {
                while(!exio::IsValidChar(*iter)) GetChar();
                var = *iter, GetChar();
                return *this; }
            BetterStreamReader& operator >> (char *var) {
                while(!exio::IsValidChar(*iter)) GetChar();
                while(exio::IsValidChar(*iter)) {
                    *var++ = *iter;
                    GetChar(); }
                *var = '\0';
                return *this; }
#ifdef RUI_STRING_H
            BetterStreamReader& operator >> (std::string &var) {
                while(!exio::IsValidChar(*iter)) GetChar();
                while(exio::IsValidChar(*iter)) {
                    var.push_back(*iter);
                    GetChar(); }
                return *this; }
        };
#endif
        class BetterStreamWriter {
            FILE *file_stream;
            char *buffer;
            long long int buffer_size, data_volume;
            inline auto PutChar(const char &_) -> void {
                if(data_volume == buffer_size) Flush();
                buffer[data_volume++] = _; }
        public:
            BetterStreamWriter(FILE *file_stream = stdout) {
                buffer_size = BUFFER_SIZE;
                buffer = new char [buffer_size] {};
                data_volume = 0;
                this->file_stream = file_stream;
            }
            inline auto Flush(void) -> void {
                fwrite(buffer, 1, data_volume, file_stream);
                data_volume = 0; }
            template <typename _Tp> BetterStreamWriter& operator << (_Tp var) {
                if(var < 0) {
                    PutChar('-');
                    var = ~var + 1; }
                static char ccf[44];
                short cur = 0;
                do ccf[cur++] = static_cast <char> (var % 10 + '0'); while(var /= 10);
                while(cur) PutChar(ccf[--cur]);
                return *this; }
            BetterStreamWriter& operator << (const char &var) {
                PutChar(var);
                return *this; }
            BetterStreamWriter& operator << (const char *var) {
                while(*var) PutChar(*var++);
                return *this; }
#ifdef RUI_STRING_H
            BetterStreamWriter& operator << (std::string &var) {
                for(std::string::iterator cur = var.begin(); cur != var.end(); ++cur) PutChar(*cur);
                return *this; }
#endif
        };
    }
#elif !defined(RUI_STDIO)
#warning "Because you did not define RUI_STDIO, therefore you cannot use file stream functions."
#elif __cplusplus < 201103L
#warning "Your C++ version is too low that some features cannot be used by you."
#endif
}

#endif
