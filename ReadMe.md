# DollyXs Adobe Indesign SDK C++ Plug-in Project Template Builder

Since java vm no longer works properly on mac os ventura/catalina you can use **DollyXs**. It is a tool that can be used instead of java based dolly developed by Adobe. You can use it to create Adobe Indesign SDK C++ Plug-In project instead on java based Dolly tool shipped with installation. This tool can be used for mac os platform for now (windows related codes were hidden however it can be)

# Setting Up the Development Environment on macOS

## Necessary Software

- git
- Xcode
- Qt Framework


### Building DollyXs

Open DollyXs.pro file on your `Qt Creator.app` then build as is

## Using DollyXs
First of all you need to download Adobe Indesign Plug-in SDK from https://developer.adobe.com/console/home. Unzip the folder whereever you want into your machine.

Adobe Indesign Plug-in SDK folder structure should be as downloaded.
```
<SDK Root>
        ├── build
                ├── mac
                      ├── prj #xcode project folder
        ├── devtools
                ├── dolly
                        ├── xs #xsl transform files folder
        ├── docs
        ├── external
        ├── flexsamples
        └── source
                ├── sdksamples #xcode source *.h/*.cpp folder
        
```
