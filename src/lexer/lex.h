#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>


namespace lex {
enum class LexerState : uint8_t {
    START       = 0x00,  // FSM start state
    ERROR       = 0x01,  // no valid state found
    COMMENT     = 0x02,  // comment
    CHANNEL     = 0x03,  // channel operator
    OVERLOAD    = 0x04,  // node overload symbol     - @=
    SIGNAL      = 0x05,  // channel signal accessor  - @!
    CASE        = 0x06,  // switch case for channel
    WS          = 0x07,  // whitespace
    IDENT       = 0x08,  // identity token
    STR         = 0x09,  // string
    NUM_Z       = 0x10,  // number: zero
    NUM_NZ      = 0x11,  // number: nonzero
    NUM_B2_P    = 0x12,  // binary prefix (nonaccepting)
    NUM_B2      = 0x13,  // number: binary
    NUM_B8_P    = 0x14,  // octal prefix (nonaccepting)
    NUM_B8      = 0x15,  // number: octal
    NUM_B16_P   = 0x16,  // hex prefix (nonaccepting)
    NUM_B16     = 0x17,  // number: hex
    NUM_FP      = 0x19,  // number: floating point
    NUM_PF      = 0x1A,  // number: postfixed (e.g. 16h, 10.0f)
    STR_ESC     = 0x20,  // backslash
    STR_ESC_Q   = 0x21,  // escaped double quote
    SYM_SQUO    = 0x30,  //  '
    SYM_DQUO    = 0x31,  //  "
    SYM_TICK    = 0x32,  //  `
    SYM_DOT     = 0x33,  //  .
    SYM_COM     = 0x34,  //  ,
    SYM_SEMI    = 0x35,  //  ;
    SYM_COL     = 0x36,  //  :
    SYM_XT      = 0x40,  //  <>
    SYM_SET     = 0x41,  //  =
    SYM_EQ      = 0x42,  //  =
    SYM_EX      = 0x43,  //  !
    SYM_QUES    = 0x44,  //  ?
    SYM_TL      = 0x45,  //  ~
    SYM_PM      = 0x50,  //  +-
    SYM_STAR    = 0x51,  //  *
    SYM_FS      = 0x52,  //  /
    SYM_AT      = 0x60,  //  @
    SYM_HASH    = 0x61,  //  #
    SYM_DOLL    = 0x62,  //  $
    SYM_MOD     = 0x63,  //  %
    SYM_UP      = 0x64,  //  ^
    SYM_AND     = 0x65,  //  &
    SYM_PIPE    = 0x66,  //  |
    SYM_BRCO    = 0x90,  //  {
    SYM_BRCC    = 0x91,  //  }
    SYM_PARO    = 0x92,  //  (
    SYM_PARC    = 0x93,  //  )
    SYM_BKTO    = 0x94,  //  [
    SYM_BKTC    = 0x95   //  ]
};

const std::unordered_map<LexerState, std::unordered_map<char, LexerState>> TransitionTable {
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

//enum class CharacterClass : uint8_t {};

constexpr std::unordered_map<std::string, uint8_t> CharacterClass {
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
    {"_",         0x00},
    {"SYM",       0x00},
    
};

const std::unordered_map<uint8_t, std::function<bool(char)>> CheckClass {
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


bool IsInClass(char, uint8_t);

class Lexer {
    bool ready;
    char current_char;
    LexerState current_state;
    std::vector<char> currentLexeme;    
    uint8_t classify(const char c);
    
  public:
    Lexer();
    Lexer(const char);
    Lexer(const std::string&);
    
    //bool ready() const;
    void consume(const char);
    //Token emit();
};

}
