//
// Created by Tobiathan on 3/11/23.
//

#pragma once

class tloc {
public:
    static tloc current(const char* _file = __builtin_FILE(), unsigned int _line = __builtin_LINE()) {
        return tloc(_file, _line);
    }

    [[nodiscard]] const char* file_name() const { return _file; }
    [[nodiscard]] const char* function_name() const { return "we are doomed!"; }
    [[nodiscard]] unsigned int line() const { return _line; }
    [[nodiscard]] unsigned int column() const { return 0; }
private:
    explicit tloc(const char* _file, unsigned int _line) : _file(_file), _line(_line) {}

    const char* _file;
    unsigned int _line;
};