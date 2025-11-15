#include "TInput.hpp"

bool TInput::key(EKey key, EKeyAction action) const {
    return glfwGetKey(window, static_cast<int>(key)) == static_cast<int>(action);
}

void TInput::onPressed(EKey key, const std::function<void()> &callback) {
    pressed_map[static_cast<int>(key)] = callback;
}
