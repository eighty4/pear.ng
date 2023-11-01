#include "net_tcp/tcp_socket.h"
#include "screen_capture/screen_capturer.h"
#include "screen_capture/source_watcher.h"
#include <iostream>
#include <fmt/chrono.h>

using namespace pear::capture;

int main() {
    auto sources = SourceWatcher::PollForAvailableSources(AllSources);
    std::cout << "Pear.ng streaming client\n---" << std::endl;
    for (int i = 0; i < sources.size(); i++) {
        auto source = sources[i];
        fmt::print("{}) {}: {}\n", i, source->type == Screen ? "Screen" : "Window", source->title);
    }

    std::shared_ptr<CaptureSource> source;
    while (!source) {
        std::cout << "Choose a screen or window to start streaming: " << std::flush;
        std::string read_source_index;
        std::cin >> read_source_index;
        int source_index = std::stoi(read_source_index, nullptr, 10);
        if (source_index <= sources.size() - 1) {
            source = sources[source_index];
        } else {
            fmt::print("{} not a valid choice\n", read_source_index);
        }
    }

    int socket = pear::net::tcp::RECOVERABLE;
    int retry = 0;
    while (socket == pear::net::tcp::RECOVERABLE && retry < 3) {
        socket = pear::net::tcp::SocketConnect("127.0.0.1", 8080);
        retry++;
    }
    if (socket == pear::net::tcp::FAILED || socket == pear::net::tcp::RECOVERABLE) {
        fmt::print("tcp connect to 127.0.0.1:8080 failed after {} {}\n", retry,
                   retry == 1 ? "try" : "tries");
        exit(1);
    }

    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    auto capturer = ScreenCapturer::Create(source);
    for (auto frame = capturer->Capture(); frame; frame = capturer->Capture()) {
        auto diff = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
        fmt::print("{} {}x{}\n", diff, frame->Width(), frame->Height());
        start = std::chrono::steady_clock::now();
        std::cout << "send " << pear::net::tcp::SocketSend(socket, "data!", 5) << std::endl;
    }
    return 0;
}
