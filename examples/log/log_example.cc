//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

// clog usage example

#include <cstdio>
#include <chrono>

void load_levels_example();

void stdout_logger_example();

void basic_example();

void rotating_example();

void daily_example();

void callback_example();

void async_example();

void binary_example();

void vector_example();

void stopwatch_example();

void trace_example();

void multi_sink_example();

void user_defined_example();

void err_handler_example();

void syslog_example();

void udp_example();

void custom_flags_example();

void file_events_example();

void replace_default_logger_example();

#include <collie/log/clog.h>
#include <collie/log/cfg/env.h>   // support for loading levels from the environment variable
#include <collie/strings/fmt/ostream.h>
#include <collie/strings/fmt/ranges.h>
#include <collie/log/bin_to_hex.h>

int main(int, char *[]) {
    // Log levels can be loaded from argv/env using "CLOG_LEVEL"
    load_levels_example();

    collie::log::info("Welcome to clog version {}.{}.{}  !", CLOG_VER_MAJOR, CLOG_VER_MINOR,
                 CLOG_VER_PATCH);

    collie::log::warn("Easy padding in numbers like {:08d}", 12);
    collie::log::fatal("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    collie::log::info("Support for floats {:03.2f}", 1.23456);
    collie::log::info("Positional args are {1} {0}..", "too", "supported");
    collie::log::info("{:>8} aligned, {:<8} aligned", "right", "left");

    // Runtime log levels
    collie::log::set_level(collie::log::level::info);  // Set global log level to info
    collie::log::debug("This message should not be displayed!");
    collie::log::set_level(collie::log::level::trace);  // Set specific logger's log level
    collie::log::debug("This message should be displayed..");

    // Customize msg format for all loggers
    collie::log::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    collie::log::info("This an info message with custom format");
    collie::log::set_pattern("%+");  // back to default format
    collie::log::set_level(collie::log::level::info);

    // Backtrace support
    // Loggers can store in a ring buffer all messages (including debug/trace) for later inspection.
    // When needed, call dump_backtrace() to see what happened:
    collie::log::enable_backtrace(10);  // create ring buffer with capacity of 10  messages
    for (int i = 0; i < 100; i++) {
        collie::log::debug("Backtrace message {}", i);  // not logged..
    }
    // e.g. if some error happened:
    collie::log::dump_backtrace();  // log them now!

    try {
        stdout_logger_example();
        basic_example();
        rotating_example();
        daily_example();
        callback_example();
        async_example();
        binary_example();
        vector_example();
        multi_sink_example();
        user_defined_example();
        err_handler_example();
        trace_example();
        stopwatch_example();
        udp_example();
        custom_flags_example();
        file_events_example();
        replace_default_logger_example();

        // Flush all *registered* loggers using a worker thread every 3 seconds.
        // note: registered loggers *must* be thread safe for this to work correctly!
        collie::log::flush_every(std::chrono::seconds(3));

        // Apply some function on all registered loggers
        collie::log::apply_all([&](std::shared_ptr<collie::log::logger> l) { l->info("End of example."); });

        // Release all clog resources, and drop all loggers in the registry.
        // This is optional (only mandatory if using windows + async log).
        collie::log::shutdown();
    }

        // Exceptions will only be thrown upon failed logger or sink construction (not during logging).
    catch (const collie::log::CLogEx &ex) {
        std::printf("Log initialization failed: %s\n", ex.what());
        return 1;
    }
}

#include <collie/log/sinks/stdout_color_sinks.h>

// or #include <collie/log/sinks/stdout_sinks.h> if no colors needed.
void stdout_logger_example() {
    // Create color multi threaded logger.
    auto console = collie::log::stdout_color_mt("console");
    // or for stderr:
    // auto console = collie::log::stderr_color_mt("error-logger");
}

#include <collie/log/sinks/basic_file_sink.h>

void basic_example() {
    // Create basic file logger (not rotated).
    auto my_logger = collie::log::basic_logger_mt("file_logger", "logs/basic-log.txt", true);
}

#include <collie/log/sinks/rotating_file_sink.h>

void rotating_example() {
    // Create a file rotating logger with 5mb size max and 3 rotated files.
    auto rotating_logger =
            collie::log::rotating_logger_mt("some_logger_name", "logs/rotating.txt", 1048576 * 5, 3);
}

#include <collie/log/sinks/daily_file_sink.h>

void daily_example() {
    // Create a daily logger - a new file is created every day on 2:30am.
    auto daily_logger = collie::log::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
}

#include <collie/log/sinks/callback_sink.h>

void callback_example() {
    // Create the logger
    auto logger = collie::log::callback_logger_mt("custom_callback_logger",
                                             [](const collie::log::details::log_msg & /*msg*/) {
                                                 // do what you need to do with msg
                                             });
}

#include <collie/log/cfg/env.h>

void load_levels_example() {
    // Set the log level to "info" and mylogger to "trace":
    // CLOG_LEVEL=info,mylogger=trace && ./example
    collie::log::cfg::load_env_levels();
    // or from command line:
    // ./example CLOG_LEVEL=info,mylogger=trace
    // #include <collie/log/cfg/argv.h> // for loading levels from argv
    // collie::log::cfg::load_argv_levels(args, argv);
}

#include <collie/log/async.h>

void async_example() {
    // Default thread pool settings can be modified *before* creating the async logger:
    // collie::log::init_thread_pool(32768, 1); // queue with max 32k items 1 backing thread.
    auto async_file =
            collie::log::basic_logger_mt<collie::log::async_factory>("async_file_logger", "logs/async_log.txt");
    // alternatively:
    // auto async_file =
    // collie::log::create_async<collie::log::sinks::basic_file_sink_mt>("async_file_logger",
    // "logs/async_log.txt");

    for (int i = 1; i < 101; ++i) {
        async_file->info("Async message #{}", i);
    }
}

// Log binary data as hex.
// Many types of std::container<char> types can be used.
// Iterator ranges are supported too.
// Format flags:
// {:X} - print in uppercase.
// {:s} - don't separate each byte with space.
// {:p} - don't print the position on each line start.
// {:n} - don't split the output to lines.

void binary_example() {
    std::vector<char> buf;
    for (int i = 0; i < 80; i++) {
        buf.push_back(static_cast<char>(i & 0xff));
    }
    collie::log::info("Binary example: {}", collie::log::to_hex(buf));
    collie::log::info("Another binary example:{:n}",
                 collie::log::to_hex(std::begin(buf), std::begin(buf) + 10));
    // more examples:
    // logger->info("uppercase: {:X}", collie::log::to_hex(buf));
    // logger->info("uppercase, no delimiters: {:Xs}", collie::log::to_hex(buf));
    // logger->info("uppercase, no delimiters, no position info: {:Xsp}", collie::log::to_hex(buf));
    // logger->info("hexdump style: {:a}", collie::log::to_hex(buf));
    // logger->info("hexdump style, 20 chars per line {:a}", collie::log::to_hex(buf, 20));
}

void vector_example() {
    std::vector<int> vec = {1, 2, 3};
    collie::log::info("Vector example: {}", vec);
}

// Compile time log levels.
// define CLOG_ACTIVE_LEVEL to required level (e.g. CLOG_LEVEL_TRACE)
void trace_example() {
    // trace from default logger
    CLOG_TRACE("Some trace message.. {} ,{}", 1, 3.23);
    // debug from default logger
    CLOG_DEBUG("Some debug message.. {} ,{}", 1, 3.23);

    // trace from logger object
    auto logger = collie::log::get("file_logger");
    CLOG_LOGGER_TRACE(logger, "another trace message");
}

// stopwatch example
#include <collie/log/stopwatch.h>
#include <thread>

void stopwatch_example() {
    collie::log::stopwatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(123));
    collie::log::info("Stopwatch: {} seconds", sw);
}

#include <collie/log/sinks/udp_sink.h>

void udp_example() {
    collie::log::sinks::udp_sink_config cfg("127.0.0.1", 11091);
    auto my_logger = collie::log::udp_logger_mt("udplog", cfg);
    my_logger->set_level(collie::log::level::debug);
    my_logger->info("hello world");
}

// A logger with multiple sinks (stdout and file) - each with a different format and log level.
void multi_sink_example() {
    auto console_sink = std::make_shared<collie::log::sinks::stdout_color_sink_mt>();
    console_sink->set_level(collie::log::level::warn);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink =
            std::make_shared<collie::log::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    file_sink->set_level(collie::log::level::trace);

    collie::log::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_level(collie::log::level::debug);
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");
}

// User defined types logging
struct my_type {
    int i = 0;

    explicit my_type(int i)
            : i(i) {};
};

template<>
struct fmt::formatter<my_type> : fmt::formatter<std::string> {
    auto format(my_type my, format_context &ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "[my_type i={}]", my.i);
    }
};


void user_defined_example() { collie::log::info("user defined type: {}", my_type(14)); }

// Custom error handler. Will be triggered on log failure.
void err_handler_example() {
    // can be set globally or per logger(logger->set_error_handler(..))
    collie::log::set_error_handler([](const std::string &msg) {
        printf("*** Custom log error handler: %s ***\n", msg.c_str());
    });
}

// syslog example (linux/osx/freebsd)
#ifndef _WIN32

#include <collie/log/sinks/syslog_sink.h>

void syslog_example() {
    std::string ident = "clog-example";
    auto syslog_logger = collie::log::syslog_logger_mt("syslog", ident, LOG_PID);
    syslog_logger->warn("This is warning that will end up in syslog.");
}

#endif

// Android example.
#if defined(__ANDROID__)
#include <collie/log/sinks/android_sink.h>
void android_example() {
    std::string tag = "clog-android";
    auto android_logger = collie::log::android_logger_mt("android", tag);
    android_logger->critical("Use \"adb shell logcat\" to view this message.");
}
#endif

// Log patterns can contain custom flags.
// this will add custom flag '%*' which will be bound to a <my_formatter_flag> instance
#include <collie/log/pattern_formatter.h>

class my_formatter_flag : public collie::log::custom_flag_formatter {
public:
    void format(const collie::log::details::log_msg &,
                const std::tm &,
                collie::log::memory_buf_t &dest) override {
        std::string some_txt = "custom-flag";
        dest.append(some_txt.data(), some_txt.data() + some_txt.size());
    }

    std::unique_ptr<custom_flag_formatter> clone() const override {
        return collie::log::details::make_unique<my_formatter_flag>();
    }
};

void custom_flags_example() {
    using collie::log::details::make_unique;  // for pre c++14
    auto formatter = make_unique<collie::log::pattern_formatter>();
    formatter->add_flag<my_formatter_flag>('*').set_pattern("[%n] [%*] [%^%l%$] %v");
    // set the new formatter using collie::log::set_formatter(formatter) or
    // logger->set_formatter(formatter) collie::log::set_formatter(std::move(formatter));
}

void file_events_example() {
    // pass the collie::log::file_event_handlers to file sinks for open/close log file notifications
    collie::log::file_event_handlers handlers;
    handlers.before_open = [](collie::log::filename_t filename) {
        collie::log::info("Before opening {}", filename);
    };
    handlers.after_open = [](collie::log::filename_t filename, std::FILE *fstream) {
        collie::log::info("After opening {}", filename);
        fputs("After opening\n", fstream);
    };
    handlers.before_close = [](collie::log::filename_t filename, std::FILE *fstream) {
        collie::log::info("Before closing {}", filename);
        fputs("Before closing\n", fstream);
    };
    handlers.after_close = [](collie::log::filename_t filename) {
        collie::log::info("After closing {}", filename);
    };
    auto file_sink = std::make_shared<collie::log::sinks::basic_file_sink_mt>("logs/events-sample.txt",
                                                                         true, handlers);
    collie::log::logger my_logger("some_logger", file_sink);
    my_logger.info("Some log line");
}

void replace_default_logger_example() {
    // store the old logger so we don't break other examples.
    auto old_logger = collie::log::default_logger();

    auto new_logger =
            collie::log::basic_logger_mt("new_default_logger", "logs/new-default-log.txt", true);
    collie::log::set_default_logger(new_logger);
    collie::log::set_level(collie::log::level::info);
    collie::log::debug("This message should not be displayed!");
    collie::log::set_level(collie::log::level::trace);
    collie::log::debug("This message should be displayed..");

    collie::log::set_default_logger(old_logger);
}
