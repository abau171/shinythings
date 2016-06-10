import time

import shinythings
import shinythings.obj

scene = shinythings.Scene()

scene.set_camera((0,0.1,3), (0,0,-3), (0.6,0,0), (0,-0.3,0))
scene.add_light((-1.2, 2, 3), (8, 8, 8))

scene.add_sphere(((0, 0.1, -2), 0.3), ((1, 1, 1), 0, 1, 0, False, (0, 0, 0)))
scene.add_model(*shinythings.obj.parse_obj("objects/teapot.obj"))

image = scene.render(300,150)
image.show()
image.save(time.strftime("%Y-%m-%d_%H-%M-%S") + ".png", "png")

