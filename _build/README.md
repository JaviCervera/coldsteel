# ColdSteel

A 3D game engine for Windows, macOS, Linux and the web (using Emscripten) released under the GPL3 license. Based on Lua, Irrlicht and SoLoud.

## Setting up Geany

### Windows

Copy the file *geany/filetypes.ColdSteel.conf* to *<User>\AppData\Roaming\geany\filedefs*.

Open a "main.lua" file in Geany, and select the menu option *Document / Set Filetype / ColdSteel file* to make the IDE
recognize it as an ColdSteel file (instead of a standard Lua script). To allow Geany to autodetect .lua files as ColdSteel
scripts, create a file *<User>\AppData\Roaming\geany\filetype_extensions.conf* with the following contents (or add the contents
to the corresponding section of the file if it already exists, separating the element with a semicolon from other elements):

```
[Extensions]
ColdSteel=*.lua;

[Groups]
Script=ColdSteel;
```

To run the program directly within Geany, select *Build / Run* from the menu, or use the F5 shortcut.
You must have the root folder in your PATH in order to run ColdSteel programs from Geany.

### Linux / macOS

Copy the file *geany/filetypes.ColdSteel.conf* to *$HOME/.config/geany/filedefs*.

Open a "main.lua" file in Geany, and select the menu option *Document / Set Filetype / ColdSteel file* to make the IDE
recognize it as an ColdSteel file (instead of a standard Lua script). To allow Geany to autodetect .lua files as ColdSteel
scripts, create a file *$HOME/.config/geany/filetype_extensions.conf* with the following contents (or add the contents
to the corresponding section of the file if it already exists, separating the element with a semicolon from other elements):

```
[Extensions]
ColdSteel=*.lua;

[Groups]
Script=ColdSteel;
```

To run the program directly within Geany, select *Build / Run* from the menu, or use the F5 shortcut.
You must have root folder in your PATH in order to run ColdSteel programs from Geany.
