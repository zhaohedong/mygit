# OpenStack通用技术
- REST（Representational State Transfer）
  - 如果一个软件架构符合REST原则，我们就称它为RESTful架构。
RESTful架构的一个核心概念是“资源”。从RESTful的角度看，网络中的任何东西都是资源。它可以是一段文本、一张图片、一首歌曲、一种服务等，每个资源都对应于一个特定的URL，并用它进行标示，访问这个URL就可以获得此资源。
资源可以有多种具体的表现形式，也就是资源的“表述”（Represent），比如一张图片可以是JPEG格式也可以是PNG格式，URL代表的是资源实体，而不是表现形式。
客户端和服务端之间的互动传递就是资源的表述。我们上网浏览网页，就是在调用资源的URL，获取它不同表现形式的过程，这种互动只能使用无状态协议HTTP，也就是说，服务端必须保存所有的状态，**客户端可以使用包括*GET、POST、PUT和DELETE*这些在内的基本操作，使服务端上的资源发生“状态转移”，也就是所谓的“*表述性状态转移*”**
- RESTful路由
  - OpenStack的API服务进程接收到客户端的HTTP请求时，一个所谓的路由模块会将请求的URL转换成响应的资源，并路由到合适的操作函数上。
- WSGI（Web Server Gateway Interface）
  -

- 什么是块存储？

- Cinder的功能
  - 与Nova利用主机本地存储为虚拟机提供的临时存储不同，Cinder为虚拟机提供持久化的块存储能力，实现虚拟机存储卷（Volume）的创建、挂载、卸载、快照等生命周期管理。

## Cinder
- Cinder 架构
  - Cinder 主要由 3 个组件组成，cinder-api, cinder-scheduler 和 cinder-volume，如下图所示。
    ![](./images/image001.gif)
  - Topic
  ```
  API_BINARY = "cinder-api"
  SCHEDULER_BINARY = "cinder-scheduler"
  VOLUME_BINARY = "cinder-volume"
  BACKUP_BINARY = "cinder-backup"
  SCHEDULER_TOPIC = SCHEDULER_BINARY
  VOLUME_TOPIC = VOLUME_BINARY
  BACKUP_TOPIC = BACKUP_BINARY
  ```      

### 创建 volume 的 RPC 通信过程
- Create volume 的过程可以看做是两个阶段的 RPC 请求处理
    - 第一阶段：(cinder-api -> cinder-scheduler)
      1. Client 即 cinder-api 调用 RPC.cast 发送消息，RPC.cast 创建 Topic Publisher 对象，topic 为 cinder-scheduler，也就是消息的 Routing Key，Exchange name 为 OpenStack，消息体中标记了消息接收者即 Server 所需要调用的方法“create_volume”；
      2. Exchange 接收到消息，根据 Routing Key 将消息发送至 Message Queue cinder-scheduler 上，这个 Message Queue 是由 cinder-scheduler 服务的 Topic Consumer 订阅的，因此，cinder-scheduler 服务将接收到 create_volume 的消息
      3. Consumer 调用注册的 RPC_dispatcher，即 cinder-scheduler manager，然后由 cinder-scheduler manager 调用 RPC 处理方法 create_volume
      ![](./images/create_volume_rpc1.jpg)

    - 第二阶段 (cinder-scheduler -> create_volume)
      1. Cinder-scheduler 调用 RPC.cast，Exchange name 为 Openstack, routing-key 为 cinder-volume:host
      2. Exchange 接收到消息，根据 Routing Key 将消息发送到队列 cinder-volume:host 上
      3. 在主机 host 上的 Cinder-volume consumer 从 Queue 中接收到 message，回调 RPC_dispatcher 即 cinder-volume manager
      4. Cinder-volume manger 根据消息体中 method 信息，调用 create_volume 方法，创建 volume
      ![](./images/create_volume_rpc2.jpg)

- Cinder具体实现的起点
    ```
    console_scripts =
    cinder-api = cinder.cmd.api:main
    cinder-backup = cinder.cmd.backup:main
    cinder-manage = cinder.cmd.manage:main
    cinder-rootwrap = oslo_rootwrap.cmd:main
    cinder-rtstool = cinder.cmd.rtstool:main
    cinder-scheduler = cinder.cmd.scheduler:main
    cinder-volume = cinder.cmd.volume:main
    cinder-volume-usage-audit = cinder.cmd.volume_usage_audit:main
    ```
- CinderAPI的执行过程
  1. cinderclient将用户命令转换为标准的HTTP请求
  2. Paste Deploy将请求路由到具体的WSGI Application，比如v1 API对应的WSGI Application
    - Cinder API服务cinder-api在此阶段开始参与，会创建一个WSGI Server去监听用户的HTTP请求。Paste Deploy路由过程主要依赖于配置文件/etc/cincer/api-paste.ini.
  3. Routes将请求路由到具体函数并执行
***
- 相关主题
  - [参考 IBM developerworks, Openstack RPC 通信原理](https://www.ibm.com/developerworks/cn/cloud/library/1403_renmm_opestackrpc/)
