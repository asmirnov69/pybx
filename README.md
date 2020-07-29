# dipole

<b>C</b>hrome + python = full power of ReactJS and pandas in one application

# Install

- pip install --user cefpython3
- sudo apt-get install build-essential libcap-dev
- pip install --user python-prctl
- pip install --user websockets
- python ./appbuild/make.py install examples/js/simplest.dpl/
- python ./appbuild/make.py build examples/js/simplest.dpl/
- python dipole1.py examples/js/simplest.dpl/

# Misc

It would be cool to attach backend via VSC. For python it can be done using pip installable module (debugpy) [https://github.com/microsoft/debugpy/]

Intresting projects about Chrome remote debugger:

- https://craig.is/writing/chrome-logger/
- https://github.com/mirumee/chromedebug

Question on python async mothod override: https://stackoverflow.com/questions/52459138/python-overloading-asynchronous-methods

# TODO

- socket closure should unlock all waiters who would be left hang indefinetly
- timeout implementation
- codegen module should have better type/typedefs support
- pointers arg passing and returns should be handled for all cases
- better Dipole exceptions
- typescript support
