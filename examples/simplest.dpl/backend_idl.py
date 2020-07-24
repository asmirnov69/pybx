import typing, enum

if 1:
    # to be defined in libdipole.py
    class struct: pass
    ObjectPtr = typing._alias(object, typing.T, inst = False)
    
class HelloCB: [] # forward declaration
HelloCBPtr = ObjectPtr[HelloCB]
#HelloCBPtrSeq = typing.List[HelloCBPtr]

class Color(enum.Enum):
    NORMAL = 0
    RED = enum.auto()
    GREEN = enum.auto()

class Greetings(struct):
    language: str
    text: str
    color: Color

class Hello:
    def sayHello(self, weSay: str) -> Greetings: []
    def register_hello_cb(self, cb: HelloCBPtr) -> str: []
   
class HelloCB:
    def confirmHello(self, hello: str) -> str: []

