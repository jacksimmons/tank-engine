:: Tests that the engine compiles and runs in an Ubuntu docker container.
:: Run me from root directory.
docker build -t tank-test -f tests/Dockerfile .
docker run tank-test