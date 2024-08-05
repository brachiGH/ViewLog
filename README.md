# ViewLog

ViewLog is a video tracking app built with Qt6. ViewLog allows you to keep track of all the videos you've watched within the app. Whether you’re managing a growing collection or just keeping tabs on your latest binge-watching session, ViewLog ensures you never lose track of your video journey.

![alt text](https://i.imgur.com/O5r2t4c.png)
![alt text](https://i.imgur.com/leqy43y.png)

# Build

```console
rm -rf build
mkdir build
cd build
qmake ../MediaPlayer.pro
make
```

if did notwork try

```console
rm -rf build
mkdir build
cd build
"/PATH/TO/Qt/6.x/gcc_64/bin/qmake" ../MediaPlayer.pro
make
```

### if you are using Windows

windeployqt Tool (Windows)

Qt provides a tool called windeployqt that simplifies the deployment process by automatically copying necessary Qt libraries and plugins:

Open a Command Prompt and navigate to the directory where your application executable resides.

Run windeployqt:

```console
windeployqt MediaPlayer.exe
```

> NOTE: if you get a "libstdc++-6.dll" not found or "libgcc_s_seh-1.dll" not found
>
> you can copy the missing dlls from "PATH/TO/Qt/tools/mingw*****/bin"
