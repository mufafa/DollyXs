# DollyXs Adobe Indesign SDK C++ Plug-in Project Template Builder

**DollyXs** is an tool for macOS (can be also implemented for windows). You can use it to create Adobe Indesign SDK C++ Plug-In project instead on java based Dolly tool shipped with installation. This tool can be used for mac os platform for now (windows related codes were hidden however it can be)

# Setting Up the Development Environment on macOS

## Necessary Software

- git
- Xcode
- Qt Framework


### Building DollyXs

Open DollyXs.pro file on your `Qt Creator.app` then build as is

## Using DollyXs
First of all you need to download Adobe Indesign Plug-in SDK from https://developer.adobe.com/console/home. Unzip the folder whereever you want into your machine.

Adobe Indesign Plug-in SDK folder structure should be similar like :
```
<SDK Root>
        build/mac/prj #destination folder for created project files (you can use another folder but it does not recommend)
        devtools/dolly/xs #transform files which will be used for `DollyXs.app`
        source/sdksamples # destination folder for source/header files (you can use another folder but it does not recommend)
```
