from __future__ import annotations 
import typing, enum, dataclasses
import dipole_idl

class Color(enum.Enum):
    NORMAL = 0
    RED = enum.auto()
    GREEN = enum.auto()

@dataclasses.dataclass
class Greetings:
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

HelloCBPtr = dipole_idl.ObjectPtr[HelloCB]
#HelloCBPtrSeq = typing.List[HelloCBPtr]

