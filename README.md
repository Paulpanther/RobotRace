# RobotRacer
Assignment for Building Interactive Devices at HPI

![Robot](https://github.com/Paulpanther/RobotRace/blob/master/robot.jpg)

### Goal
A projector shows [black, white and grey circles](http://xman.tw/radial.html) on the ground. Based on the light, the robot has to manouver to the center and be faster than the other robots

### About our Robot
We use 3 Photodiodes to sense the light. The Sensors are arranged in a triangle, with one at the front, one at the left and another one at the right side. A simple algorithm finds maxima in the light histogram to determine if we are in a black, grey or white circle.   
The drive logic works like this:
- If all sensors have the same value: Drive forward
- If all sensors have different values: Drive forwards
- If the front is different than the two sensor in the back, drive forwards or turn around, depending on what color points to the goal direction
- If one side sensor is different than front and the other, turn either towards the sensor, or away from it, depending on the color

### How to build it
1. Setup [platformio](https://platformio.org/)
2. Clone this repo and compile the code in the `software/` directory
3. Lasercut the `cuttingPlan.svg` in the hardware directory (It's made with Adobe Illustrator. In other programs it might not be loading correctly). 
Put some rubber bands around the frame so it holds together. 
Put the motors through the holes and use screws to clamp them to the frame.
Do the wiring like in `schematic.pdf` and stack everything together (Use some cardboard to attach the sensors).
You can put shrink tube on the motor axle to have more friction
4. Plug the Robot in, upload the software.
5. Try it out. You may need to move the robot a bit at the start, to let it see all the circles. The algorithm will give better results the long it is running.
6. Debug it with the USB-cable attached. It will output what the Sensors see

### Magic Numbers / Constants
- Pin Numbers: `main.cpp (8-12)` Motors should use only pwm-able pins
- Bucket Count: `Sensor.h 7` How many different light values are counted
- Max Value: `Sensor.h 8` The maximum value a photo diode will read. This should be as low as possible
- Min Bucket Distance: `Sensor.h 10` How far apart maxima in the histogram have to be, to be counted
- Time Drive / Time Wait Ratio: `Driver.h 5, 6` If the Motor is on all the time, it will shut down the Arduino, so we have to turn it off and on again. This ratio will effect the speed
- Speed forward, Speed turn: `Driver.h 8, 9` How fast the Motors are spinning
- Motor Min/Max: `Motor.h` The maximum and minium (Our motor stopped if they got a value under 130) value for the motors
- Offset between the Motors: `main.cpp 19, 20` Set positive integers here, which will get subtracted from the motor speed. Use this if your robot is not driving streight 

### List of Materials
TODO
