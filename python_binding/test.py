import sys
import os
from pathlib import Path

CWD = os.path.dirname(os.path.realpath(__file__))

# Global build folder
sys.path.append(Path(CWD).parent.absolute().joinpath(
    "build/python_binding").as_posix())

# Local build fonder
sys.path.append(Path(CWD).joinpath("build").as_posix())

if __name__ == "__main__":
    # Import dynamically linked library
    import vector_of_struct_py

    # Get a vector from the library
    struct_vector = vector_of_struct_py.createStructVector(5)

    # Print vector
    vector_of_struct_py.printStructVector(struct_vector)

    # Modify values of a vector
    struct_vector[0].x = 10
    struct_vector[0].y = 15
    struct_vector[0].vec = [0, 1, 2, 3, 4, 5]

    # Print modified vector
    vector_of_struct_py.printStructVector(struct_vector)
