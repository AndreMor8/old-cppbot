FROM alpine:latest

WORKDIR /home/cppbot

RUN apk add --no-cache build-base make cmake git libsasl tar pkgconfig openssl-dev curl zlib-dev linux-headers

RUN wget https://github.com/mongodb/mongo-c-driver/releases/download/1.17.4/mongo-c-driver-1.17.4.tar.gz && \
    tar xzf mongo-c-driver-1.17.4.tar.gz && \
    cd mongo-c-driver-1.17.4 && \
    mkdir cmake-build && \
    cd cmake-build && \
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. && \
    cmake --build . --target install

RUN curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.2/mongo-cxx-driver-r3.6.2.tar.gz && \
    tar -xzf mongo-cxx-driver-r3.6.2.tar.gz && \
    cd mongo-cxx-driver-r3.6.2/build && \
    cmake .. -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build . --target install

RUN git clone --recursive https://github.com/zeroxs/aegis.cpp && \
    cd aegis.cpp && \
    CXX=$CXX sh ./install-deps.sh && \
    mkdir -p build && \
    cd build && \
    CXX=$CXX cmake -DCMAKE_CXX_STANDARD=17 .. && \
    cmake --build . --target install

COPY . .

RUN mkdir -p build && \
    cd build && \
    CXX=$CXX cmake .. && \
    cmake --build .

RUN apk del git tar curl

RUN rm -rf mongo-c-driver-1.17.4.tar.gz mongo-cxx-driver-r3.6.2.tar.gz mongo-c-driver-1.17.4 mongo-cxx-driver-r3.6.2 aegis.cpp

CMD ["/home/cppbot/bin/cppbot"]