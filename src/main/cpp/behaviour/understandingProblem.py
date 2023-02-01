
# -x => +x
# -y ^ +y


# current angle, current position, centre of mass position (relative to robot centre), convergence point

class Position2D():
    def __init__(self, x=None, y=None):
        self.x = x
        self.y = y

class Pose():
    def __init__(self, position=None, angle=None):
        self.position = position
        self.angle = angle


class Robot():
    def __init__(self):
        self.angle = 0
        self.currentPosition = Pose(Position2D(0, 0), 0)
        self.centreOfMassPosition = Position2D(0, 0)

        self.convergencePoint = Position2D(3, None)




