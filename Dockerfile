
FROM ubuntu:24.04

# Install standard Linux build tools
RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    curl \
    gcc \
    binutils \
    && rm -rf /var/lib/apt/lists/*

# Install Rust
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs \
    | sh -s -- -y --profile minimal

# Add Rust to PATH
ENV PATH="/root/.cargo/bin:${PATH}"

# Install nightly Rust and rust-src
RUN rustup toolchain install nightly --profile minimal
RUN rustup component add rust-src --toolchain nightly
RUN rustup target add x86_64-unknown-none

# Set execution folder
WORKDIR /os
