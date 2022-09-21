# Build && Run

To build and run the project:

```bash
mkdir build
cd build
cmake .. -DGITHUB_PROXY_ENABLE=ON
make -j8
./csrc/benchmark 300 400 500
```

## Dependency

The dependency source code will download from Github, the download speed may be slow in some places. There are two ways to solve this.

1. We provide a CMake option to download source code from a proxy site: https://ghproxy.com/.

```
cmake .. -DGITHUB_PROXY_ENABLE=ON
```

2. You can also configure a proxy server as needed.

```
git config --global http.proxy http://127.0.0.1:1080
git config --global https.proxy https://127.0.0.1:1080
```
