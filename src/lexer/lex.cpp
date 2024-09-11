#include "lex.h"

namespace lex {
std::unordered_map<std::string, uint8_t> CharacterClass {
    {"",          0x00},
    {"A-Z",       0x01},
    {"a-z",       0x02},
    {"A-Za-z",    0x03},
    {"0-9",       0x04},
    {"A-Za-z0-9", 0x07},
    {"SP",        0x08},
    {"TAB",       0x10},
    {"NL",        0x20},
    {"WS",        0x00},
    {"_",         0x40},
    {"SYM",       0x80}    
};

std::unordered_map<uint8_t, std::function<bool(char)>> CheckClass {
    {0x01, [](char c){ return (c >= 'A' && c <= 'Z'); }},
    {0x02, [](char c){ return (c >= 'a' && c <= 'z'); }},
    {0x04, [](char c){ return (c >= '0' && c <= '9'); }},
    {0x08, [](char c){ return c == ' '; }},
    {0x10, [](char c){ return c == '\t'; }},
    {0x20, [](char c){ return c == '\n'; }},
    {0x40, [](char c){ return c == '_'; }},
    {0x80, [](char c){ return (
        (c >= ' ' && c < '0') || (c > '9' && c < 'A') ||
        (c > 'Z' && c < 'a') || (c > 'z' && c < 127)); }}
};

// should the inner map be CharacterClass -> LexerState?
// {LexerState::START,
//   {{CharacterClass["WS"], LexerState::WS},
//    {CharacterClass["0-9"], LexerState::NUM_NZ},
//    {CharacterClass["A-Za-z"], LexerState::IDENT},
// ?
std::unordered_map<LexerState, std::unordered_map<char, LexerState>> TransitionTable {
    {LexerState::START,
     {{{' ', LexerState::WS},
       {'0', LexerState::NUM_Z},
       {'1', LexerState::NUM_NZ},
       {'A', LexerState::IDENT},
       {'\"', LexerState::SYM_DQUO},
       {'.', LexerState::SYM_DOT},
       {'@', LexerState::SYM_AT},
       {':', LexerState::SYM_COL},
       {'~', LexerState::SYM_TL},
       {'+', LexerState::SYM_PM},
       {'-', LexerState::SYM_PM},
       {'?', LexerState::SYM_QUES},
       {'/', LexerState::SYM_FS},
       {'<', LexerState::SYM_XT},
       {'>', LexerState::SYM_XT},
       {'=', LexerState::SYM_EQ},
       {'!', LexerState::SYM_EX},
       {'{', LexerState::SYM_BRCO},
       {'}', LexerState::SYM_BRCC},
       {'(', LexerState::SYM_PARO},
       {')', LexerState::SYM_PARC},
       {'[', LexerState::SYM_BKTO},
       {']', LexerState::SYM_BKTC}}}},  
    
    {LexerState::WS,
     {{' ', LexerState::WS}}},

    {LexerState::NUM_Z,
     {{'0', LexerState::NUM_Z},
      {'1', LexerState::NUM_NZ},
      {'.', LexerState::NUM_FP},
      {'B', LexerState::NUM_B2_P},
      {'b', LexerState::NUM_B2_P},
      {'O', LexerState::NUM_B8_P},
      {'o', LexerState::NUM_B8_P},
      {'X', LexerState::NUM_B16_P},
      {'x', LexerState::NUM_B16_P},
      {'f', LexerState::NUM_PF}}},

    {LexerState::NUM_NZ,
     {{'0', LexerState::NUM_NZ},
      {'1', LexerState::NUM_NZ},
      {'.', LexerState::NUM_FP},
      {'f', LexerState::NUM_PF}}}, 
    
    {LexerState::NUM_FP,
     {{'0', LexerState::NUM_FP},
      {'1', LexerState::NUM_FP},
      {'f', LexerState::NUM_PF}}},    
        
    // for now we'll delay validation of different bases, until classification improves
    {LexerState::NUM_B2_P,
     {{'0', LexerState::NUM_B2},
      {'1', LexerState::NUM_B2}}},
    
    {LexerState::NUM_B2,
     {{'0', LexerState::NUM_B2},
      {'1', LexerState::NUM_B2}}},
    
    {LexerState::NUM_B8_P,
    {{'0', LexerState::NUM_B8},
     {'1', LexerState::NUM_B8}}},
    
    {LexerState::NUM_B8,
    {{'0', LexerState::NUM_B8},
     {'1', LexerState::NUM_B8}}},
    
    {LexerState::NUM_B16_P,
     {{'0', LexerState::NUM_B16},
      {'1', LexerState::NUM_B16},
      {'A', LexerState::NUM_B16}}},
    
    {LexerState::NUM_B16,
     {{'0', LexerState::NUM_B16},
      {'1', LexerState::NUM_B16},
      {'A', LexerState::NUM_B16}}},
    
    {LexerState::SYM_DOT,
     {{'0', LexerState::NUM_FP},
      {'1', LexerState::NUM_FP},
      {'>', LexerState::CHANNEL}}},
    
    {LexerState::SYM_AT,
    {{'!', LexerState::SIGNAL},
        {'=', LexerState::OVERLOAD},
        {'>', LexerState::CHANNEL}}},
    
    {LexerState::SYM_TL,
    {{'>', LexerState::CHANNEL},
     {'<', LexerState::CHANNEL}}},
    
    {LexerState::SYM_COL,
    {{'A', LexerState::CASE},
     {'0', LexerState::CASE},
     {'1', LexerState::CASE}}},

    {LexerState::CASE,
     {{'A', LexerState::CASE},
      {'0', LexerState::CASE},
      {'1', LexerState::CASE},
      {'>', LexerState::CHANNEL}}},

    {LexerState::SYM_QUES,
     {{'>', LexerState::CHANNEL}}},

    {LexerState::SYM_FS,
     {{'/', LexerState::COMMENT},
      {'>', LexerState::CHANNEL}}},

    {LexerState::SYM_PM,
     {{'>', LexerState::CHANNEL}}},
    
    {LexerState::SYM_SET,
     {{'=', LexerState::SYM_EQ}}},

    {LexerState::SYM_EX,   {}},
    {LexerState::NUM_PF,   {}},
    {LexerState::SYM_HASH, {}},
    {LexerState::SYM_DOLL, {}},
    {LexerState::SYM_MOD,  {}},
    {LexerState::SYM_UP,   {}},
    {LexerState::SYM_AND,  {}},
    {LexerState::SYM_STAR, {}},
    {LexerState::SYM_SEMI, {}},
    {LexerState::SYM_COM,  {}},
    {LexerState::SYM_XT,   {}},
    {LexerState::SYM_PIPE, {}},
    {LexerState::SYM_EQ,   {}},
    {LexerState::SYM_TICK, {}},
    {LexerState::SYM_PARO, {}},
    {LexerState::SYM_PARC, {}},
    {LexerState::SYM_BRCO, {}},
    {LexerState::SYM_BRCC, {}},
    {LexerState::SYM_BKTO, {}},
    {LexerState::SYM_BKTC, {}},
    {LexerState::COMMENT,  {}},
    {LexerState::CHANNEL,  {}},
    {LexerState::STR,      {}}
};
}

lex::Lexer::Lexer() : current_state(LexerState::START) { }
lex::Lexer::Lexer(const std::string& line) : current_state(LexerState::START) {}
void lex::Lexer::consume(const char c) {
    this->current_char = c;
    //....
}

bool IsInCClass(char c, uint8_t cclass) {
    bool inClass = true;
    while (cclass && inClass) {
        inClass &= lex::CheckClass[(cclass & (-cclass))](c);
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
