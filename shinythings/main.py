import PIL.Image

import _shinythings

class Scene():

    def __init__(self):
        self._scene = _shinythings.Scene()

    def add_sphere(self, *args):
        self._scene.add_sphere(*args)

    def add_plane(self, *args):
        self._scene.add_plane(*args)

    def add_light(self, *args):
        self._scene.add_light(*args)

    def add_model(self, *args):
        self._scene.add_model(*args)

    def set_camera(self, *args):
        self._scene.set_camera(*args)

    def render(self, width, height):
        return PIL.Image.fromarray(self._scene.render(width, height), "RGB")

