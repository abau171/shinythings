import shinythings

scene = shinythings.Scene()
scene.add_sphere(((0.0, 0.0, 0.0), 0.5), ((1.0, 0.0, 0.4), 0.02, 1.0))
scene.add_light((3.0, 3.0, 3.0), (10.0, 10.0, 10.0))
image = scene.render()
image.show()

