# to build the image: 
#       docker build -t yaha-dev-2 .
#
# to run the container: 
#       docker run -it --name yaha-dev-2 -v c:\home\jari\yaha:/workspace yaha-dev-2

# Use the official Debian 12 image as the base
FROM alpine:latest

# Install necessary packages
RUN apk update && apk add --no-cache \
    g++ \
    gdb \
    cmake \
    make \
    curl \
    git \
    build-base ninja zip unzip \
    pkgconf \
    linux-headers perl bash


# add vcpkg
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
RUN git clone --depth 1 https://github.com/microsoft/vcpkg.git /tmp/vcpkg && \
    /tmp/vcpkg/bootstrap-vcpkg.sh \
    && /tmp/vcpkg/vcpkg install flatbuffers:x64-linux-release \
    && /tmp/vcpkg/vcpkg install nlohmann-json:x64-linux-release \
    && /tmp/vcpkg/vcpkg install sqlite3:x64-linux-release \
    && /tmp/vcpkg/vcpkg install mosquitto:x64-linux-release \
    && /tmp/vcpkg/vcpkg install cpp-httplib:x64-linux-release \
    && /tmp/vcpkg/vcpkg install tinyxml2:x64-linux-release \
    && /tmp/vcpkg/vcpkg install catch2:x64-linux-release \
    && mv /tmp/vcpkg/installed /opt/vcpkg \
    && rm -rf /tmp/vcpkg \
    && apk del linux-headers perl

ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH=$PATH:/opt/vcpkg

# Set the working directory
WORKDIR /workspace

# Start the shell
CMD ["bash", "--login", "-i"]