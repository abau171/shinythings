import shinythings

scene = shinythings.Scene()
scene.add_sphere(((0.25, 0.35, 0.25), 0.15), ((1.0, 0.0, 0.4), 0.05, 10.0))
scene.add_sphere(((-0.25, -0.25, -0.25), 0.25), ((0.4, 0.0, 1.0), 0.2, 100.0))
scene.add_light((3.0, 3.0, 3.0), (10.0, 10.0, 10.0))
image = scene.render()
image.show()

