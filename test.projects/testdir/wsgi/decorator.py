# decorator.py
def log(func):
    def wrapper(*args, **kw):
        print 'call %s():' % func.__name__
        return func(*args, **kw)
    return wrapper
import functools

def log2(text):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kw):
            print '%s %s():' % (text, func.__name__)
            return func(*args, **kw)
        return wrapper
    return decorator

@log2('aaa')
def now():
    print '2013-12-25'

def test1():
    print 'hello python1!'

def test2():
    print 'hello python2!'

def test():
    print 'hello python!'

now()
