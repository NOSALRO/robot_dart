//
// tiny-process-library C++ library - https://gitlab.com/eidheim/tiny-process-library
// Commit: 6166ba5dce461438cefb57e847832aca25d510d7
//
// The MIT License (MIT)
//
// Copyright (c) 2015-2020 Ole Christian Eidheim
//

#include "process.hpp"

namespace TinyProcessLib {

Process::Process(const std::vector<string_type> &arguments, const string_type &path,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(arguments, path);
  async_read();
}

Process::Process(const string_type &command, const string_type &path,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(command, path);
  async_read();
}

Process::Process(const std::vector<string_type> &arguments, const string_type &path,
                 const environment_type &environment,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(arguments, path, &environment);
  async_read();
}

Process::Process(const string_type &command, const string_type &path,
                 const environment_type &environment,
                 std::function<void(const char *bytes, size_t n)> read_stdout,
                 std::function<void(const char *bytes, size_t n)> read_stderr,
                 bool open_stdin, const Config &config) noexcept
    : closed(true), read_stdout(std::move(read_stdout)), read_stderr(std::move(read_stderr)), open_stdin(open_stdin), config(config) {
  open(command, path, &environment);
  async_read();
}

Process::~Process() noexcept {
  close_fds();
}

Process::id_type Process::get_id() const noexcept {
  return data.id;
}

bool Process::write(const std::string &str) {
  return write(str.c_str(), str.size());
}

} // namespace TinyProcessLib
