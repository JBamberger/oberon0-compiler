#include "ParseException.h"
#include "Parser.h"
#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <ostream>
#include <string>
#include <utility>

#include "Logger.h"

class DebugLogger : public Logger {
  protected:
    void log(LogLevel, const std::string&, int, int, const std::string&) const override {}

  public:
    DebugLogger() = default;
};

struct test_spec {
    const std::string code;
    const std::string msg;

    test_spec(std::string code, std::string message)
        : code(std::move(code)), msg(std::move(message))
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const test_spec& obj)
    {
        return os << obj.code;
    }
};

struct ErrorTest : public testing::TestWithParam<test_spec> {
    std::shared_ptr<DebugLogger> logger;

    ErrorTest() { logger = std::make_shared<DebugLogger>(); }
};

TEST_P(ErrorTest, testSuccess)
{
    const auto params = GetParam();
    std::stringstream in(params.code);
    const auto scanner = std::make_shared<Scanner>(&in, logger.get());
    auto parser = std::make_shared<Parser>(scanner.get(), logger.get());

    try {
        static_cast<void>(parser->parse());
    } catch (const ParseException& e) {
        EXPECT_EQ(e.what(), params.msg);
        return;
    }
    FAIL();
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(ExpectThrow, ErrorTest, testing::Values(
    //test_spec("", getErrMsg(error_id::E000, "expected", "actual")), //TODO
    test_spec("MODULE test; VAR a, a: INTEGER; BEGIN a := 1 END test.", getErrMsg(error_id::E001, "a")),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p; CONST b = a; BEGIN a := 1; END p; BEGIN a := 1 END test.", getErrMsg(error_id::E002)),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p; TYPE x = ARRAY a OF INTEGER; BEGIN a := 1; END p; BEGIN a := 1 END test.", getErrMsg(error_id::E003)),
    test_spec("MODULE test; TYPE a = ARRAY -1 OF INTEGER; BEGIN a := 1 END test.", getErrMsg(error_id::E004, -1)),
    test_spec("MODULE test; VAR a : INTEGER; b: a; BEGIN a := 1 END test.", getErrMsg(error_id::E005, "a")),
    //test_spec("", getErrMsg(error_id::E006)), // unused
    //test_spec("", getErrMsg(error_id::E007)), // unused
    //test_spec("", getErrMsg(error_id::E008)), // unused
    //test_spec("", getErrMsg(error_id::E009)), // unused
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p(VAR x: ARRAY 2 OF INTEGER); BEGIN a := 0 END p; BEGIN a := 0 END test.", getErrMsg(error_id::E010)),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p(VAR x: RECORD a :INTEGER END); BEGIN a := 0 END p; BEGIN a := 0 END test.", getErrMsg(error_id::E011)),
    test_spec("MODULE test; VAR a: BOOLEAN; BEGIN a := a + a END test.", getErrMsg(error_id::E012, "BOOLEAN")),
    test_spec("MODULE test; VAR a: INTEGER; b: BOOLEAN; BEGIN a := a < b END test.", getErrMsg(error_id::E013, "INTEGER", "BOOLEAN")),
    test_spec("MODULE test; VAR a: INTEGER; BEGIN a := a & a END test.", getErrMsg(error_id::E014, "INTEGER")),
    test_spec("MODULE test; BEGIN a := 1 END test.", getErrMsg(error_id::E015, "a")),
    test_spec("MODULE test; VAR a: INTEGER; BEGIN a := a[1] END test.", getErrMsg(error_id::E016, "INTEGER")),
    test_spec("MODULE test; VAR a: ARRAY 1 OF INTEGER; BEGIN a[2] := 1 END test.", getErrMsg(error_id::E017, 2, 1)),
    test_spec("MODULE test; VAR a: INTEGER; BEGIN a.b := 1 END test.", getErrMsg(error_id::E018, "INTEGER")),
    test_spec("MODULE test; VAR a: RECORD b: INTEGER; END; BEGIN a.a := 1 END test.", getErrMsg(error_id::E015, "a", "[R;b,INTEGER;]")),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p(); BEGIN a := 0 END p; BEGIN p(1) END test.", getErrMsg(error_id::E020, 1, 0, "p")),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p(x: BOOLEAN); BEGIN a := 0 END p; BEGIN p(0) END test.", getErrMsg(error_id::E021, "BOOLEAN", "INTEGER")),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p(VAR x: INTEGER); BEGIN a := 0 END p; BEGIN p(0) END test.", getErrMsg(error_id::E022, "x")),
    test_spec("MODULE test; TYPE a = INTEGER; VAR b: INTEGER; BEGIN b := a END test.", getErrMsg(error_id::E023, "a")),
    test_spec("MODULE test; CONST a = 0; BEGIN a := 1 END test.", getErrMsg(error_id::E024)),
    test_spec("MODULE test; VAR a: BOOLEAN; BEGIN a := 1 END test.", getErrMsg(error_id::E025, "BOOLEAN", "INTEGER")),
    test_spec("MODULE test; VAR a: INTEGER; BEGIN IF 1 THEN a := 1 END END test.", getErrMsg(error_id::E026, "INTEGER")),
    test_spec("MODULE test; VAR a: INTEGER; BEGIN IF 1 = 1 THEN a := 1 ELSIF 1 THEN a := 1 END END test.", getErrMsg(error_id::E026, "INTEGER")),
    test_spec("MODULE test; BEGIN WHILE 1 DO a := 1 END END test.", getErrMsg(error_id::E027, "INTEGER")),
    test_spec("MODULE t1; VAR a: INTEGER; BEGIN a := 1 END t2.", getErrMsg(error_id::E028, "t1", "t2")),
    test_spec("MODULE test; VAR a: INTEGER; PROCEDURE p1; BEGIN a := 1 END p2; BEGIN a := 1 END test.", getErrMsg(error_id::E029, "p1", "p2")),
    test_spec("MODULE test; VAR a: ARRAY 1 OF INTEGER; BEGIN a[1>2] := 1 END test.", getErrMsg(error_id::E030, "BOOLEAN")),
    test_spec("MODULE test; VAR a: INTEGER; BEGIN a END test.", getErrMsg(error_id::E031, "a"))
));
// clang-format on
