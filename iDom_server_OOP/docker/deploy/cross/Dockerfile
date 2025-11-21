FROM debian:bullseye

RUN dpkg --add-architecture arm64 && \
    apt-get update && apt-get install -y \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    build-essential \
    cmake \
    git \
    pkg-config \
    curl \
    wget \
    libmosquitto-dev:arm64 \
    libcurl4-openssl-dev:arm64 \
    libi2c-dev:arm64 \
    libssl-dev:arm64 \
    libmpdclient-dev \
    libmpd-dev \
    qemu-user-static binfmt-support \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# zapewnij natywny nagłówek i ewentualny stub debug_printf.h
RUN apt-get update && apt-get install -y libmpdclient-dev || true

RUN mkdir -p /usr/include/libmpd-1.0/libmpd && \
    cat > /usr/include/libmpd-1.0/libmpd/debug_printf.h <<'EOF'
#ifndef LIBMPD_DEBUG_PRINTF_H
#define LIBMPD_DEBUG_PRINTF_H
#include <stdarg.h>
#include <stdio.h>
static inline void debug_printf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap);
}
#endif
EOF

ENV CC=aarch64-linux-gnu-gcc
ENV CXX=aarch64-linux-gnu-g++
ENV AR=aarch64-linux-gnu-ar
ENV STRIP=aarch64-linux-gnu-strip
ENV PKG_CONFIG_PATH=/usr/lib/aarch64-linux-gnu/pkgconfig
ENV CPPFLAGS="-I/usr/aarch64-linux-gnu/include"
ENV LDFLAGS="-L/usr/lib/aarch64-linux-gnu"

WORKDIR /workspace

CMD ["/bin/bash"]