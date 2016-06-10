import time

import shinythings
import shinythings.obj

scene = shinythings.Scene()
#scene.add_sphere(((0.25, 0.35, 0.25), 0.15), ((1, 0, 0.4), 0.05, 10))
#scene.add_sphere(((-0.25, -0.25, -0.25), 0.25), ((0.4, 0, 1), 0.2, 100))
#scene.add_plane(((0, -0.5, 0), (0, 1, 0)), ((0, 1, 0.4), 0, 1))
scene.add_light((-1.2, 2, 3), (8, 8, 8))
#scene.add_model(
#    [(0, 0.65, 0), (-0.5, 0.15, -0.25), (0.5, 0.15, -0.25), (0, 0.15, 0.25)],
#    [(0, 2, 1), (0, 1, 3), (0, 3, 2), (1, 2, 3)],
#    ((0, 0.4, 1), 0, 1))
#scene.add_model(*shinythings.obj.parse_obj("objects/box.obj"))
scene.set_camera((0,0.5,3), (0,0,-3), (0.66,0,0), (0,-1.1,0))
scene.add_model(*shinythings.obj.parse_obj("objects/lucy.obj"))
image = scene.render(600,1000)
image.show()
image.save(time.strftime("%Y-%m-%d_%H-%M-%S") + ".png", "png")

