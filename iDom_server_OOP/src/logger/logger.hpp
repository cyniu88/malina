/*
 * logger.hpp
 *
 *
 * Logger Library Header
 *
 *
 * Copyright (C) 2013-2014  Bryant Moscon - bmoscon@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license and disclaimer information.
 *
 * 3. The end-user documentation included with the redistribution, if any, must
 *    include the following acknowledgment: "This product includes software
 *    developed by Bryant Moscon (http://www.bryantmoscon.org/)", in the same
 *    place and form as other third-party acknowledgments. Alternately, this
 *    acknowledgment may appear in the software itself, in the same form and
 *    location as other such third-party acknowledgments.
 *
 * 4. Except as contained in this notice, the name of the author, Bryant Moscon,
 *    shall not be used in advertising or otherwise to promote the sale, use or
 *    other dealings in this Software without prior written authorization from
 *    the author.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#pragma once

#include <fstream>
#include <cassert>
#include <ctime>
#include <sstream>

// Log levels
typedef enum
{
    US = 0,
    VERBOSE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
    FATAL
} logger_level;

class Logger : public std::ostringstream
{
public:
    explicit Logger(const char *f);
    explicit Logger(const std::string &f);
    Logger(const Logger &);
    Logger &operator=(const Logger &);
    ~Logger();
    static pthread_mutex_t mutex_log;

    void set_level(const logger_level &level);
    void flush();
    void mutex_lock();
    void mutex_unlock();

    template <typename T>
    Logger &operator<<(const T &t)
    {
        *static_cast<std::ostringstream *>(this) << t;
        return (*this);
    }

    Logger &operator<<(const logger_level &level);
    typedef Logger &(*LoggerManip)(Logger &);
    Logger &operator<<(LoggerManip m);

private:
    std::string get_time() const;
    inline const char *level_str(const logger_level &level);

public:
    std::ofstream _file;

private:
    std::ostream &_log;
    logger_level _level;
    logger_level _line_level;
};

namespace std
{
    inline Logger &endl(Logger &out)
    {
        out.put('\n');
        out.flush();
        return (out);
    }
} // end namespace std

