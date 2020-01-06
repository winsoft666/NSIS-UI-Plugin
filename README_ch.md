[>>> English Version](README.md)

# 介绍
提供一种创建漂亮地、现代化地Windows平台安装界面的方式。
默认使用Qt作为界面库，理论上你也可以使用任何界面库来创建安装界面，如[DuiLib](https://github.com/winsoft666/duilib2)等。

---

# 依赖项

**1. NSIS**

从[https://nsis.sourceforge.io/Download](https://nsis.sourceforge.io/Download) 下载NSIS并安装，新增系统环境变量`NSIS_DIR`为NSIS安装目录。

**2. Python**

之所以需要安装Python，主要是为了执行`NsisScriptGenerate.py`脚本。

将`Python.exe`所在目录添加到`Path`环境变量。

**3. Qt**

因为插件默认使用Qt作为界面库，所以需要安装Qt。

Qt安装包会默认将安装目录添加`QTDIR`环境变量，如果没有自动添加，则需要手动添加。

---

# 开始使用

**编译NSIS-UI-Plugin**

安装完上面依赖项之后，编译`NSIS-UI-Plugin\NSIS-UI-Plugin.vcxproj`工程，Visual Studio生成后事件会自动将目标文件（Debug版：`nsQtPluginD.dll` Release版：`nsPlugin.dll`）拷贝到NSIS插件目录（`NSIS_DIR\Plugins\x86-unicode`）。

**生成安装包**

`VimeoSetup`目录是如何在NSIS中使用插件的示例工程：

```txt
App                -- 放置需要打包到安装包中的文件
VCRuntimeDLL       -- 放置VC++运行时库文件（Debug版和Release版），Qt界面库默认采用MD模式编译，运行时需要依赖VC++运行时库
vimeo-template.nsi -- NSIS模板文件，文件名的`-template`后缀固定的，NsisScriptGenerate.py会根据该模板生成vimeo.nsi
build-setup [debug].bat -- 生成Debug版的安装包，即使用Debug版的Qt和NSIS-UI-Plugin
build-setup [debug].bat -- 生成Release版的安装包
```

将需要打包的文件放置到`App`目录，然后运行`build-setup.bat`生成安装包。

>`NsisScriptGenerate.py`脚本功能：因为NSIS没有提供获取文件释放进度的功能，所以`NsisScriptGenerate.py`遍历`App`目录，采用`File`命令挨个添加文件，并调用插件接口`SetInstallStepDescription`通知安装详情。

---

# 截图

仅用作示例，你可以使用任意界面库创建任意的界面。

![Screenshot-1](https://github.com/winsoft666/NSIS-UI-Plugin/blob/master/Screenshot/1.png)

![Screenshot-2](https://github.com/winsoft666/NSIS-UI-Plugin/blob/master/Screenshot/2.png)

![Screenshot-3](https://github.com/winsoft666/NSIS-UI-Plugin/blob/master/Screenshot/3.png)
