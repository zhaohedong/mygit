## Preparation
- Pre-Requirements
    - http://docs.ceph.com/docs/master/rbd/iscsi-target-cli/
    - add all gateway host information in every gateway nodes in /etc/hosts

- epel installation
```
sudo yum install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
yum makecache
```

- ceph installation
```
setenforce 0
vi /etc/yum.repos.d/ceph.repo

[Ceph]
name=Ceph packages for $basearch
baseurl=http://download.ceph.com/rpm-luminous/el7/$basearch
enabled=1
gpgcheck=1
type=rpm-md
gpgkey=https://download.ceph.com/keys/release.asc

[Ceph-noarch]
name=Ceph noarch packages
baseurl=http://download.ceph.com/rpm-luminous/el7/noarch
enabled=1
gpgcheck=1
type=rpm-md
gpgkey=https://download.ceph.com/keys/release.asc

[ceph-source]
name=Ceph source packages
baseurl=http://download.ceph.com/rpm-luminous/el7/SRPMS
enabled=1
gpgcheck=1
type=rpm-md
gpgkey=https://download.ceph.com/keys/release.asc

yum install ceph
```

- Dependencies installation:
```
yum install git cmake gcc python-devel libnl3 librbd1 pyparsing pyOpenSSL python-gobject kmod-devel iptables-services python-pip
pip install pip --upgrade
pip install kmod pyudev urwid pyparsing rados rbd netaddr netifaces crypto requests flask pycrypto
```

- tcmu-runner:
```
git clone https://github.com/open-iscsi/tcmu-runner
cd tcmu-runner
./extra/install_dep.sh
cmake -Dwith-glfs=true -Dwith-qcow=false -DSUPPORT_SYSTEMD=ON -DCMAKE_INSTALL_PREFIX=/usr
make install
systemctl daemon-reload
systemctl enable tcmu-runner
systemctl start tcmu-runner
```

- rtslib-fb:
```
git clone https://github.com/open-iscsi/rtslib-fb.git
cd rtslib-fb
python setup.py install
```

- configshell-fb:
```
git clone https://github.com/open-iscsi/configshell-fb.git
cd configshell-fb
python setup.py install
```

- targetcli-fb:
```
git clone https://github.com/open-iscsi/targetcli-fb.git
cd targetcli-fb
python setup.py install
mkdir /etc/target
mkdir /var/target
```

- ceph-iscsi-config:
```
git clone https://github.com/ceph/ceph-iscsi-config.git
cd ceph-iscsi-config
python setup.py install --install-scripts=/usr/bin
cp usr/lib/systemd/system/rbd-target-gw.service /lib/systemd/system
systemctl daemon-reload
systemctl enable rbd-target-gw
systemctl start rbd-target-gw
```

- ceph-iscsi-cli:
```
git clone https://github.com/ceph/ceph-iscsi-cli.git
cd ceph-iscsi-cli
python setup.py install
cp usr/lib/systemd/system/rbd-target-api.service /lib/systemd/system
```

- vi /etc/ceph/iscsi-gateway.cfg
```
[config]
# Name of the Ceph storage cluster. A suitable Ceph configuration file allowing
# access to the Ceph storage cluster from the gateway node is required, if not
# colocated on an OSD node.
cluster_name = ceph

# Place a copy of the ceph cluster's admin keyring in the gateway's /etc/ceph
# drectory and reference the filename here
gateway_keyring = ceph.client.admin.keyring


# API settings.
# The API supports a number of options that allow you to tailor it to your
# local environment. If you want to run the API under https, you will need to
# create cert/key files that are compatible for each iSCSI gateway node, that is
# not locked to a specific node. SSL cert and key files *must* be called
# 'iscsi-gateway.crt' and 'iscsi-gateway.key' and placed in the '/etc/ceph/' directory
# on *each* gateway node. With the SSL files in place, you can use 'api_secure = true'
# to switch to https mode.

# To support the API, the bear minimum settings are:
api_secure = false

# Additional API configuration options are as follows, defaults shown.
# api_user = admin
# api_password = admin
# api_port = 5001
# change to your own IPs
trusted_ip_list = 10.0.1.201,10.0.1.203
```
## Build Target
```
scp ceph.conf,iscsi-gateway.cfg,ceph.client.admin.keyring in /etc/ceph/ from the first gateway to other gateways
create iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw
cd iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/gateways
create vm-gate1 10.0.1.201 skipchecks=true
create vm-tcmu2 10.0.1.203 skipchecks=true
cd /disks
create pool=rbd image=disk_1 size=90G
cd /iscsi-target/iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/hosts
create iqn.1994-05.com.redhat:rh7-client
auth chap=myiscsiusername/myiscsipassword
disk add rbd.disk_1 
```

## Build Initiator
```
yum install iscsi-initiator-utils
yum install device-mapper-multipath
mpathconf --enable --with_multipathd y

vi /etc/iscsi/initiatorname.iscsi
InitiatorName=iqn.1994-05.com.redhat:rh7-client

vi /etc/iscsi/iscsid.conf 
node.session.auth.authmethod = CHAP
node.session.auth.username = myiscsiusername
node.session.auth.password = myiscsipassword

iscsiadm -m discovery -t sendtargets -p 10.0.1.201
iscsiadm -m node -T iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw -l

```

## TroubleShooting 
- Trouble:
    - when excute gwcli, shell show "IndexError: list index out of range"
- Solution:
    ```
    yum install ceph 
    ```
- Trouble:
    - when excute gwcli, shell show "ImportError: No module named 'requests.packages.urllib3'"
- Solution:
    ```
    pip uninstall urllib3
    pip install --upgrade urllib3
    ```
- Trouble:
    - Unable to access the configuration object : REST API failure, code : 500 GatewayError: 
- Solution:
    ```
    create pool named rbd 
    systemctl daemon-reload
    systemctl enable rbd-target-api
    systemctl start rbd-target-api
    systemctl enable rbd-target-gw
    systemctl start rbd-target-gwsy
    stemctl start rbd-target-gw
    ```
- Trouble:
    - When we try to add second gateway, gwcli show " Gateway creation failed, gateway(s) unavailable:10.0.2.201(UNKNOWN state)"
- Solution: 
    - read https://github.com/ceph/ceph-iscsi-cli/issues/59
    - vi /etc/ceph/iscsi-gateway.cfg and add
    ``` 
    trusted_ip_list =10.0.1.201,10.0.1.203
    ```
    - vi /etc/hosts add
    ```
    10.0.1.201 vm-tcmu
    10.0.1.203 vm-tcmu2
    ```
    - stop firewallld, iptables
    ```
    systemctl stop firewalld.service
    systemctl stop iptables 
    ```
- Trouble:
    - iscsiadm -m discovery -t -st 10.0.1.201 can not Discover the target portals, and systemctl status iscsi.service returned "start condition failed at xxx,none of the trigger conditions were met"
- Solution
    ```
    Do not edit /etc/iscsi/iscsid.conf for CHAP, keep it No CHAP as default
    systemctl enable iscsid iscsi
    systemctl start iscsid iscsi
    ```
- Trouble:
    - when we execute "iscsiadm -m node -T iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw -l" to login node it returns "initiator reported error (24 - iSCSI login failed due to authorization failure)"
- Solution
    ```
    refer to https://github.com/ceph/ceph-iscsi-cli/issues/13
    vi /etc/iscsi/initiatorname.iscsi
    InitiatorName=iqn.1994-05.com.redhat:rh7-client

    vi /etc/iscsi/iscsid.conf 
    node.session.auth.authmethod = CHAP
    node.session.auth.username = myiscsiusername
    node.session.auth.password = myiscsipassword

    systemctl restart iscsid iscsi
    ```    
## Useful commands
- service start
```
systemctl daemon-reload
systemctl enable tcmu-runner
systemctl start tcmu-runner
systemctl daemon-reload
systemctl enable rbd-target-gw
systemctl start rbd-target-gw
systemctl daemon-reload
systemctl enable rbd-target-api
systemctl start rbd-target-api
```

- service restart
```
systemctl daemon-reload
systemctl restart tcmu-runner
systemctl restart rbd-target-api
systemctl restart rbd-target-gw
```

- service status
```
systemctl status tcmu-runner
systemctl status rbd-target-api
systemctl status rbd-target-gw
```
- curl exmaple:
```
curl --insecure --user admin:admin -X GET http://10.0.1.203:5000/api/gateways
curl --insecure --user admin:admin -d "ip_address=10.0.1.203" \
-X PUT https://10.0.1.201/api/gateway/vm-tcmu2
```

- systemctl commands
```
systemctl list-unit-files
systemctl list-dependencies iscsi.service
```

- scp example:
```
scp root@vm-tcmu:/etc/ceph/iscsi-gateway.cfg .
scp iscsi-gateway.cfg root@vm-tcmu2:/etc/ceph/
```

- iscsiadm
```
show node info:
iscsiadm -m node -T iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw
remove targets:
rm -rf /var/lib/iscsi/send_targets/10.0.1.201,3260/
```