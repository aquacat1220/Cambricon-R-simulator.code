FROM ubuntu:22.04
RUN apt-get update && apt-get upgrade -y

# Build essentials for C++
RUN apt-get install -y build-essential cmake git
RUN apt-get install -y gdb
RUN apt-get install -y libboost-all-dev

# TODO: Add other dependencies (DRAMSIM?)