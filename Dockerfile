FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libpq-dev \
    libpqxx-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    libboost-system-dev \
    zlib1g-dev

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build -- -j$(nproc)

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libpq5 \
    libpqxx-6.4 \
    libcurl4 \
    libssl3 \
    libboost-system1.74.0 \
    zlib1g \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/build/xoplay .

CMD ["./xoplay"]