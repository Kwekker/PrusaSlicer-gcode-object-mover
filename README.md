# PrusaSlicer gcode object mover

This program moves specified objects in a file by a specified amount in every possible axis.
It can be used to make multicolor prints with a single extruder printer.

I will make a video tutorial soon for setting up the model before putting it through this program.


## Compiling
After cloning, compile the code using the following command. It *should* work fine on all operating systems.
```bash
make
```
You will, of course, need to have make and gcc installed.


## Running
The program can be run as follows:
```html
./objectmover <file> <options>
```
Where:
- `<file>` is the path to your gcode file
- `<options>` see next chapter


## Options
Specifying an object's offset can be done by using the following flags. The program reads them in order. When you use a `-N`, all (lowercase) flags after it will be about that object. Using a `-N` again will start the specifications of the next object.
- `-N <name>`: The object's name in prusaslicer
- `-aI <offset>`: One of the axes you want to move. I is the axis' 