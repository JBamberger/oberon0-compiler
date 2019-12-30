#include "DebugLogger.h"
#include "Parser.h"
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <ostream>
#include <string>
#include <utility>

struct test_spec {
    const std::string msg;
    const std::string description; // test case name

    test_spec(std::string m, std::string description)
        : msg(std::move(m)), description(std::move(description))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const test_spec& obj)
    {
        return os << obj.description;
    }
};

struct ErrorTest : public testing::TestWithParam<test_spec> {
    std::shared_ptr<DebugLogger> logger;

    ErrorTest() { logger = std::make_shared<DebugLogger>(); }
};

TEST_P(ErrorTest, testSuccess)
{
    std::cout << "hello" << std::endl;
    const auto params = GetParam();
    std::stringstream in(params.msg);
    const auto scanner = std::make_shared<Scanner>(&in, logger.get());
    auto parser = std::make_shared<Parser>(scanner.get(), logger.get());


    static_cast<void>(parser->parse());
    const auto& e = logger->getErrors();
    assert(e.size() == 1);
    EXPECT_EQ(e[0], params.msg);
    std::cout << "hello2" << std::endl;
}

INSTANTIATE_TEST_SUITE_P(ExpectThrow, ErrorTest, testing::Values(test_spec("MODULE test; VAR a: ; BEGIN a := 1 END test.", "")));
