[>>> 中文版](README_ch.md)

# NSIS-UI-Plugin
Create beautiful, modern UI setup for windows.

Using Qt as default UI library, you can use any UI library in theory, such as [DuiLib](https://github.com/winsoft666/duilib2).

---

# Dependencies

**1. NSIS**

Download [NSIS](https://nsis.sourceforge.io/Download) and install, then add `NSIS_DIR` system environment variable as NSIS install directory.

**2. Python**

Install Python just for execute `NsisScriptGenerate.py` script. 

After installed, add the direcotry where `Python.exe` is located to `Path` system environment variable.

**3. Qt**

NSIS-UI-Plugin use Qt as UI library by default.

The Qt installer will add the `QtDIR` system environment variable as Qt install directory. If it not be added automatically, you need to add it manually.

**4. Visual Studio**

Install Visual Studio and "Qt VS Tool" plugin, configure Qt version and install directory.

---

# Getting Started

**Compile NSIS-UI-Plugin**

After installing the above dependencies, then open `NSIS-UI-Plugin\NSIS-UI-Plugin.vcxproj` project, setting Qt version in project property page.

Compile project, Visual Studio's *After Build Event* will copy target file(Debug: `nsQtPluginD.dll` Release: `nsPlugin.dll`) to NSIS plugin directory(`NSIS_DIR\Plugins\x86-unicode`).

If the copy fails, it may be caused by permission issues. You need to run Visual Studio with administrator rights and compile again.

**Generate Setup**

`VimeoSetup` directory is sample about how to use NSIS-UI-Plugin.

```txt
App                        -- Place the files that need to be packaged into the installer
VCRuntimeDLL               -- Place VC++ runtime files(Debug/Release), Qt compiled with MD mode by default.
vimeo-template.nsi         -- NSIS template script file,`-template` suffix is fixed, can't change. NsisScriptGenerate.py will generate vimeo.nsi based on this template.
build-setup [debug].bat    -- Generate Debug mode setup, that means using debug version Qt/NSIS-UI-Plugin.
build-setup [debug].bat    -- Generate Release mode setup
```

Place the files you need to packaged into the `App` directory, then run `build-setup.bat` to generate the installer.


>`NsisScriptGenerate.py`:
Since NSIS has no way to get file extract progress/details, so I write `NsisScriptGenerate.py`, enumerate `App` folder, add each file by `File` command, call `SetInstallStepDescription` interface to notify installing details.

---

# Screenshot

For example only, you can use any UI library to create any UI by yourself.

![Screenshot-1](https://github.com/winsoft666/NSIS-UI-Plugin/blob/master/Screenshot/1.png)

![Screenshot-2](https://github.com/winsoft666/NSIS-UI-Plugin/blob/master/Screenshot/2.png)

![Screenshot-3](https://github.com/winsoft666/NSIS-UI-Plugin/blob/master/Screenshot/3.png)
