#include <Log.hpp>

Logger& debug() {
    static Logger log;
    return log;
}
