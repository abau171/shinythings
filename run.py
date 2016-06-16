import time

import shinythings
import shinythings.obj

scene = shinythings.Scene()

scene.set_camera((0,0,3), (0,0,-3), (1.92,0,0), (0,-1.08,0))
scene.set_background_color(0.05, 0.3, 0.6)
scene.set_background_color(0.05, 0.3, 0.6)

scene.add_light((-1.2, 2, 3), (8, 8, 8))
scene.add_light((0, 2000, 100), (40, 40, 40))

scene.add_plane(((0, -0.5, 0), (0, 1, 0)), ((1, 1, 1), 0, 1, 0, False, (0,0,0), 1))

scene.add_sphere(((-0.5, 0.4, -1.2), 0.6), ((1, 1, 1), 0.1, 10, 0.5, False, (0,0,0), 1))
scene.add_sphere(((-0.1, -0.2, 0.1), 0.15), ((0.02, 0.02, 0.02), 0.04, 10, 0, True, (0,0,0), 1.1))

m = shinythings.Matrix()
m.scale(4.5)
m.translate(0, -0.5, 0)
m.rotate_z(-0.2)
m.rotate_y(-0.5)
m.rotate_x(-0.5)
m.translate(0.6, 0.05, 0)
scene.add_model(*shinythings.obj.parse_obj("objects/lucy.obj"), ((0.1, 0.1, 0.1), 0.2, 20, 0, True, (80, 30, 5), 1.01), m)

m = shinythings.Matrix()
m.scale(0.05)
m.rotate_y(0.5)
m.translate(-0.5, -0.4, 0)
scene.add_model(*shinythings.obj.parse_obj("objects/dragon.obj"), ((0.4, 1, 0), 0.5, 40, 0, False, (1, 1, 1), 1), m)

m = shinythings.Matrix()
m.scale(0.3)
m.rotate_x(-0.5)
m.rotate_y(0.3)
m.translate(0.2, -0.2, -0.8)
scene.add_model(*shinythings.obj.parse_obj("objects/suzanne.obj"), ((1, 0.1, 0.1), 0.01, 10, 0, False, (1, 1, 1), 1), m)

m = shinythings.Matrix()
m.scale(3)
m.translate(0.6, 0, -0.8)
scene.add_model(*shinythings.obj.parse_obj("objects/bunny.obj"), ((0.7, 0, 1), 0.001, 1, 0, False, (1, 1, 1), 1), m)

image = scene.render(1920//2, 1080//2)
image.save(time.strftime("%Y-%m-%d_%H-%M-%S") + ".png", "png")
image.show()

