# механизм
A rythm-based video editor. Uses `libopenshot`, `OpenCV` (soon) and `Qt5`. WIP and very buggy, beware. This is one of those 3 am hobby projects, don't judge.

it also has many memory leaks rn
## Building



```bash
$ sudo apt install build-essential qtbase5-dev qt5-qmake qtcreator libqt5svg5-dev libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavdevice-dev libzmq3-dev libjsoncpp-dev


$ mkdir build
$ cd build

$ cmake -DCMAKE_BUILD_TYPE=Debug ..
# or
$ cmake ..

$ make -j4
```

## Using

```bash
$ ./src/mechanizm
```
