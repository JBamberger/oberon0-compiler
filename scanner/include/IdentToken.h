/*
 * Header file of the identifier tokens used by parser of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 2/27/18.
 */

#ifndef OBERON0C_IDENTTOKEN_H
#define OBERON0C_IDENTTOKEN_H

#include "Token.h"

class IdentToken final : public Token {

    std::string value_;

  public:
    explicit IdentToken(FilePos pos, const std::string& value);
    ~IdentToken() override;

    std::string getValue() const;

    void print(std::ostream& stream) const override;
};

#endif // OBERON0C_IDENTTOKEN_H
