# Kaname (千鳥 かなめ)

Kaname is an annotating tool. Currently only image is supported. Video support is planned and comming soon.

At this time, to annotate a video, you'll need to use our `Ixtractor` tool converting video to images. 

## Build
This is a regular Qt subdir project. Execute `qmake` and build it with your favourite compiler.

### Tested Platforms

* Windows 8.1 64-bit, Qt 5.7.0/MinGW-W64 5.3.0/shared/32-bit
* Windows 8.1 64-bit, Qt 5.7.0/Visual Studio 2015/shared/64-bit

## Subdirs

### Kaname
This is the core annotating application.

### LabelJson
This is an annotating data format plugin.

### ObjJson
This is an object definition format plugin.

## Use of plugins
Copy plugins to the app exe folder. The app will automatically load these plugins when starts.

## License
Licensed under MIT now

## Kaname
![千鳥 かなめ](Kaname.png)
