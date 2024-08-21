# Library for using userspace-driven ALSA timers

## Compilation

Firstly, set the `INSTALL_HDR_PATH` environment variable to the installed headers path. Since userspace-driven timers are not
in the mainline kernel yet, you have to install headers from compatible kernel (e.g. linux-next), and set the variable to
point to them:

```bash
# export INSTALL_HDR_PATH=<path for headers>
```

After that, build the library:
```bash
# make
```

Build the test and the example application as well:

```bash
# make tests
# cd example
# make
```

## Testing

In the compatible environment (i.e. the kernel with userspace-driven timers implemented and enabled), run the `test` unit-test
to make sure that the library functions correctly:

```bash
# cd tests
# ./test
```

You should see the following line if the test succeeds:
```
All of the tests passed!
```

## Sample application

Please, find the sample application which uses userspace-driven ALSA timers in the `examples/` subfolder.

The application creates the virtual userspace-driven ALSA timer and triggers it 10 times.
