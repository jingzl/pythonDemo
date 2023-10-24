import client
from module1.client2 import Func

if __name__ == "__main__":
    print( __name__ )
    client.func()
    print( client.__name__ )
    
    f = Func()
    res = f.add( 10, 20 )
    print( res )



