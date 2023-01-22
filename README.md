# PrusaSlicer gcode object mover

This program moves specified objects in a file by a specified amount in every possible axis.
It can be used to make multicolor prints with a single extruder printer.
To use it, follow these steps:
1. Create a model with parts that you would like to print in multiple colors, which your printer could feasably do. The program works by first printing the first color, then switching filament, and then printing the next color. Make sure this is possible for your model.
2. Load the model into PrusaSlicer. Make sure the different colored parts are seperate objects in PrusaSlicer.
3. Go to **Print settings -> Output options -> Complete individual objects** and turn it on (this is an advanced option).
4. I'll finish this tutorial later.