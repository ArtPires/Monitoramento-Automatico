FROM ubuntu:24.04

# Variáveis do toolchain
ENV DEBIAN_FRONTEND=noninteractive

# Atualiza o sistema e instala dependências básicas
RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    g++-aarch64-linux-gnu \
    gcc-aarch64-linux-gnu \
    pkg-config \
    ssh \
    rsync \
    zip \
    unzip \
    nano \
    python3 \
    python3-pip \
    && apt clean

# Define o compilador padrão para cross-compilation
ENV CC=aarch64-linux-gnu-gcc
ENV CXX=aarch64-linux-gnu-g++

# Diretório de trabalho
WORKDIR /workspace

# Entry point padrão
CMD ["/bin/bash"]

RUN echo "export PS1='\\u@ubuntu:\\w\\$ '" >> /root/.bashrc
