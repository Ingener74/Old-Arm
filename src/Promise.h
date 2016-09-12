#pragma once

#include <functional>
#include <memory>

namespace oldarm {

class Promise {
public:
    enum State {
        PENDING, FULFILED, REJECTED,
    };

    template<typename T>
    using promise_function = std::function<T(std::function<void(const T&)> resolve, std::function<void(const std::exception&)> reject)>;

    template<typename T>
    Promise(const promise_function<T>& f) {
        _core = std::make_shared<Core<T>>();
        _core->_state = PENDING;
        _core->_function = f;
        try {
            f([_core] {
                _core->_state = FULFILED;
            }, [_core] {
                _core->_state = REJECTED;
            });
        } catch (const std::exception& e) {
        }
    }
    virtual ~Promise() = default;

    Promise& then(std::function<void()> resolve, std::function<void()> reject);
    Promise& then(std::function<void()> resolve);
    Promise& error(std::function<void()> error);

private:
    template<typename T>
    class Core {
    public:
        State _state;
        promise_function<T> _function;
        Promise _next;
    };
    std::shared_ptr<Core> _core;
};

}
