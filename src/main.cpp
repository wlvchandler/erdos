#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <vector>
#include <format>
#include <algorithm>
#include <unordered_map>
#include "lexer/lex.h"

namespace fs = std::filesystem;

// DJB2
constexpr unsigned long long hash(std::string_view str) {
    unsigned long long hash = 5381;
    for (char c : str) {
        hash = hash * 33 + c;
    }
    return hash;
}

enum class TokenType {
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
    CHANNEL_INPUT,
    CHANNEL_OUTPUT,
    CHANNEL_FLOW,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    COLON,
    DOT,
    MODIFIER,
    AT_SYMBOL,
    STRUCT,
    NODE,
    BOOL,
    NULL_TOKEN,
    EOF_TOKEN
};


struct Token {
    TokenType type;
    std::string_view value;
    size_t line;
    size_t column;

    auto format() const {
        return std::format("Token({}, '{}', line: {}, column: {})", 
        static_cast<int>(type), value, line, column);
    }
};



struct HashEntry {
    unsigned long long hash;
    TokenType type;
};

template<std::size_t N>
constexpr auto create_table(const std::array<std::pair<std::string_view, TokenType>, N>& tokens) {
    std::array<HashEntry, N> table{};
    for (size_t i = 0; i < N; ++i) {
        table[i] = HashEntry{hash(tokens[i].first), tokens[i].second};
    }
    return table;
}

constexpr auto create_keyword_table() {
    constexpr std::array tokens {
        std::pair{std::string_view("struct"), TokenType::STRUCT},
        std::pair{std::string_view("null"), TokenType::BOOL},
        std::pair{std::string_view("false"), TokenType::BOOL},
        std::pair{std::string_view("null"), TokenType::NULL_TOKEN}
    };

    return create_table(tokens);
}

constexpr auto create_operator_table() {
    constexpr std::array tokens {
        std::pair{std::string_view("<~"), TokenType::CHANNEL_INPUT},
        std::pair{std::string_view("~>"), TokenType::CHANNEL_OUTPUT},
        std::pair{std::string_view("->"), TokenType::CHANNEL_FLOW},
        std::pair{std::string_view("=="), TokenType::OPERATOR},
        std::pair{std::string_view("+"), TokenType::OPERATOR},
        std::pair{std::string_view("*"), TokenType::OPERATOR},
        std::pair{std::string_view("/"), TokenType::OPERATOR}
    };
    return create_table(tokens);
}


constexpr auto create_single_char_table() {
    constexpr std::array tokens {
        std::pair{std::string_view("("), TokenType::LPAREN},
        std::pair{std::string_view(")"), TokenType::RPAREN},
        std::pair{std::string_view("["), TokenType::LBRACKET},
        std::pair{std::string_view("]"), TokenType::RBRACKET},
        std::pair{std::string_view("{"), TokenType::LBRACE},
        std::pair{std::string_view("}"), TokenType::RBRACE},
        std::pair{std::string_view(":"), TokenType::COLON},
        std::pair{std::string_view("."), TokenType::DOT},
        std::pair{std::string_view("!"), TokenType::MODIFIER},
        std::pair{std::string_view("%"), TokenType::MODIFIER},
        std::pair{std::string_view("?"), TokenType::MODIFIER},
        std::pair{std::string_view("@"), TokenType::AT_SYMBOL}
    };
    return create_table(tokens);
} 


constexpr auto operator_table = create_operator_table();
constexpr auto keyword_table = create_keyword_table();
constexpr auto single_char_table = create_single_char_table();




class Lexer {
    std::string_view input_;
    size_t position_ = 0;
    size_t line_ = 1;
    size_t column_ = 1;

  public:
    Lexer(std::string_view input) : input_(input) {
        for (char c : input_) {
            std::cout << "[Lexer]\tlex_char(" << c << ")\n";
        }
        std::cout<<std::endl;
    }
    
    template<typename Table>
    TokenType get_token_type(std::string_view input, const Table& table) const {
        unsigned long long input_hash = hash(input);
        auto it = std::find_if(table.begin(), table.end(), [input_hash](const auto& entry) {
            return entry.hash == input_hash;
        });
        return it != table.end() ? it->type : TokenType::IDENTIFIER;
    }
    
    TokenType get_operator_type(std::string_view op) const {
        return get_token_type(op, operator_table);
    }
    
    TokenType get_single_char_type(std::string_view c) const {
        return get_token_type(c, single_char_table);
    }
    
    TokenType get_keyword_type(std::string_view word) const {
        return get_token_type(word, keyword_table);
    }

};



int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "No input file provided." << std::endl;
        return 1;
    }
    
    fs::path filepath = argv[1];
    if (!fs::exists(filepath)) {
        std::cerr << "File doesn't exist." << std::endl;
        return 1;
    }
    
    try {
        std::ifstream f(filepath, std::ios::binary | std::ios::ate);
        if (!f) {
            throw std::runtime_error("Unable to open file");
        }
        // may end up not needing this. we could just open with std::ios::in in that case.
        std::streamsize sz = f.tellg();
        f.seekg(0, std::ios::beg);

        // note: if lookahead is needed we'll use an mmap approach. for now - easy is best
        std::string line;
        size_t line_n = 0, token_n = 0;
        Lexer lexer("");
        while (std::getline(f, line)) {
            ++line_n;
            if (!line.length()) continue;
            
            std::cout << "Lexer: ln:" << line_n << ": " << line <<std::endl;
            lexer = Lexer(line);
        }
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl; // TODO: improve
        return 1;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl; // TODO: improve
        return 1;
    }
    
    return 0;
}
