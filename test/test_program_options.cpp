/**
 * @file
 * @brief Tests for program_options.hpp
 * @copyright Copyright (c) 2022, Max Planck Gesellschaft.
 */
#include <gtest/gtest.h>

#include <cli_utils/program_options.hpp>

using namespace cli_utils;

class TestArgs : public ProgramOptions
{
public:
    std::string string_arg;
    int int_arg;
    int optional_arg = 42;

    std::string help() const override
    {
        return "Help";
    }

    void add_options(boost::program_options::options_description& options,
                     boost::program_options::positional_options_description&
                         positional) override
    {
        namespace po = boost::program_options;
        // clang-format off
            options.add_options()
                ("string-arg",
                 po::value<std::string>(&string_arg)->required(),
                 "Help for string-arg")
                ("int-arg,i",
                 po::value<int>(&int_arg)->required(),
                 "Help for int-arg")
                ("optional,o",
                 po::value<int>(&optional_arg),
                 "Help for optional arg")
                ;
        // clang-format on

        // string-arg is positional
        positional.add("string-arg", 1);
    }
};

TEST(TestCliUtils, program_options_long_no_optional)
{
    TestArgs args;
    const char* argv[] = {"test", "hello", "--int-arg", "13"};
    bool success = args.parse_args(4, argv);
    ASSERT_TRUE(success);

    EXPECT_EQ(args.string_arg, "hello");
    EXPECT_EQ(args.int_arg, 13);
    EXPECT_EQ(args.optional_arg, 42);
}

TEST(TestCliUtils, program_options_long_with_optional)
{
    TestArgs args;
    const char* argv[] = {"test", "hello", "--int-arg", "13", "-o", "23"};
    bool success = args.parse_args(6, argv);
    ASSERT_TRUE(success);

    EXPECT_EQ(args.string_arg, "hello");
    EXPECT_EQ(args.int_arg, 13);
    EXPECT_EQ(args.optional_arg, 23);
}

TEST(TestCliUtils, program_options_short)
{
    TestArgs args;
    const char* argv[] = {"test", "hello", "-i", "13", "--optional", "23"};
    bool success = args.parse_args(6, argv);
    ASSERT_TRUE(success);

    EXPECT_EQ(args.string_arg, "hello");
    EXPECT_EQ(args.int_arg, 13);
    EXPECT_EQ(args.optional_arg, 23);
}

TEST(TestCliUtils, program_options_missing_required)
{
    TestArgs args;
    const char* argv[] = {"test", "hello"};
    bool success = args.parse_args(2, argv);
    ASSERT_FALSE(success);
}

TEST(TestCliUtils, program_options_missing_positional)
{
    TestArgs args;
    const char* argv[] = {"test", "-i", "13"};
    bool success = args.parse_args(3, argv);
    ASSERT_FALSE(success);
}

TEST(TestCliUtils, program_options_unknown)
{
    TestArgs args;
    const char* argv[] = {"test", "-x", "13"};
    bool success = args.parse_args(3, argv);
    ASSERT_FALSE(success);
}
