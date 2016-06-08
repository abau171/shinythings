from distutils.core import setup, Extension
import numpy

module = Extension("_shinythings",
                   sources=["_shinythings/_shinythings.c",
                            "_shinythings/_shinythings_Scene.c",
                            "_shinythings/render.c",
                            "_shinythings/color.c",
                            "_shinythings/py_parse.c",
                            "_shinythings/kd_tree.c",
                            "_shinythings/scene.c",
                            "_shinythings/geometry.c"],
                   include_dirs=[numpy.get_include(), "_shinythings/"],
                   extra_compile_args=["-O3"])

setup(name="shinythings", packages=["shinythings"], ext_modules=[module])

