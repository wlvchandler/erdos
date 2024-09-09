#include "lex.h"


lex::Lexer Lexer() { }

lex::Lexer Lexer(const std::string& line) {
    currentState = LexerState::START;
}

bool lex::Lexer::ready() {
    return this->ready;
}

void lex::Lexer::consume(const char c) {
    this->current_char = c;

    uint8_t chclass = classify(c);
    
    switch (chclass) {
        case 0x20:
            this->current_state = LexerToken::WS;
            break;
        case '0':
            this->current_state = LexerToken::NUM_Z;
            break;
        case '1':
            this->current_state = LexerToken::NUM_NZ;
            break;
    }
}

Token lex::Lexer::emit() {
    
}


// collapse some characters into classifications
inline uint8_t lex::Lexer::classify() {
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
