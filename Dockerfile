FROM busybox:latest
WORKDIR /var/www 

RUN wget https://github.com/Infinage/calculator/releases/download/v0.0.1/Calculator-0.0.1-Wasm.zip && \
    unzip Calculator-0.0.1-Wasm.zip && rm Calculator-0.0.1-Wasm.zip && \
    mv calculator.html index.html 

RUN mkdir -p docs && \
    wget https://github.com/Infinage/calculator/releases/download/v0.0.1/Calculator-0.0.1-Docs.zip -O docs/docs.zip && \
    cd docs && unzip docs.zip && rm docs.zip 

RUN mkdir -p coverage && \
    wget https://github.com/Infinage/calculator/releases/download/v0.0.1/Calculator-0.0.1-Coverage.zip -O coverage/coverage.zip && \
    cd coverage && unzip coverage.zip && rm coverage.zip && \
    mv coverage.html index.html

EXPOSE 80
CMD ["httpd", "-f", "-p", "80", "-h", "/var/www"]
