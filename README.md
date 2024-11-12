# ViewLog

ViewLog is a video tracking app built with Qt6. ViewLog allows you to keep track of all the videos you've watched within the app. Whether you’re managing a growing collection or just keeping tabs on your latest binge-watching session, ViewLog ensures you never lose track of your video journey.

![alt text](https://i.imgur.com/q2jTYx4.png)
![alt text](https://i.imgur.com/jo5l6O9.png)
![alt text](https://i.imgur.com/sbs83Yt.png)

# Build

1. **Export from Qt Design Studio**
   ```bash
   # In Qt Design Studio
   File -> Export Project -> Generate Cmake files
   ```

2. **Open in Qt Creator**
   ```bash
   # Open the exported CMakeLists.txt file
   ```

3. **Configure Build Settings**
   ```bash
   # In Qt Creator
   Tools -> Options -> Build & Run
   Select your compiler and Qt 6 kit
   ```

4. **Build for Release**
   ```bash
   # Change build configuration to Release
   Select Release from the bottom-left build configuration dropdown
   ```

5. **Build the Project**
   ```bash
   Build -> Build Project
   # or press Ctrl+B
   ```

6. **Deploy Dependencies**
   ```bash
   # Windows
   windeployqt.exe --qmldir <path_to_qml_folder> path/to/your/app.exe

   #e.g C:\Qt\6.5.3\mingw_64\bin\windeployqt.exe --qmldir C:\Users\user1\Desktop\qt\ViewLog\content\. ViewLog.exe

   # Linux
   linuxdeployqt  --qmldir <path_to_qml_folder> path/to/your/app

   # macOS
   macdeployqt  --qmldir <path_to_qml_folder> path/to/your/app.app
   ```

7. **Find Executable**
   ```
   # Default location:
   build-<project>-<kit>-Release/
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

[![Badge License]][License]
