class DataWrapper(object):
    def __init__(self, data):
        self.data = data

class MonMap(DataWrapper):
    str = 'mon_map'
    strb = 'strb'
    def __init__(self,data):
        self.strc = 'strc' 
        self.data = data

b = DataWrapper(3)
a = MonMap(b)
print a.data
print MonMap.str
print MonMap.strb
print str(MonMap) 
