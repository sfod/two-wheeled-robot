# two-wheeled-robot

Test project for two wheeled robot.


### Design

Robot can do two things:
1. Turn around itself.
2. Move to the specified coordinates.

When a user specifies new target robot rotates until it's directed to the target.
Then it moves to the target.
After robot gets to the target it rotates again until it reaches the specified orientation.

Users could set multiple targets. In that case robot will visit targets one after another in the order of adding.

##### Available commands
Robot receives commands via HTTP API

- POST goto

  Move to the specified coordinates.
  ```
  curl -i -X POST '127.0.0.1:5001/goto' --data '{"x": 12.9, "y": 10.4, "theta": 1, "max-speed": 1.3, "acceleration": 0.2}'
  ```

- GET stop

  Stop the robot and discard targets.
  ```
  curl -i '127.0.0.1:5001/stop'
  ```

- GET pause

  Pause robot, targets remain active.
  ```
  curl -i '127.0.0.1:5001/pause'
  ```

- GET resume

  Resume movement to the next target.
  ```
  curl -i '127.0.0.1:5001/resume'
  ```

- GET status

  Return robot's current coordinates, orientation and speed.
  ```
  curl -i '127.0.0.1:5001/status'
  ```

### TODOs and Thoughts

1. Robot sets orientation with some inaccuracy. When it moves toward the target inaccuracy increases, so it's possible to drive past the target.\
   The solution is to periodically correct the course during the movement.

2. The Wheel class works with a linear speed. It should work with angular speed instead.

3. Use strong types (e.g. Meter, MeterPerSec).

4. I'd use POST with /stop request as it changes the state of the robot.

5. Probably it'll be better to remove sleep() statement from the main loop: the app will consume 100% CPU but accuracy of the movements will improve. Or calculate sleep period by the specified accuracy.
