#include "assert_reached.h"

#include <stdexcept>
#include <sstream>

namespace assert_reached {

details::Instance details::instance;

void details::flag(size_t n) {
    std::lock_guard<std::mutex> lk(instance.mutex);

    for (auto&& it : instance.table) {
        it.second->flag(n);
    }
}

Guard::Guard(const char* x) : str_(x) {
    std::lock_guard<std::mutex> lk(details::instance.mutex);
    id_ = details::instance.counter++;
    details::instance.table[id_] = this;
}

Guard::~Guard() noexcept(false) {
    std::lock_guard<std::mutex> lk(details::instance.mutex);
    details::instance.table.erase(id_);
}

void Guard::callback(const Line& line, bool hit) {
    if (!std::uncaught_exception() && !hit) {
        std::stringstream ss;

        ss << this->name() << "(" << this->str() << ") Failed to reach : " << line.str()
           << " - " << line.file() << ":" << line.line();

        throw std::runtime_error(ss.str());
    }
}

void Guard::flag(size_t n) {
    if (n >= begin() && n < end()) {
        lines_[n] = true;
    }
}

}
