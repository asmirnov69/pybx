import typing, enum

"""
# future ObjectPtr passing example
ObjPtr = typing._alias(object, typing.T, inst = False)
class HelloCB: [] # forward declaration
HelloCBPtr = ObjPtr[HelloCB]
HelloCBPtrSeq = typing.List[HelloCBPtr]
"""

class Color(enum.Enum):
    NORMAL = 0
    RED = enum.auto()
    GREEN = enum.auto()

class Greetings(dipole_idl.struct):
    language: str
    text: str
    color: Color

class Hello:
    def sayHello(self, weSay: str) -> Greetings: []
    #def sayAloha(self, language: str) -> str: []
    #def get_holidays(self) -> str: []
    ###def register_hello_cb(self, cb: HelloCBPtr) -> None: []

"""    
class HelloCB:
    def confirmHello(self, hello: str) -> str: []
"""
