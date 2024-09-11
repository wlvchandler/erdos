#include "lex.h"


lex::Lexer::Lexer() : current_state(LexerState::Start) { }
lex::Lexer::Lexer(const std::string& line) : current_state(LexerState::START) {}

void lex::Lexer::consume(const char c) {
    this->current_char = c;
    //....
}


bool lex::IsInCClass(char c, uint8_t cclass) {
    bool inClass = true;
    while (cclass && inClass) {
        inClass &= CheckClass[(cclass & (-cclass))](c);
        cclass &= (cclass - 1);
    }
    return inClass;
}

// collapse some characters into classifications
// TODO: add logic to collapse based on previous token (e.g. symbols in strings)?
uint8_t lex::Lexer::classify(const char c) {
    uint8_t chclass = 0;
    for (const auto& [bit, check] : CheckClass) {
        if (check(c)) {
            chclass |= bit;
        }
    }
    return chclass;
}
