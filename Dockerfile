FROM ubuntu:24.04

# Install the standard Linux build engine tools
RUN apt-get update && apt-get install -y \
    build-essential \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set the execution folder mapping target
WORKDIR /os
