## Using cameras without a X11 server

When running experiments on a remote computer, for instance via ssh, a X11 server might not be available, which might cause problems while using the Cameras to record images from the simulations. To avoid this you can create a dummy X11 server with:
`xinit -- :0 -nolisten tcp vt$XDG_VTNR -noreset +extension GLX +extension RANDR +extension RENDER +extension XFIXES &`
Then, you can start your application as usual.


