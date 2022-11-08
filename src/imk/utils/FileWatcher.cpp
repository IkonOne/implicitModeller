#include "FileWatcher.h"

#include <iostream>
#include <filesystem>
#include <string>

namespace imk {

void FileWatcher::start(const std::string& path) {
    _isWatching = true;

    if (!std::filesystem::exists(path)) {
        std::string err = "File does not exist: ";
        throw std::runtime_error(err + path);
    }

    this->_path = path;
    _lastWriteTime = std::filesystem::last_write_time(path);
}

void FileWatcher::stop() {
    _isWatching = false;
    this->_path = "";
    this->_lastWriteTime = this->_lastWriteTime.min();
}

bool FileWatcher::checkModified() {
    if (!_isWatching) {
        throw std::runtime_error("FileWatcher is not started...");
    }

    auto writeTime = std::filesystem::last_write_time(this->_path);
    if (writeTime > this->_lastWriteTime) {
        this->_lastWriteTime = writeTime;
        return true;
    }
    return false;
}

} // imk