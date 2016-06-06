import PIL.Image

import _shinythings

def render(width, height):
    return PIL.Image.fromarray(_shinythings.render(width, height), "RGB")

