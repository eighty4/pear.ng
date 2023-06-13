#ifndef PEAR_SCREEN_CAPTURE_SOURCE_WATCHER
#define PEAR_SCREEN_CAPTURE_SOURCE_WATCHER

#include "capture_source.h"
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace pear::capture {

    class SourceUpdateHandler {
    public:
        virtual void OnSourceUpdates(std::vector<std::shared_ptr<CaptureSource>> added,
                                     std::vector<std::shared_ptr<CaptureSource>> removed,
                                     std::vector<std::shared_ptr<CaptureSource>> updated) = 0;
    };

    enum SourcePollType {
        AllSources, ScreenSourcesOnly, WindowSourcesOnly
    };

    class SourceWatcher {
    public:
        SourceWatcher(SourceUpdateHandler *handler, SourcePollType poll_type);

        ~SourceWatcher();

        std::vector<std::shared_ptr<CaptureSource>> GetAvailableSources();

        std::shared_ptr<CaptureSource> GetSource(const std::string &source_id);

        static std::vector<std::shared_ptr<CaptureSource>> PollForAvailableSources(SourcePollType poll_type);

    private:
        void UpdateSourceList();

    private:
        bool active_ = true;
        SourceUpdateHandler *handler_;
        std::mutex sources_mutex_;
        int poll_interval_ = 250;
        std::thread *poll_thread_;
        SourcePollType poll_type_;
        std::unordered_map<std::string, std::shared_ptr<CaptureSource>> sources_;
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_SOURCE_WATCHER
