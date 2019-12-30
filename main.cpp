/*
 * Main class of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 12/14/17.
 */

#include "ParseException.h"
#include "Parser.h"
#include "PrintVisitor.h"
#include "Scanner.h"
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
    const auto scanner = std::make_unique<Scanner>(filename, logger.get());
    auto parser = std::make_unique<Parser>(scanner.get(), logger.get());

    std::unique_ptr<Node> tree;
    try {
        tree = parser->parse();
    } catch (const ParseException& e) {
        logger->error(e.position(), e.what());
        exit(EXIT_FAILURE);
    }

    std::cout << "Parse Tree:" << std::endl;
    const auto visitor = std::make_unique<PrintVisitor>(std::cout);
    tree->visit(visitor.get());

    logger->info(filename, "Parsing complete.");
    exit(EXIT_SUCCESS);
}
