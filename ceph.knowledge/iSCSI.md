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
cd iscsi-target
create iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw
cd iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/gateways
create tcmu-gw1 10.0.1.111 skipchecks=true
create tcmu-gw2 10.0.1.112 skipchecks=true
cd /iscsi-target/iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/hosts
create iqn.1994-05.com.redhat:rh7-client
auth chap=myiscsiusername/myiscsipassword
disk add rbd_pool.disk_1
create pool=rbd_pool image=disk_1 size=1T
```

- targetcli add disk:
```
cd /backstores/user:rbd
create name=my_ec_test size=50T cfgstring=rbd_pool/image2,max_data_area_mb=256 hw_max_sectors=8192 
cd /iscsi/iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/tpg1/luns
create /backstores/user:rbd/my_ec_test
```

- targetcli remove disk:
```
targetcli remove disk:
cd /backstores/user:rbd
delete my_ec_test
```
- gwcli reconfigure:
```
reconfigure image_id=rbd.disk_1 attribute=max_data_area_mb value=128
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

iscsiadm --mode node --logout 
systemctl restart iscsid
iscsiadm -m discovery -t sendtargets -p 10.0.1.111
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
    systemctl start rbd-target-gw
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
- Trouble:
    - poor performance
- Solution:
    - ensure max_data_area_mb >= 128 &&  hw_max_sectors>=4096
    - set hw_max_sectors in gwcli 
        - we should add code in ceph-iscsi-config/ceph_iscsi_config/lun.py 
        ```
        def add_dev_to_lio(self, in_wwn=None):
        """
        Add an rbd device to the LIO configuration
        :param in_wwn: optional wwn identifying the rbd image to clients
        (must match across gateways)
        :return: LIO LUN object
        """
        self.logger.info("(LUN.add_dev_to_lio) Adding image "
                         "'{}' to LIO".format(self.config_key))

        # extract control parameter overrides (if any) or use default
        controls = self.controls.copy()
        for k in ['max_data_area_mb']:
            if controls.get(k, None) is None:
                controls[k] = getattr(settings.config, k, None)

        control_string = gen_control_string(controls) 
        control_string += ",hw_max_sectors=8192 #add code 
        print "zhd-debug control string is %s." % control_string #add code
        ...
        ...
        ...
        ```
- Trouble:
    - tcmu_reset_netlink:146: Kernel does not support reseting netlink
    - tcmu_block_device:387 rbd/rbd.disk_4: Kernel does not support the block_dev action.
    - tcmu_rbd_check_excl_lock_enabled:757 rbd/rbd.disk_4: HA not supported.
    - tcmu_unblock_netlink:114: Kernel does not support unblocking netlink
- Solution:
    - none
- Trouble:
    - targetcli show "Could not create NetworkPortal in configFS"
- Solution:
    - delete 0.0.0.0 3260 in targetcli
- Trouble:
    - Failed to start Ceph iscsi target configuration API.
- Solution:
    - reboot PC
- Trouble:
    - iscsiadm: Could not logout of [sid: 111, target: iqn.2018-09.com.test:target1, portal: 10.0.1.111,3260]
- Solution:
    - iscsiadm -m node -T iqn.2018-09.com.test:target1 -p 10.0.1.111 --logout
    - iscsiadm -m node -T iqn.2018-09.com.test:target1 -p 10.0.1.111 -o delete
    - iscsiadm -m node --op delete --targetname iqn.2018-09.com.test:target1 
- Trouble:
    - Failed : disk create/update failed on tcmu-gw1. LUN allocation failure
    - Only image features RBD_FEATURE_LAYERING,RBD_FEATURE_EXCLUSIVE_LOCK,RBD_FEATURE_OBJECT_MAP,RBD_FEATURE_FAST_DIFF,RBD_FEATURE_DEEP_FLATTEN are supported
- Solution:
    - ```
         rbd -p rbd_pool feature enable image2 exclusive-lock
         rbd -p rbd_pool feature enable image2 object-maprbd -p rbd_pool feature enable image2 object-map
         rbd -p rbd_pool feature enable image2 fast-diff rbd -p rbd_pool feature enable image2 fast-diff 
         rbd -p rbd_pool feature enable image2 {deep-flatten} //cannot update immutable features
      ```
- Trouble: 
    - gwcli delete rbd_pool.imagex too slow
- Solution:
    - remove disk in gateway
    ```
    rados -p rbd get gateway.conf - > gateway.conf
    delete disk section manually
    rados -p rbd put gateway.conf gateway.conf
    ```
    - disk section example
    ```
        "disks": {
        "rbd_pool.image2": {
            "controls": {
                "max_data_area_mb": "1024"
            },
            "created": "2018/09/12 07:36:08",
            "image": "image2",
            "owner": "tcmu-gw1",
            "pool": "rbd_pool",
            "pool_id": 4,
            "updated": "2018/09/12 07:36:08",
            "wwn": "6a36e98c-7818-4681-9fde-be4866a74795"
        }
    ```
- Test Enviroment
    ```
    // ??ec rbd pool
    ceph osd pool create rbd 32 32 replicated
    ceph osd pool create rbd_pool 64 64 replicated
    rbd pool init

    // ??????host??????ruleset-failure-domain=host
    ceph osd erasure-code-profile set ec_rbd_pool_profile k=4 m=2 ruleset-failure-domain=host
    ceph osd erasure-code-profile set ec_rbd_pool_profile k=4 m=2 ruleset-failure-domain=osd

    // create ec pool
    ceph osd pool create ec_rbd_pool 128 128 erasure ec_rbd_pool_profile
    ceph osd pool set ec_rbd_pool allow_ec_overwrites true

    // create ec image using ec pool as data pool
    rbd create --size 10T --data-pool ec_rbd_pool rbd_pool/image2 --image-feature layering
    rbd create --size 10T --data-pool ec_rbd_pool rbd_pool/image1 --image-feature layering

    // delete EC pool
    ceph osd pool rm ec_rbd_pool ec_rbd_pool --yes-i-really-really-mean-it 
    ceph osd pool rm rbd_pool rbd_pool --yes-i-really-really-mean-it 

    // remove/get ec profile
    ceph osd erasure-code-profile rm ec_rbd_pool_profile 
    ceph osd erasure-code-profile get ec_rbd_pool_profile 

    // gwcli add disk:
    cd /disks
    create pool=rbd_pool image=image2 size=10T {max_data_area_mb=1024}
    cd /iscsi-target/iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/hosts/iqn.1994-05.com.redhat:rh7-client/
    disk add rbd_pool.image2

    reconfigure image_id=rbd_pool.image4 attribute=max_data_area_mb value=128


    // gwcli remove disk:
    cd /iscsi-target/iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/hosts/iqn.1994-05.com.redhat:rh7-client
    disk remove rbd_pool.image2
    cd /disks
    delete rbd_pool.image2

    // iscsi login
    iscsiadm --mode node --logout 
    systemctl restart iscsid
    iscsiadm -m discovery -t sendtargets -p 10.0.1.111
    iscsiadm -m node -T iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw -l

    //bare rbd
    rbd map rbd_pool/image1 --name client.admin -m vm-ubuntu1604-2 -k ceph.client.admin.keyring 
    ```
## Useful commands
- clear gateways
```
rados -p rbd get gateway.conf -
rados -p rbd put gateway.conf gateway.conf
    {
    "clients": {},
    "created": "2018/08/24 03:06:39",
    "disks": {},
    "epoch": 0,
    "gateways": {},
    "groups": {},
    "updated": "",
    "version": 3
    }
```

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
systemctl status tcmu-runner
systemctl status rbd-target-api
systemctl status rbd-target-gw
```

- service status
```
systemctl status tcmu-runner
systemctl status rbd-target-api
systemctl status rbd-target-gw
```
- curl exmaple:
```
curl --insecure --user admin:admin -X GET http://10.0.1.111:5000/api/gateways
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
cat /sys/kernel/config/target/iscsi/iqn.2003-01.com.redhat.iscsi-gw:iscsi-igw/tpgt_1/attrib/default_cmdsn_depth
iscsiadm --mode node --logout 
iscsiadm -m session -P3 
```

- build cross
```
apt-get build-dep --no-install-recommends binutils
apt-get source binutils
pushd binutils-2.*/
DEB_TARGET_ARCH=armhf TARGET=armhf dpkg-buildpackage -d -T control-stamp
dpkg-checkbuilddep

dpkg-buildpackage --target-arch armhf


