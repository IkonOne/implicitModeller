#pragma once

#include <filesystem>
#include <string>

class FileWatcher {
public:
    FileWatcher() : _isWatching(false) {}
    ~FileWatcher() = default;
    FileWatcher(const FileWatcher&) = delete;

    void start(const std::string& path);
    void stop();
    bool checkModified();

    const std::filesystem::file_time_type lastWriteTime() const {
        return this->_lastWriteTime;
    }

    const std::string path() const { return this->_path; }
    const bool isWatching() const { return this->_isWatching; }

private:
    bool _isWatching;
    std::string _path;
    std::filesystem::file_time_type _lastWriteTime;
};