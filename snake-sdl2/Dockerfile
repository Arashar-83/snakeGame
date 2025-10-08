FROM debian:stable-slim
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential pkg-config cmake libsdl2-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .
RUN make
CMD ["./build/snake"]