#ifndef PEAR_SCREEN_CAPTURE_CAPTURE_SOURCE
#define PEAR_SCREEN_CAPTURE_CAPTURE_SOURCE

#include <string>
#include <utility>

namespace pear::capture {

#ifdef PEAR_MAC
    typedef uint32_t SourceId;
#elif defined(PEAR_WIN)
    typedef intptr_t SourceId;
#endif

    enum CaptureSourceType {
        Screen, Window
    };

    struct CaptureSource {
        static std::string CaptureSourceKey(SourceId id, CaptureSourceType type) {
            return (type == Screen ? 's' : 'w') + std::to_string(id);
        }

        CaptureSource(SourceId id, std::string title, CaptureSourceType type)
                : id(id), key(CaptureSourceKey(id, type)), title(std::move(title)), type(type) {}

        SourceId id;
        std::string key;
        std::string title;
        CaptureSourceType type;
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_CAPTURE_SOURCE
