#include "lex.h"


lex::Lexer::Lexer() { }

lex::Lexer::Lexer(const std::string& line) : current_state(LexerState::START) {}

// bool lex::Lexer::ready() {
//     return this->ready;
// }

void lex::Lexer::consume(const char c) {
    this->current_char = c;
    //uint8_t chclass = classify(c);
    
}


bool lex::IsInCClass(char c, uint8_t cclass) {
    bool inClass = true;
    while (cclass && inClass) {
        inClass &= CheckClass[(cclass & (-cclass))](c);
        cclass &= (cclass - 1);
    }
    return inClass;
}

// Token lex::Lexer::emit() {
    
// }


// collapse some characters into classifications
// TODO: add logic to collapse based on previous token (e.g. symbols in strings)?
uint8_t lex::Lexer::classify(const char c) {
    uint8_t chclass = static_cast<uint8_t>(c);
    if (chclass == 0x9 || chclass == 0xA || chclass == 0x20) {
        chclass = 0x20;
    }
    
    if (chclass > '0' && chclass <= '9') {
        chclass = static_cast<uint8_t>('1');
    }
    
    if ((chclass >= 'A' && chclass <= 'Z') || (chclass >= 'a' && chclass <= 'z') || chclass == '_') {
        chclass = static_cast<uint8_t>('A'); 
    }
    return chclass;
}
