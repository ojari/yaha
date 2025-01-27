# Use the official Debian image as a base
FROM debian:stable-slim

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-arm-linux-gnueabi \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*