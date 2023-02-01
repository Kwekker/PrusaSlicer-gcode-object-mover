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
- `-O <filename>`: Specify the output file. If you don't use this flag, the input file will be overwritten.
- `-F`: Force the program to overwrite the file without asking for confirmation.
- `-N <name>`: The object's name in prusaslicer
- `-a<I> <offset>`: One of the axes you want to move. I is the axis' identifying character (case sensitive). Moving the object 25mm along the X axis would be -aX 25. Supports negative values.
- `-c`: Don't change colors(filament) before printing this object .

### Example
    
```bash
./objectMover print.gcode -O moved.gcode -N text -aX -40 -aY 30 -N otherText -aX 50 -aY -30 -c
```
Broken down into a list:
```bash
./objectMover print.gcode
    -O moved.gcode  # Write the output to a file named 'moved.gcode'

    -N text         # Start specifying object 'text'
        -aX -40     # Move object 'text' -40mm along the X axis
        -aY 30      # Move object 'text 30mm along the Y axis

    -N otherText    # Start specifying object 'otherText'
        -aX 50      # Move 'otherText' 50mm along the X axis
        -aY -40     # Move 'otherText' -40mm along the Y axis
        -c          # Don't change filaments before printing this object
```

## Purpose
I made this program because I wanted to print objects in multiple colors with my Prusa MINI, with as few filament changes as possible. I was doing this by just printing the first part first, and then printing the other part over it. This was pretty limiting and required me to start a new print for every extra color I added. So I went looking for a better solution. I could either see if someone else had already solved this problem before me, or spend hours thinking of and making one myself. I decided on the latter, the result of which you are now looking at right now.