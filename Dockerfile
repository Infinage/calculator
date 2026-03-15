# Stage 1: Fetch and Prepare (Native Host)
FROM --platform=$BUILDPLATFORM alpine:latest AS fetch
RUN apk add --no-cache curl unzip 
WORKDIR /tmp

# 1. WASM / Main UI
RUN curl -L -o wasm.zip https://github.com/Infinage/calculator/releases/download/v0.0.1/Calculator-0.0.1-Wasm.zip && \
    unzip wasm.zip && rm wasm.zip && \
    mv calculator.html index.html 

# 2. Docs
RUN mkdir -p docs && \
    curl -L -o docs.zip https://github.com/Infinage/calculator/releases/download/v0.0.1/Calculator-0.0.1-Docs.zip && \
    unzip docs.zip -d docs && rm docs.zip 

# 3. Coverage
RUN mkdir -p coverage && \
    curl -L -o coverage.zip https://github.com/Infinage/calculator/releases/download/v0.0.1/Calculator-0.0.1-Coverage.zip && \
    unzip coverage.zip -d coverage && rm coverage.zip && \
    mv coverage/coverage.html coverage/index.html

# Stage 2: Final Runtime (Target Arch: e.g., arm64 for your Pi)
FROM busybox:latest
WORKDIR /var/www

# Copy everything prepared in Stage 1
COPY --from=fetch /tmp/ ./

EXPOSE 80
CMD ["httpd", "-f", "-p", "80", "-h", "/var/www"]
