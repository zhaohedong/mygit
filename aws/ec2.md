- autoscaling
	- 负载均衡器
		- 负载均衡器负载把流量路由到目标群组中的实例/ip地址
		- Elastic Load Balancing 跨多个可用区中的多个目标（如 Amazon EC2 实例、容器和 IP 地址）分发传入应用程序或网络流量。Elastic Load Balancing 会在应用程序的传入流量随时间的推移发生更改时扩展负载均衡器。它可以自动扩展来处理绝大部分工作负载。
	- 目标群组
		- 负载均衡器使用您指定的协议和端口将请求路由至此目标组中的目标，并使用您指定的运行状况检查设置对目标执行运行状况检查
	- Autoscaling group
		- Elastic Load Balancing 启用 Auto Scaling，则 Auto Scaling 所启动的实例会自动注册到负载均衡器。同样，Auto Scaling 所终止的实例会自动从负载均衡器取消注册。
		- autoscaling 组使用启动模块，完成实例的伸缩。
		- 负载均衡器和目标组可以附加到AutoScaling组，完成自动伸缩。
	- 控制策略
	```
	策略类型:
	分步扩展
	执行策略的时间:
	awsec2-zhao-as-02-Low-CPU-	
	达到警报阈值: CPUUtilization <= 10，持续 300 秒
	用于指标维度
	AutoScalingGroupName = zhao_as_02
	请执行以下操作:
	删除	
	1
	% 的组	
	当
	10
	>=
	CPUUtilization
	>
	-无限
	移除实例时至少需移除
	1
	个实例

	```
	
