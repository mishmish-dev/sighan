#include <sighan/sighan.hpp>

#include <cerrno>
#include <csignal>
#include <cstring>
#include <unistd.h>

namespace {

std::string error_text(const int error_num) {
    return std::strerror(error_num);
}

} // namespace


namespace sighan {

SignalHandler::SignalHandler(const std::initializer_list<Signal> signals) {
    if (signals.size() == 0) {
        throw std::logic_error{"Won't init SignalHandler with empty list of signals"};
    }
    some_of = *signals.begin();

    if (sigemptyset(&sigset) != 0) {
        throw Exception{"sigemptyset error: " + error_text(errno)};
    }

    for (const auto s : signals) {
        if (sigaddset(&sigset, s) != 0) {
            throw Exception{"sigaddset error: " + error_text(errno)};
        }
    }

    if (const auto err = pthread_sigmask(SIG_BLOCK, &sigset, nullptr)) {
        throw Exception{"pthread_sigmask error: " + error_text(err)};
    }

    std::packaged_task<pthread_t()> set_pthread_id{[]() { return pthread_self(); }};
    listener_pthread = set_pthread_id.get_future();

    auto worker = [this, set_pthread_id = std::move(set_pthread_id)]() mutable {
        set_pthread_id();

        Signal signal = NO_SIGNAL;
        if (const auto err = sigwait(&sigset, &signal)) {
            std::lock_guard<std::mutex> lock{mut};
            received_signal = ERROR;
            error_msg = "sigwait error: " + error_text(err);
        } else {
            std::lock_guard<std::mutex> lock{mut};
            if (manual_stop_requested) {
                received_signal = MANUAL_STOP;
            }
        }

        cv.notify_all();
        {
            std::lock_guard<std::mutex> lock{mut};
            listening = false;
        }
        pthread_sigmask(SIG_UNBLOCK, &sigset, nullptr);
    };

    listener = std::thread{std::move(worker)};
    listening = true;
}

SignalHandler::~SignalHandler() {
    stop();
    wait();
    if (listener.joinable()) {
        listener.join();
    }
}

Signal SignalHandler::wait() {
    std::unique_lock<std::mutex> lock{mut};
    cv.wait(lock, [this]() {
        return received_signal != NO_SIGNAL;
    });
    return received_signal;
}

void SignalHandler::stop() {
    {
        std::lock_guard<std::mutex> lock{mut};
        if (manual_stop_requested || !listening) {
            return;
        }
        manual_stop_requested = true;
    }
    pthread_kill(listener_pthread.get(), some_of);
}

} // namespace sighan
