def fix_index(index):
    return index if index < 1 else index - 1

def parse_face_parameter(param):
    vertex_index = None
    texture_coord_index = None
    normal_index = None
    subparams = [sp for sp in param.split("/")]
    if len(subparams) == 0:
        raise Exception
    vertex_index = fix_index(int(subparams[0]))
    if len(subparams) >= 2 and subparams[1] != "":
        texture_coord_index = fix_index(int(subparams[1]))
    if len(subparams) >= 3 and subparams[2] != "":
        normal_index = fix_index(int(subparams[2]))
    return vertex_index, texture_coord_index, normal_index

def parse_obj(filename):
    vertices = []
    triangles = []
    with open(filename) as obj_file:
        for line in obj_file:
            tokens = line.split()
            if len(tokens) == 0:
                continue
            if tokens[0] == "v":
                vertex = tuple(5.0 * float(dim_str) for dim_str in tokens[1:4])
                vertices.append(vertex)
            elif tokens[0] == "f":
                vertex_indices = []
                for token in tokens[1:]:
                    vertex_index, texture_coord_index, normal_index = parse_face_parameter(token)
                    vertex_indices.append(vertex_index)
                if len(vertex_indices) >= 3:
                    triangles.append((vertex_indices[0], vertex_indices[1], vertex_indices[2]))
                if len(vertex_indices) >= 4:
                    triangles.append((vertex_indices[2], vertex_indices[3], vertex_indices[0]))
    return (vertices, triangles, ((0, 0.4, 1), 0.2, 10))

