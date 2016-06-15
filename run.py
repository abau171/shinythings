import time

import shinythings
import shinythings.obj

scene = shinythings.Scene()

scene.set_camera((0,0.5,3), (0,0,-3), (0.66,0,0), (0,-1.1,0))
scene.add_light((-1.2, 2, 3), (8, 8, 8))
scene.add_plane(((0, -0.5, -4), (0, 0, 1)), ((2, 2, 2), 0, 1, 0, False, (0,0,0), 1))
#scene.add_sphere(((0, 0.8, -2), 0.3), ((3, 3, 0), 0, 1, 0, False, (0,0,0), 1))
m = shinythings.Matrix()
m.scale(5)
m.translate(0, -0.5, 0)
m.rotate_z(-0.2)
m.rotate_y(-0.5)
m.rotate_x(-0.5)
m.translate(-0.07, 0.5, 0)
scene.add_model(*shinythings.obj.parse_obj("objects/lucy.obj"), m)

image = scene.render(960//4, 1600//4)
image.show()
image.save(time.strftime("%Y-%m-%d_%H-%M-%S") + ".png", "png")

