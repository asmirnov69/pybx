import typing
import enum
import dipole_idl

"""
if 1:
    # to be defined in libdipole.py
    class struct: pass
    ObjectPtr = typing._alias(object, typing.T, inst = False)
"""

class HelloCB: [] # forward declaration
HelloCBPtr = dipole_idl.ObjectPtr[HelloCB]
#HelloCBPtrSeq = typing.List[HelloCBPtr]

class Color(enum.Enum):
    NORMAL = 0
    RED = enum.auto()
    GREEN = enum.auto()

class Greetings(dipole_idl.struct):
    language: str
    text: str
    color: Color

GreetingsSeq = typing.List[Greetings]

class Hello(dipole_idl.interface):
    def sayHello(self, weSay: str) -> Greetings: []
    def reformatGreetings(self, gs: GreetingsSeq) -> GreetingsSeq: []
    def register_hello_cb(self, cb: HelloCBPtr) -> str: []
   
class HelloCB(dipole_idl.interface):
    def confirmHello(self, hello: str) -> str: []
