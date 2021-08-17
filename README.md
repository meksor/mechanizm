# механизм
A rythm-based video editor. Uses `libopenshot`, `OpenCV` (soon) and `Qt5`. WIP and very buggy, beware. This is one of those 3 am hobby projects, don't judge.

## Building

Install `Qt5`, cmake takes care of the rest.

```bash
$ mkdir build
$ cd build

$ cmake -DCMAKE_BUILD_TYPE=Debug ..
# or
$ cmake ..

$ make -j4
```

## Using

I am not going to write a tutorial, but know this: everything is done with keyboard shortcuts. Check the source code for keymappings `<insert trollface here>`.

```bash
$ ./mechanizm
```
