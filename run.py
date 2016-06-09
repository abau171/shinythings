import time

import shinythings
import shinythings.obj

scene = shinythings.Scene()
#scene.add_sphere(((0.25, 0.35, 0.25), 0.15), ((1, 0, 0.4), 0.05, 10))
#scene.add_sphere(((-0.25, -0.25, -0.25), 0.25), ((0.4, 0, 1), 0.2, 100))
#scene.add_plane(((0, -0.5, 0), (0, 1, 0)), ((0, 1, 0.4), 0, 1))
scene.add_light((3, 3, 3), (10, 10, 10))
#scene.add_model(
#    [(0, 0.65, 0), (-0.5, 0.15, -0.25), (0.5, 0.15, -0.25), (0, 0.15, 0.25)],
#    [(0, 2, 1), (0, 1, 3), (0, 3, 2), (1, 2, 3)],
#    ((0, 0.4, 1), 0, 1))
#scene.add_model(*shinythings.obj.parse_obj("objects/box.obj"))
scene.add_model(*shinythings.obj.parse_obj("objects/suzanne.obj"))
image = scene.render(1920, 1080)
image.show()
image.save(time.strftime("%Y-%m-%d_%H-%M-%S") + ".png", "png")

