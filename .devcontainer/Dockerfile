FROM mcr.microsoft.com/vscode/devcontainers/base:debian-11

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*
