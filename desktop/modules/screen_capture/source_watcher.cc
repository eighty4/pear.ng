#include "source_watcher.h"

namespace pear::capture {

    SourceWatcher::SourceWatcher(SourceUpdateHandler *handler, SourcePollType poll_type)
            : handler_(handler),
              poll_type_(poll_type) {
        poll_thread_ = new std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_));
            do {
                UpdateSourceList();
                std::this_thread::sleep_for(std::chrono::milliseconds(poll_interval_));
            } while (active_);
        });
    }

    SourceWatcher::~SourceWatcher() {
        active_ = false;
        poll_thread_->join();
        delete poll_thread_;
        handler_ = nullptr;
    }

    std::vector<std::shared_ptr<CaptureSource>> SourceWatcher::GetAvailableSources() {
        sources_mutex_.lock();
        std::vector<std::shared_ptr<CaptureSource>> sources;
        for (const auto &source: sources_) {
            sources.push_back(source.second);
        }
        sources_mutex_.unlock();
        return sources;
    }

    std::shared_ptr<CaptureSource> SourceWatcher::GetSource(const std::string &source_id) {
        if (sources_.contains(source_id)) {
            return sources_.at(source_id);
        } else {
            return nullptr;
        }
    }

    void SourceWatcher::UpdateSourceList() {
        std::vector<std::shared_ptr<CaptureSource>> added;
        std::vector<std::shared_ptr<CaptureSource>> updated;
        std::vector<std::shared_ptr<CaptureSource>> removed;
        std::vector<std::shared_ptr<CaptureSource>> current = PollForAvailableSources(poll_type_);
        sources_mutex_.lock();
        std::vector<std::string> prev_source_ids;
        for (const auto &source: sources_) {
            prev_source_ids.push_back(source.first);
        }
        for (const auto &source: current) {
            if (std::find(prev_source_ids.begin(), prev_source_ids.end(), source->key) != prev_source_ids.end()) {
                prev_source_ids.erase(std::remove(prev_source_ids.begin(), prev_source_ids.end(), source->key),
                                      prev_source_ids.end());
                auto prev_source = sources_.at(source->key);
                if (prev_source->title != source->title) {
                    prev_source->title = source->title;
                    updated.push_back(prev_source);
                }
            } else {
                sources_.emplace(source->key, source);
                added.push_back(source);
            }
        }
        for (const auto &prev_source_id: prev_source_ids) {
            const auto removed_source = sources_.at(prev_source_id);
            sources_.erase(prev_source_id);
            removed.push_back(removed_source);
        }
        sources_mutex_.unlock();
        if (!added.empty() || !removed.empty() || !updated.empty()) {
            handler_->OnSourceUpdates(std::move(added), std::move(removed), std::move(updated));
        }
    }

} // pear::capture
