/*
 * Provided by Michael Grossniklaus and modified.
 */

#include "Scanner.h"
#include "IdentToken.h"
#include "NumberToken.h"
#include "StringToken.h"
#include <fstream>
#include <limits>
#include <utility>

Scanner::Scanner(std::string filename, const Logger* logger)
    : filename_(std::move(filename)), logger_(logger), token_(nullptr), lineNo_(1), charNo_(0),
      ch_(-1)
{
    this->initTable();
    file_in_ = std::make_unique<std::ifstream>();
    file_in_->open(filename_, std::ios::in);
    if (!file_in_->is_open()) {
        // TODO I/O Exception
        logger_->error(filename_, "Cannot open file.");
        exit(1);
    }
    file_ = file_in_.get();
    read();
}

Scanner::Scanner(std::istream* input, const Logger* logger)
    : filename_("StreamInput"), logger_(logger), token_(nullptr), lineNo_(1), charNo_(0),
      file_(input), ch_(-1)
{
    this->initTable();
    read();
}

Scanner::~Scanner()
{
    delete token_;
}

void Scanner::initTable()
{
    keywords_ = {{"DIV", TokenType::op_div},
                 {"MOD", TokenType::op_mod},
                 {"OR", TokenType::op_or},
                 {"MODULE", TokenType::kw_module},
                 {"PROCEDURE", TokenType::kw_procedure},
                 {"BEGIN", TokenType::kw_begin},
                 {"END", TokenType::kw_end},
                 {"WHILE", TokenType::kw_while},
                 {"DO", TokenType::kw_do},
                 {"IF", TokenType::kw_if},
                 {"THEN", TokenType::kw_then},
                 {"ELSE", TokenType::kw_else},
                 {"ELSIF", TokenType::kw_elsif},
                 {"VAR", TokenType::kw_var},
                 {"CONST", TokenType::kw_const},
                 {"TYPE", TokenType::kw_type},
                 {"ARRAY", TokenType::kw_array},
                 {"RECORD", TokenType::kw_record},
                 {"OF", TokenType::kw_of},
                 {"TRUE", TokenType::const_true},
                 {"FALSE", TokenType::const_false}};
}

const Token* Scanner::peekToken()
{
    if (token_ == nullptr) {
        token_ = this->next();
    }
    return token_;
}

std::unique_ptr<const Token> Scanner::nextToken()
{
    const Token* token;
    if (token_ != nullptr) {
        token = token_;
        token_ = nullptr;
    } else {
        token = this->next();
    }
    return std::unique_ptr<const Token>(token);
}

const Token* Scanner::next()
{
    const Token* token;
    // Skip whitespace
    while ((ch_ != -1) && (ch_ <= ' ')) {
        read();
    }
    if (ch_ != -1) {
        FilePos pos = getPosition();
        if (((ch_ >= 'A') && (ch_ <= 'Z')) || ((ch_ >= 'a') && (ch_ <= 'z'))) {
            // Scan identifier
            token = ident();
        } else if ((ch_ >= '0') && (ch_ <= '9')) {
            // Scan number
            token = new NumberToken(pos, number());
        } else {
            switch (ch_) {
            case '&':
                token = new Token(TokenType::op_and, pos);
                read();
                break;
            case '*':
                token = new Token(TokenType::op_times, pos);
                read();
                break;
            case '+':
                token = new Token(TokenType::op_plus, pos);
                read();
                break;
            case '-':
                token = new Token(TokenType::op_minus, pos);
                read();
                break;
            case '=':
                token = new Token(TokenType::op_eq, pos);
                read();
                break;
            case '#':
                token = new Token(TokenType::op_neq, pos);
                read();
                break;
            case '<':
                read();
                if (ch_ == '=') {
                    token = new Token(TokenType::op_leq, pos);
                    read();
                } else {
                    token = new Token(TokenType::op_lt, pos);
                }
                break;
            case '>':
                read();
                if (ch_ == '=') {
                    token = new Token(TokenType::op_geq, pos);
                    read();
                } else {
                    token = new Token(TokenType::op_gt, pos);
                }
                break;
            case ';':
                token = new Token(TokenType::semicolon, pos);
                read();
                break;
            case ',':
                token = new Token(TokenType::comma, pos);
                read();
                break;
            case ':':
                read();
                if (ch_ == '=') {
                    token = new Token(TokenType::op_becomes, pos);
                    read();
                } else {
                    token = new Token(TokenType::colon, pos);
                }
                break;
            case '.':
                token = new Token(TokenType::period, pos);
                read();
                break;
            case '(':
                read();
                if (ch_ == '*') {
                    comment();
                    token = next();
                } else {
                    token = new Token(TokenType::lparen, pos);
                }
                break;
            case ')':
                token = new Token(TokenType::rparen, pos);
                read();
                break;
            case '[':
                token = new Token(TokenType::lbrack, pos);
                read();
                break;
            case ']':
                token = new Token(TokenType::rbrack, pos);
                read();
                break;
            case '~':
                token = new Token(TokenType::op_not, pos);
                read();
                break;
            case '"':
                token = new StringToken(pos, string());
                read();
                break;
            default:
                token = nullptr;
                read();
                logger_->error(filename_, "Invalid character: " + std::to_string(ch_));
                throw std::exception();
            }
        }
    } else {
        token = new Token(TokenType::eof, getPosition());
    }
    return token;
}

void Scanner::read()
{
    if (ch_ == '\n') {
        lineNo_++;
        charNo_ = 0;
    }
    if (file_->get(ch_)) {
        charNo_++;
    } else if (file_->eof()) {
        charNo_++;
        ch_ = -1;
    } else {
        // TODO I/O Exception
        logger_->error(filename_, "Error reading file.");
    }
}

FilePos Scanner::getPosition() const
{
    FilePos pos;
    pos.fileName = filename_;
    pos.lineNo = lineNo_;
    pos.charNo = charNo_;
    return pos;
}

void Scanner::comment()
{
    const auto pos = getPosition();
    read();
    while (true) {
        while (true) {
            while (ch_ == '(') {
                read();
                if (ch_ == '*') {
                    comment();
                }
            }
            if (ch_ == '*') {
                read();
                break;
            }
            if (ch_ == -1) {
                break;
            }
            read();
        }
        if (ch_ == ')') {
            read();
            break;
        }
        if (ch_ == -1) {
            logger_->error(pos, "Comment not terminated.");
            break;
        }
    }
}

const Token* Scanner::ident()
{
    const auto pos = getPosition();
    std::stringstream ss;
    do {
        ss << ch_;
        read();
    } while (((ch_ >= '0') && (ch_ <= '9')) || ((ch_ >= 'a') && (ch_ <= 'z')) ||
             ((ch_ >= 'A') && (ch_ <= 'Z')));
    const auto ident = ss.str();
    std::unordered_map<std::string, TokenType>::const_iterator it = keywords_.find(ident);
    if (it != keywords_.end()) {
        return new Token(it->second, pos);
    }
    return new IdentToken(pos, ident);
}

int Scanner::number()
{
    auto isHex = false;
    auto decValue = 0;
    auto hexValue = 0;
    const auto pos = getPosition();
    do {
        isHex = isHex | ((ch_ >= 'A') && (ch_ <= 'F'));
        if (decValue <= ((std::numeric_limits<int>::max() - ch_ + '0') / 10)) {
            if ((ch_ >= '0') && (ch_ <= '9')) {
                decValue = 10 * decValue + (ch_ - '0');
                hexValue = 16 * hexValue + (ch_ - '0');
            } else { // A - F
                hexValue = 16 * hexValue + (ch_ - 'A' + 10);
            }
        } else {
            logger_->error(pos, "Number too large.");
            return 0;
        }
        read();
    } while (((ch_ >= '0') && (ch_ <= '9')) || ((ch_ >= 'A') && (ch_ <= 'F')));
    if (ch_ == 'H') {
        // hexadecimal number identified by trailing 'H'
        isHex = true;
        read();
    }
    if (isHex) {
        return hexValue;
    }
    return decValue;
}

std::string Scanner::string()
{
    std::stringstream ss;
    do {
        ss << ch_;
        if (ch_ == '\\') {
            read();
            ss << ch_;
        }
        read();
    } while (ch_ != '"');
    ss << ch_;
    return ss.str();
}
