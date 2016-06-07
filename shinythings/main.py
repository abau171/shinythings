import PIL.Image

import _shinythings

class Scene():

    def __init__(self):
        self._scene = _shinythings.Scene()

    def add_sphere(self, *args):
        self._scene.add_sphere(*args)

    def add_light(self, *args):
        self._scene.add_light(*args)

    def render(self):
        return PIL.Image.fromarray(self._scene.render(), "RGB")

