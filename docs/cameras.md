## Using OpenGL without a X11 server

When running experiments on a remote computer, for instance via ssh or a cluster environment, a X11 server might not be available, which will cause problems when using any OpenGL call (e.g., WindowlessGLApplication). To avoid this you can create a dummy X11 server with:
`xinit -- :0 -nolisten tcp vt$XDG_VTNR -noreset +extension GLX +extension RANDR +extension RENDER +extension XFIXES &`
Then, you can start your application as usual.
