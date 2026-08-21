# libarea-feedstock

A conda package for libarea, the 2D closed-curve arithmetic library that sits
on top of Clipper.

## Why it is packaged

Clipper works on an integer lattice, so every boundary that reaches it comes
back as straight segments. libarea is what makes that survivable: an arc is
carried in as a centre and an end point, only walked into segments at the last
moment, and `FitArcs` puts the arcs back on the way out. It also rebuilds the
nesting of the result and re-winds contours by depth, so it does not care what
the input winding claimed.

Two consumers here need exactly that -- FreeCAD's Area module, which already
vendors libarea, and IfcOpenShell's OpenCASCADE kernel, whose 2D subtraction
path currently has to refuse any operand with a curved boundary. Packaging it
means one copy of the library rather than a second vendored fork.

Clipper is built into this package rather than depended on. It is not on
conda-forge under any name, it is a single translation unit, and libarea is
the only thing here that drives it. Its header is installed alongside
libarea's own, so a consumer that wants `ClipperLib` directly gets it from the
same library -- which is the point: two copies of `ClipperLib` in one process
would be interposed on each other by the loader.

## Source

There is no upstream release. Dan Heeks' repository has no tags and no
tarball, and the copy that is actually maintained was the one in FreeCAD's
`src/Mod/Area/libarea` -- which has since been split into a repository of its
own, `../libarea`, the step this package existed to justify. That repository
carries its own CMake build, so the recipe no longer has to supply one; it is
still reached by relative path, from the checkout next door, until there is a
remote with a tag to fetch.

Left out of the repository, and so out of the package:

| Not built | Reason |
| --- | --- |
| `Adaptive.cpp` | FreeCAD's adaptive-clearing toolpaths. LGPL, unrelated to `CArea`, and it uses Clipper directly rather than libarea, so FreeCAD keeps compiling it itself. |
| `AreaDxf.cpp` | Needs FreeCAD's Import module for `dxf.h`. |
| `PythonStuff.cpp`, `pyarea.cpp` | The Boost.Python wrapper. |

## Building

    rattler-build build --recipe recipe/recipe.yaml -m .ci_support/linux_64_.yaml

Nothing outside conda-forge is needed to build it.

## Using it

    find_package(libarea CONFIG REQUIRED)
    target_link_libraries(mytarget PRIVATE libarea::area)

    #include <libarea/Area.h>
    #include <libarea/clipper.hpp>   // only if ClipperLib is wanted directly

`recipe/test_consumer` is the package test and doubles as the smallest
worked example: a square with a round hole, one corner cut off it, checked for
the area it should have lost and for the arcs still being arcs.
