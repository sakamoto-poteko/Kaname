# Kaname (千鳥 かなめ)

Kaname is a labeling & annotating tool. Currently only image is supported. Video support is planned and comming soon.

At this time, to label a video, you'll need to use our `Ixtractor` tool converting video to images. 

## Build
This is a regular Qt subdir project. Execute `qmake` and build it with your favourite compiler.

### Tested Platforms

* Windows 8.1 64-bit, Qt 5.7.0/MinGW-W64 5.3.0/shared/32-bit
* Windows 8.1 64-bit, Qt 5.7.0/Visual Studio 2015/shared/64-bit

## Subdirs

### Kaname
This is the core labeling application.

### LabelJson
This is a labeling data format plugin.

### ObjJson
This is a object definition format plugin.

## Use of plugins
Copy plugins to the app exe folder. The app will automatically load these plugins when starts.

## License
It seems that GPLv3 is the only choice. To be honest, I dislike this license. The reason is that I'm statically linking with Qt lib, 
and, for the future video support, ffmpeg prebuild under Windows I'm using is released under GPLv3.

```
Copyright (C) 2016  Afa.L Cheng <afa@afa.moe>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

## Kaname
![千鳥 かなめ](Kaname.png)
