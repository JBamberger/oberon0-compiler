/*
 * Main class of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 12/14/17.
 */

#include "parser/Parser.h"
#include "parser/PrintVisitor.h"
#include "scanner/Scanner.h"
#include <iostream>

int main(const int argc, const char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: oberon0c <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    auto logger = std::make_unique<Logger>();
    logger->setLevel(LogLevel::DEBUG);
    auto scanner = std::make_unique<Scanner>(filename, logger.get());
    auto parser = std::make_unique<Parser>(scanner.get(), logger.get());

    std::cout << "Parse Tree:" << std::endl;
    const auto tree = parser->parse();

    const auto visitor = std::make_unique<PrintVisitor>(std::cout);

    tree->visit(visitor.get());

    logger->info(filename, "Parsing complete.");
    exit(0);
}
