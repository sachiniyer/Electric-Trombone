# Main Changes

## Frictional Issues
This version takes a lot from the last version, but instead aims to distribute force a lot more effectively. Last attempt was not force efficient at all resulting in a lot of unnecessary friction. The main source of this friction was due to the 90 degree angle between the gearing mechanism and the slide, which made it very difficult to pull. This version instead mounts everything at the end of the slide which means that it should be interacting at a 180 degree angle allowing the string to interface directly and transfer more of the force to the constant force spring. The major downfall of this may be that it is harder to hold with a lot of the weight at the end of slide (this is what I was trying to avoid initially). To solve this problem, I've cut down on the amount of moving parts by as much as possible.

## Shearing Issues
The second model was built with the main shaft standing tall on the build plate. This meant that rotational force on the shaft cause the print layers to split more often than they needed to. This build I'm making a conscious decision to build the main shaft horizontally, along with more chamfers in high torque areas to distribute the force more evenly.

## Constant Force Spring Issues
The amount of measurements and calculations I had done when selecting the constant force spring and building the mount were very rudimentary at the start. I was not taking advantage of the full length of the spring and passing on too little force to the slide. This time, I intentionally created a difference in radius between the portion of shaft that interfaces with the string connected to the slide and the portion of the shaft that interfaces with the spring. This was calculated to use the whole spring over the course of the slide length. It should deliver more force and be more stable due to increased strength hopefully.

## Mounting Electronics
This time, I am making better mounting hardware. I am also aiming to use as little duct tape in this version as possible (the last two were full of it).
