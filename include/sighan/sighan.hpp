#pragma once
#ifndef SIGHAN_HPP
#define SIGHAN_HPP

#include <condition_variable>
#include <future>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace sighan {

using Signal = int;

constexpr Signal NO_SIGNAL = 0;
constexpr Signal ERROR = -1;
constexpr Signal MANUAL_STOP = -2;
constexpr Signal MANUAL_TERMINATION = -3;

class SignalHandler {
public:
    explicit SignalHandler(std::initializer_list<Signal>);
    ~SignalHandler();

    void stop();

    Signal wait();

    Signal received();
    std::string received_name();
    std::string error_reason();

private:
    void request_intervention(bool terminate = false);
    void broadcast_error(const std::string &reason);

    Signal received_signal = NO_SIGNAL;
    sigset_t sigset;
    std::condition_variable cv;
    std::mutex mut;

    std::thread listener;
    std::future<pthread_t> listener_pthread;

    Signal some_of;
    bool manual_stop_requested = false;
    bool listening = false;
    std::string error_msg;
};

class Exception : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace sighan


#endif // SIGHAN_HPP
