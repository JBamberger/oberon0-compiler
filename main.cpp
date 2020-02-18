/*
 * Main class of the Oberon-0 compiler.
 *
 * Created by Michael Grossniklaus on 12/14/17.
 */

#include "ParseException.h"
#include "Parser.h"
#include "PrintVisitor.h"
#include "Scanner.h"
#include <CodeGenerator.h>
#include <fstream>
#include <iostream>

int main(const int argc, const char* argv[])
{
    if (argc != 2 && argc != 3) {
        std::cout << "Usage: oberon0c <oberonfile> [<outputfile>]" << std::endl;
        return EXIT_FAILURE;
    }
    std::string filename = argv[1];
    auto logger = std::make_shared<Logger>();
    logger->setLevel(LogLevel::INFO);
    const auto scanner = std::make_shared<Scanner>(filename, logger.get());
    auto parser = std::make_unique<Parser>(scanner, logger);
    auto code_gen = CodeGenerator::create(Architecture::X86_64);

    std::unique_ptr<ModuleNode> tree;
    try {
        tree = parser->parse();
    } catch (const ParseException& e) {
        logger->error(e.position(), e.what());
        exit(EXIT_FAILURE);
    }

    //    std::cout << "Parse Tree:" << std::endl;
    //    const auto visitor = std::make_unique<PrintVisitor>(std::cout);
    //    tree->visit(visitor.get());

    std::ofstream of;
    std::ostream* output;
    if (argc == 3) {
        output = &of;
        of.open(argv[2]);
    } else {
        output = &std::cout;
    }

    code_gen->generate(std::move(tree), output);

    logger->info(filename, "Parsing complete.");
    exit(EXIT_SUCCESS);
}
