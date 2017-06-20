# Cinder
## 关于存储
  - 存储是OpenStack所管理的最重要的资源之一。
  - Nova实现了OpenStack虚拟机世界的抽象，并利用主机的本地存储作为虚拟机的“临时存储（Ephemeral Storage）”。
  - 基于SAN，NAS等不同的类型的存储设备，Swift（对象存储）与块存储（Cinder）引入了“永久存储（Persistent Storage）”
    - SAN
      - 存储区域网络（Storage Area Network，简称SAN）采用网状通道（Fibre Channel ，简称FC，区别与Fiber Channel光纤通道）技术，通过FC交换机连接存储阵列和服务器主机，建立专用于数据存储的区域网络。（http://baike.baidu.com/item/存储区域网络/6091260?fromtitle=SAN&fromid=10789152）
    - NAS
      - NAS被定义为一种特殊的专用数据存储服务器，包括存储器件（例如磁盘阵列、CD/DVD驱动器、磁带驱动器或可移动的存储介质）和内嵌系统软件，可提供跨平台文件共享功能。（http://baike.baidu.com/link?url=_Jt-piYvwJ3lvkJbzpKmXcziG6BzeByF16zOc1BjV0-d3m5kJXK4WFUuOHRdqVPPlbRgYWVEPetvUYlBZwIfAq）
    - 对象存储
      - Object storage (also known as object-based storage[1]) is a computer data storage architecture that manages data as objects, as opposed to other storage architectures like file systems which manage data as a file hierarchy and block storage which manages data as blocks within sectors and tracks.[2] Each object typically includes the data itself, a variable amount of metadata, and a globally unique identifier. （https://en.wikipedia.org/wiki/Object_storage）
      - 对象存储是无层次结构的数据存储方法，通常用于云中。不同于其他数据存储 (英文) 方法，基于对象的存储不使用目录树。各个单独的数据（对象）单元存在于存储池中的同一级别。每个对象都有唯一的识别名称，供应用进行检索。此外，每个对象可包含有助于检索的元数据。（https://www.ibm.com/cloud-computing/cn-zh/object-storage/）
      - 适合存放静态数据（长期不会发生更新的数据）
    - 块存储
      - 块是一个字节序列（例如，一个512字节的数据块）。基于块的存储接口是最常见的存储数据方法，它们基于旋转介质，像硬盘、 CD 、软盘、甚至传统的 9 磁道磁带。基于块设备的存储方式，定义为块存储。（http://docs.ceph.org.cn/rbd/rbd/）
      - 适合存放需要实时更新的数据

## 历史
  - Cinder的前身是Nova中的nova-volume服务，在Folsom版本发布时，从Nova中剥离作为一个独立的OpenStack项目存在。

## 功能
- 与Nova利用主机本地存储为虚拟机提供的临时存储不同，Cinder为虚拟机提供**持久化的**块存储能力，实现虚拟机**存储卷（Volume）的创建、挂载、卸载、快照等生命周期管理**。

## 结构
- overview
  ![](./../images/cinder-arch01.png)
  - DB: sql database for data storage. Used by all components (LINKS NOT SHOWN).
  - Web Dashboard: potential external component that talks to the api.
  - api: component that receives http requests, converts commands and communicates with other components via the queue or http.
  - Auth Manager: component responsible for users/projects/and roles. Can backend to DB or LDAP. This is not a separate binary, but rather a python class that is used by most components in the system.
  - scheduler: decides which host gets each volume.
  - volume: manages dynamically attachable block devices.

- cinder如何为nova提供Volume
  ![](./../images/cinder-arch02.png)
  - iSCSI
  OSI七层模型中，iSCSI属于传输层协议，规定了iSCSI Target和iSCSI Initiator之间的通信机制。

- 实现方法
  - Cinder本身不直接提供块存储设备实际的管理和服务，而是在虚拟机和具体的存储设备之间引入一个抽象的“逻辑存储卷”。Cinder通过添加不同厂商的Driver来整合后端存储设备，并通过提供统一的API接口的方式为云平台提供持久性的块存储服务，类似于Amazon的EBS（Elastic Block Storage）。
- 核心模块
  - cinder-client（作为开端，非核心模块）
    - cinder命令的可执行文件，将用户命令转换位标准的HTTP请求
  - cinder-api
    - 提供RESTful API接口的统一卷管理接口，其主要功能是接受客户端发来的HTTP请求，然后进行用户身份认证和消息分发。Cinder API本质是一个WSGI App，启动Cinder API服务相当于启动一个名为osapi_volume的WSGI服务去监听Client的HTTP请求。实现上主要依赖WSGI服务器、WSGI App、Paste Deploy和路由器技术。
  - cinder-volume
    - 运行在存储节点上管理具体存储设备的存储空间，每个存储节点上都会运行一个cinder-volume服务，多个存储节点构成存储资源池。
    - Volume生命周期管理的真正实现模块。
      - manager.py是cinder-volume最为核心的代码，其中的VolumeManager类用于执行接收到的RPC请求，包含所有有关Volume的生命周期管理的函数。
  - cinder-scheduler
    - 根据预定的策略选择合适的cinder-volume节点来处理用户的请求。
  - cinder-backup
    - 用于提供卷的备份功能，支持将块存储设备备份到OpenStack备份存储后端，如Swift、Ceph、NFS等。

## 其他
- Cinder里面使用的wsgi源自于eventlet库，对应eventlet.wsgi.server
  ```
  from oslo_service import wsgi
  import eventlet.wsgi
  ```
- OpenStack的日志

- 格式 <时间戳><日志等级><代码模块><Request ID><日志内容><源代码位置>

  - 时间戳：日志记录的时间，包括 年 月 日 时 分 秒 毫秒
  - 日志等级：有INFO WARNING ERROR DEBUG等
  - 代码模块：当前运行的模块
  - **Request ID日志会记录连续不同的操作，为了便于区分和增加可读性，每个操作都被分配唯一的Request ID,便于查找**
  - 日志内容：这是日志的主体，记录当前正在执行的操作和结果等重要信息
  - 源代码位置：日志代码的位置，包括方法名称，源代码文件的目录位置和行号。这一项不是所有日志都有
- RESTful是什么？
  - Representational State Transfer，表述性状态转移。
  - RESTful架构的核心概念是“资源”。网络里的任何东西都是资源，每个资源都对应一个特定的URI（统一资源定位符），访问URI就可以“操作”这个资源。
  - 互联网里，客户端和服务器之间的互动传递的就只是资源的表述，我们上网的过程，就是调用资源的URI，获取它不同表现形式的过程。
  - 互动只能使用无状态协议HTTP，服务端保存所有状态，客户端使用HTTP的几个基本动作，包括GET，POST，PUT，DELETE使服务器上的资源发生”状态转化”，就是所谓的表述性状态转移。
- WSGI是什么？
  - Web Server Gateway Interface Web服务器通用网关接口，是Python语言所定义的Web服务器和Web应用程序或框架之间的接口标准。
  - 从名称来看，WSGI是一个网关，作用就是协议之间进行转换，WSGI就是一座桥梁（服务器／网关端<-->WSGI<-->应用端／框架端）
  - WSGI将Web组件分为三类，Web服务器（WSGI Server） <--> Web中间件（WSGI Middleware）<--> Web应用程序（WSGI Application）
  - WSGI Server
    - WSGI Server接收HTTP请求，封装一系列环境变量（environ），按照WSGI接口标准调用注册的WSGI Application，最后将响应返回给客户端。
  - WSGI Application
    - WSGI Application是一个可被调用的Python对象，它接收2个参数，通常为environ和start_response。
    - 当处理一个WSGI请求时，服务端为应用端提供上下文信息（environ）和一个回调函数（start_response），当应用处理完请求后，使用服务端提供的回调函数返回相对应的处理结果。
    ```
    def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    return '<h1>Hello, web!</h1>'
    ```

- 相关主题
  - [参考 【OpenStack 存储剖析】 ](https://www.ibm.com/developerworks/cn/cloud/library/1402_chenhy_openstackstorage/)
  - [参考 【Cinder System Architecture】](https://docs.openstack.org/developer/cinder/devref/architecture.html)
  - 参考 【OpenStack设计与实现（第二版）】
  - 参考 【OpenStack系统架构设计实战】
