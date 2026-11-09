#pragma once
#include <unistd.h>

class FileDescriptor
{
public:
    explicit FileDescriptor(int fd)
    :   fd_ {fd}
    {}

    FileDescriptor(const FileDescriptor& other) = delete;
    FileDescriptor& operator=(const FileDescriptor& other) = delete;
    FileDescriptor(FileDescriptor&& other) = delete;
    FileDescriptor& operator=(FileDescriptor&& other) = delete;
    ~FileDescriptor() 
    { 
        if (fd_ >= 0) { close(fd_); } 
    }

    [[nodiscard]] int get() const noexcept { return fd_; }

private:
    int fd_ {-1};
};
