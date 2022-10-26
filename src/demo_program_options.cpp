/**
 * @file
 * @brief Simple example on how to use ProgramOptions
 * @copyright 2022, Max Planck Gesellschaft.  All rights reserved.
 * @license BSD 3-clause
 *
 * To use it, one needs to implement a custom class which is derived from
 * cli_utils::ProgramOptions.  In this class, member variables for the desired
 * arguments are declared (potentially with default values), as well as methods
 * for providing a help text and to define the actual command line arguments.
 *
 * An instance of this custom class is then used to parse the command line
 * arguments and to access the values.
 *
 * See ``demo_program_options --help`` for the auto-generated help.
 */
#include <cli_utils/program_options.hpp>

class Args : public cli_utils::ProgramOptions
{
public:
    // declare a variable for each argument
    std::string message;
    int repetitions;
    int optional_arg = 42;
    bool flag;

    // help() should return the text shown when called with `--help`
    std::string help() const override
    {
        return R"(Simple demo on how to use cli_utils::ProgramOptions.

        It prints a given message multiple times.

Usage:  demo_program_options [options] <message>

)";
    }

    // in add_options the arguments are defined
    void add_options(boost::program_options::options_description &options,
                     boost::program_options::positional_options_description
                         &positional) override
    {
        namespace po = boost::program_options;

        // The chaining of parentheses calls does not go well with clang-format,
        // so better disable auto-formatting for this block.

        // clang-format off
        options.add_options()
            ("message",
             po::value<std::string>(&message)->required(),
             "Message that is printed.")
            ("repetitions,n",  // Can use either "--repetitions 5" or "-n 5"
             po::value<int>(&repetitions)->required(),
             "How often to print the message")
            ("optional",
             po::value<int>(&optional_arg),
             "This is an optional argument with default value.")
            ("flag",
             "A flag without value.")

            ;
        // clang-format on

        // mark 'message' as positional argument
        positional.add("message", 1);
    }

    // for boolean flags without values, some post-processing is needed
    void postprocess(const boost::program_options::variables_map &args) override
    {
        // check if --flag has been set
        flag = args.count("flag") > 0;
    }
};

int main(int argc, char **argv)
{
    // read command line arguments
    Args args;
    if (!args.parse_args(argc, argv))
    {
        return 1;
    }

    for (int i = 0; i < args.repetitions; ++i)
    {
        std::cout << args.message << std::endl;
    }

    std::cout << "\n\nValue of the optional argument: " << args.optional_arg
              << std::endl;
    if (args.flag)
    {
        std::cout << "--flag has been set" << std::endl;
    }
}
