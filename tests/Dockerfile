# Run me from root directory
# https://assimp-docs.readthedocs.io/en/latest/about/introduction.html#dependencies
FROM ubuntu:latest
RUN apt-get update && apt-get install -y \
    build-essential \
    mesa-common-dev \
    cmake
COPY ../ ./tank-engine/
WORKDIR "/tank-engine/vendor/assimp"
RUN cmake -DASSIMP_BUILD_ZLIB=ON .
RUN make
RUN cp bin/libassimp.so.5 lib/libassimp5.so
WORKDIR "/tank-engine"
CMD [ "./build_and_run_linux.sh" ]