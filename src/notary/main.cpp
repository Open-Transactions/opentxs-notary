// Copyright (c) 2011-2024 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <boost/program_options.hpp>
#include <opentxs/opentxs.hpp>
#include <iostream>
#include <string_view>
#include <utility>

namespace po = boost::program_options;

constexpr auto help_arg_ = "help";
constexpr auto only_init_arg_ = "only_init";
constexpr auto version_arg_ = "version";

struct Options {
    opentxs::Options args_{};
    bool show_help_{false};
    bool show_version_{false};
    bool only_init_{false};
    int network_{1};
};

auto options() noexcept -> const po::options_description&;
auto process_arguments(int argc, char** argv) noexcept -> Options;
auto read_options(int argc, char** argv) noexcept -> bool;
auto variables() noexcept -> po::variables_map&;

auto main(int argc, char* argv[]) -> int
{
    if (false == read_options(argc, argv)) { return 1; }

    const auto options = process_arguments(argc, argv);

    if (options.show_version_) {
        std::cout << "opentxs library-" << opentxs::VersionString() << '\n';

        return 0;
    }

    if (options.show_help_) {
        std::cout << ::options() << '\n' << options.args_.HelpText() << '\n';

        return 0;
    }

    opentxs::api::Context::PrepareSignalHandling();
    const auto& ot = opentxs::InitContext(options.args_);
    ot.StartNotarySession(options.args_, 0);

    if (options.only_init_) {
        opentxs::Cleanup();
    } else {
        ot.HandleSignals();
        opentxs::Join();
    }

    return 0;
}

auto options() noexcept -> const po::options_description&
{
    static const auto output = [] {
        auto out = po::options_description{"opentxs-notary options"};
        out.add_options()(help_arg_, "Display this message");
        out.add_options()(
            only_init_arg_, "Create or verify notary contract only");
        out.add_options()(version_arg_, "Show application version and exit");

        return out;
    }();

    return output;
}

auto process_arguments(int argc, char** argv) noexcept -> Options
{
    auto output = Options{};
    auto& args = output.args_;
    args.SetHome(opentxs::api::Context::SuggestFolder("opentxs-notary"));
    args.ParseCommandLine(argc, argv);

    for (const auto& [name, value] : variables()) {
        if (name == help_arg_) {
            output.show_help_ = true;
        } else if (name == version_arg_) {
            output.show_version_ = true;
        } else if (name == only_init_arg_) {
            output.only_init_ = true;
        }
    }

    return output;
}

auto read_options(int argc, char** argv) noexcept -> bool
{
    try {
        const auto parsed = po::command_line_parser(argc, argv)
                                .options(options())
                                .allow_unregistered()
                                .run();
        po::store(parsed, variables());
        po::notify(variables());

        return true;
    } catch (po::error& e) {
        std::cerr << "ERROR: " << e.what() << "\n\n" << options() << std::endl;

        return false;
    }
}

auto variables() noexcept -> po::variables_map&
{
    static auto output = po::variables_map{};

    return output;
}
