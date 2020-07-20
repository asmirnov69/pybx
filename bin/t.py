import sys
from KVAN import fuargs
import ast

@fuargs.action
def ast_dump(fn):
    source_code = "\n".join(open(fn).readlines())
    pt = ast.parse(source_code)
    print(ast.dump(pt))

if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
    
