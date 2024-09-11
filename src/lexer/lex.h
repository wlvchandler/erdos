#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <functional>

namespace lex { enum class LexerState : uint8_t; }
namespace lex { class Lexer; }

namespace lex {
extern std::unordered_map<std::string, uint8_t> CharacterClass;
extern std::unordered_map<uint8_t, std::function<bool(char)>> CheckClass;
extern std::unordered_map<LexerState, std::unordered_map<char, LexerState>> TransitionTable;
}

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
