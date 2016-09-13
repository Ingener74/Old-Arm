#pragma once

#include <functional>
#include <memory>
#include <list>

namespace oldarm {

using Resolve = std::function<void()>;
using Reject = std::function<void()>;

class Promise {
public:
    enum State {
        PENDING, FULFILLED, REJECTED,
    };

    Promise(std::function<void(Resolve, Reject)> f) {
        try {
            f([this]() {
                if(m_nexts.empty()) {
                    _state = FULFILLED;
                } else {
                    auto f = m_nexts.front();
                    m_nexts.pop_front();
                    f();
                }
            }, [this] {
                _state = REJECTED;
            });
        } catch (const std::exception& e) {
        }
    }
    virtual ~Promise() = default;

    Promise& then(std::function<void()> resolve) {
        if (_state == PENDING) {
            m_nexts.push_back(resolve);
        } else if (_state == FULFILLED) {
            resolve();
        } else if (_state == REJECTED) {
            if (m_error)
                m_error();
        }
        return *this;
    }
    Promise& then(std::function<void()> resolve, std::function<void()> reject) {
        if (_state == PENDING) {
            m_nexts.push_back(resolve);
            m_error = reject;
        } else if (_state == FULFILLED) {
            resolve();
        } else if (_state == REJECTED) {
            reject();
        }
        return *this;
    }
    Promise& error(std::function<void()> error) {
        return *this;
    }

private:
    State _state = PENDING;

    std::list<std::function<void()>> m_nexts;
    std::function<void()> m_error;
};

}
