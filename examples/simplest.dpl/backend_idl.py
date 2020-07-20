import typing
ObjPtr = typing._alias(object, typing.T, inst = False)

class HelloCB: []

HelloCBPtr = ObjPtr[HelloCB]
HelloCBPtrSeq = typing.List[HelloCBPtr]

class Hello:
    def sayHello(self) -> str: []
    def sayAloha(self, language: str) -> str: []
    def get_holidays(self) -> str: []
    def register_hello_cb(self, cb: HelloCBPtr) -> None: []

class HelloCB:
    def confirmHello(self, hello: str) -> str: []
    
