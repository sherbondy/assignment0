Just run `make` to build.
I've also included libvecmath.
To build that, cd into `vecmath` and run `make`.
It'll build `libvecmath.a` inside `lib.`

All of the Mac OS specific code has been implemented
with ifdefs or (for the makefiles), checking for Darwin.
The application should still run swimmingly on Linux.

Then try `./a0 < gargoyle.obj`.

a0soln is not included since the binary
was compiled for linux machines.

The following websites proved helpful:
- cplusplus.com for information on stringstreams and vectors.
- http://macdevcenter.com/pub/a/mac/2005/04/01/opengl.html for an introduction to building OpenGL applications on MacOS (slightly outdated).
- http://alumni.cs.ucsb.edu/~wombatty/tutorials/opengl_mac_osx.html for more build tips.
- http://www.songho.ca/opengl/gl_transform.html for help thinking about transformations in OpenGL

I have implemented mouse drags for rotation about the y axis, and I plan to do more extra credit time allowing.

So far, I've spent about 5 hours on the assignment, mostly because I really wanted to learn about make and how to build on OSX.
