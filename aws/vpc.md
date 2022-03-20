- 私有ip地址
```
     ---------      -----      -----      -----
A类 | 0******* | . | Host | . | Host | . | Host |
     ---------      -----      -----      -----
0 ~ 126

127 (01111111) 是一个A类地址，是给loopback testing保留的，不能在network中使用。

     ---------      --------      -----      -----
B类 | 10****** | . | NetWork | . | Host | . | Host |
     ---------      --------      -----      -----
128 ~ 191

     ---------      --------      --------      -----
C类 | 110***** | . | NetWork | . | NetWork | . | Host |
     ---------      --------      --------      -----
192 ~ 223    
参考https://www.jianshu.com/p/6a7a9e6f478d



Class	Private Address Range
A	10.0.0.0 ~ 10.255.255.255
B	172.16.0.0 ~ 172.31.255.255
C	192.168.0.0 ~ 192.168.255.255
```
- 实践
	- 4091 ips ( 2**10 - 5)
		- 172.31.144.0/20   
		- 10101100.00011111.1001xxxx.xxxxxxxx (12 bits x, can dynamically change)
		- 172.31.144.1 ~ 172.31.159.255
	- 4091 ips ( 2**10 - 5)
		- 172.31.160.0/20
		- 10101100.00011111.1010xxxx.xxxxxxxx (12 bits x, can dynamically change)
		- 172.31.160.1 ~ 172.31.175.255
    - 1019 ips 
		- 172.31.128.0/22
		- 10101100.00011111.100000xx.xxxxxxxx (10 bits x, can dynamically change)
		- 172.31.128.1 ~ 172.168.131.255

- nat
	- nat服务器可以用来把内网的流量转发到internet
	- 需要保证nat服务器可以与internet通信，同时与局域网通信, 并设定ipv4_forward=1
	- 局域网的机器，default路由要执行nat网关
	- 参考aws in action 第六章
- AWS Direct Connect
	- AWS Direct Connect 通过标准的以太网光纤电缆将您的内部网络链接到 AWS Direct Connect 位置。电缆的一端接到您的路由器，另一端接到 AWS Direct Connect 路由器。有了此连接以后，您就可以创建直接连接到公共 AWS 服务（如 Amazon S3）或 Amazon VPC 的虚拟接口，从而绕过您的网络路径中的 Internet 服务提供商。
