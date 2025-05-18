ARG VARIANT="jammy"

FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# Atualiza o sistema e instala os pacotes necessarios
RUN apt update && apt install -y \
    build-essential \
    gcc \
    g++ \
    cmake \
    make \
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

# Variáveis do toolchain
ENV DEBIAN_FRONTEND=noninteractive