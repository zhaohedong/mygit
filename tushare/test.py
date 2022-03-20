from os import path
import tushare as ts
ts.set_token('8ee1e90e876d4c2760947fca5cfe903eb39205de80c20385022d3208')
pro = ts.pro_api()
df = pro.query('daily', ts_code='600718.SH', start_date='20200228', end_date='20290302')
print (df)

