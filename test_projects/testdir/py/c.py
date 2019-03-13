# -*- coding: utf-8 -*-  
#!/usr/bin/env python  

import urllib
my_text = '你想搜索的东西'
url = 'http://www.baidu.com/s?wd=%s' % urllib.requests.quote(my_text)
result = urllib.requests.urlopen(url).read()  # 搜索结果页的内容
print(result)  
