# ViewLog

ViewLog is a video tracking app built with Qt6. ViewLog allows you to keep track of all the videos you've watched within the app. Whether you’re managing a growing collection or just keeping tabs on your latest binge-watching session, ViewLog ensures you never lose track of your video journey.

# Build

First install the nlohmann/json library

Ubuntu/Debian:
You can install the nlohmann-json3-dev package using apt:

```console
sudo apt update
sudo apt install nlohmann-json3-dev
```

Fedora:
You can install the json-devel package using dnf:

```console
Copy code
sudo dnf install json-devel
```

build
```console
mkdir build
cd build
qmake6 ../MediaPlayer.pro
```