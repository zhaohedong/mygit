- zsh后卡顿
    - git config --global oh-my-zsh.hide-status 1
- links
	- git
		- https://docs.google.com/presentation/d/1IQCRPHEIX-qKo7QFxsD3V62yhyGA9_5YsYXFOiBpgkk/edit#slide=id.g4e79868052_229_0
	- gerrit
		- https://docs.google.com/presentation/d/1C73UgQdzZDw0gzpaEqIC6SPujZJhqamyqO1XOHjH-uk/edit#slide=id.g4d6c16487b_1_767
- git use flow 
  ```
    git add file.name
    git commit -m "log contents"
    git push
  ```
- git reset 
	 - --soft branch(y) index(n) working tree(n)
	 - --mix(defalut) branch(y) index(y) working tree(n)
	 - --hard branch(y) index(y) working tree(y)
- git rebase
	- 合并 commit 记录，保持分支整洁
	- 相比 merge 来说会减少分支合并的记录
	- 只要你的分支上需要 rebase 的所有 commits 历史还没有被 push 过，就可以安全地使用 git-rebase来操作(最好个人使用)
- 初始化仓库
	- git init
	- git add .
	- git commit -m "first commit"	
- 创建本地分支
	- git checkout -b branchname
- git 取消某次提交
	- git reset --hard HEAD^
- git 冲突解决A
	- git apply –reject patch
	- git status查看有哪些.rej文件，和新规文件
	- 在.rej文件里找见冲突的diff段，手动修改对应的代码
	- git add related_files
	- git am –resolved
- git speed up
	```
	git config --global http.https://github.com.proxy socks5://127.0.0.1:1086
	git config --global https.https://github.com.proxy socks5://127.0.0.1:1086

	git config --global --unset http.proxy
	git config --global --unset https.proxy
	git config -l
	```

- create remote branch
```
git clone
git checkout -b my-test  //在当前分支下创建my-test的本地分支分支
git push origin my-test  //将my-test分支推送到远程
git branch --set-upstream-to=origin/my-test //将本地分支my-test关联到远程分支my-test上   
git branch -vv //查看远程分支 
```
- three steps to create a remote branch and track it
  - git checkout -b [本地分支名] [远程名]/[远程分支名]
  - git push origin [本地分支名] 
  - git branch --set-upstream-to=[远程名]/[远程分支名] [本地分支名]
  - git branch -vv 
- create local branch
  - git checkout -b localbranchname origin/serverfix
- push local branch to remote
  - git push origin localbranchname
  - git push (远程仓库名) (分支名)
  - git push [远程名] [本地分支]:[远程分支]
  - git push origin localbranch:remotebranch
- track remote branch
  - git checkout --track origin/serverfix
- merge remote bugfix branch to master branch
  - git checkout master
  - git merge bugfix
  - git push
- git checkout 
  - git checkout -b [分支名] [远程名]/[分支名]
  - git checkout -b localbranchname origin/serverfix
- delete remote branch
  - git push [远程名] [空]:[远程分支]
- git fetch vs git pull
  - git pull = git fetch + git merge
- git set track branch
  - git branch --set-upstream-to=[远程名]/[远程分支名] [本地分支名]
- git show track
  - git branch -vv
- git generate patch
  - git format-patch -{num}
  - git format-patch {commit}
  - git format-patch <r1>..<r2>   #生成两个commit间的修改的patch（包含两个commit. <r1>和<r2>都是具体的commit号)
  - git format-patch -1 <r1>      #生成单个commit的patch
  - git format-patch <r1>      #生成某commit以来的修改patch（不包含该commit）
  - git format-patch -n 使用 [PATCH n/m]，即使只有一个补丁
  - git format-patch -s 使用签名
- git apply patch
  - git apply --stat filename.patch
  - git apply --check filename.patch
  - git apply filename.patch
- git switch remote repo
  - git remote set-url origin ​new_remote_repository_address
  - git remote set-url origin http://10.4.47.98/montage-tech.com/dsc.git
- git move remote repo
  - git clone --mirror old.git 
  - cd old.git
  - git remote set-url --push origin  new.git 
  - git push --mirror 
- 查看最近一次变更内容
  - git show
- 查看最近n次变更
  - git log -p -n
- git tag
  - git tag -a release_for_v2.08 -m 'V2.06-montage-source-20190628'
  - git tag -a release_for_v2.08_new -m 'new V2.08-montage-source-20190628'
  - git push origin --tags
- git 回滚某次提交
  - git revert commitid
- log 修改
  - git commit --amend
- git patch相关
  - git 为某次commit生成patch
    - git format-patch -1 指定commit号
  - git 为最近几次commit生成patch
    - git format-patch -n
  - 合并多个commit成为一个patch
    - git diff commitID1 commitID2 > ./patch
  - 某两次提交间的所有patch
    - git format-patch commitID1 commitID2
  - 检查patch
    - git apply --stat xxx.patch
  - 检查patch是否能应用
    - git apply --check xxx.patch
  - 打patch
    - git apply xxx.patch
    - git am --signoff < xxx.patch 
  - 冲突解决
    - git apply --reject xxx.patch merge没有冲突的部分
    - 冲突部分保存在.rej文件中，手动解决
    - 删除.rej，并执行git add .添加修改
    - git am --resolved最后push上去
  - 解决本地版本领先于master
    - git reset --hard origin/master
- svn 
  - svn checkout
    - svn checkout svn://192.168.0.1/runoob01 --username=user01
  - svn 提交
    - svn add file
    - svn commit -m "log"
- 复制仓库
  - 创建仓库的裸克隆
    - git clone --bare https://github.com/exampleuser/old-repository.git
  - 镜像推送至新仓库
    - cd old-repository.git
    - git push --mirror https://github.com/exampleuser/new-repository.git
  - 删除步骤 1 中创建的临时本地仓库
    - cd ..
    - rm -rf old-repository.git
  - ref
    - https://help.github.com/cn/github/creating-cloning-and-archiving-repositories/duplicating-a-repository
- ssh 反向穿透内网
  - ref
    - https://cherrot.com/tech/2017/01/08/ssh-tunneling-practice.html
    - https://www.jianshu.com/p/b1cc3b5aa00d
  - 公网机器
    - 端口开放
      - 22
      - 2222
    - 公网IP
      - 13.231.121.201 
    - 公网user
      - ec2-user
    - 确保公网2222端口没有被占用
      - lsof -t -i:2222
      - kill $(lsof -t -i:2222)
  - 内网
    - 内网操作
      - ssh -qTfNn -R '[::]:2222:localhost:22' ec2-user@13.231.121.201 -vvv
    - 内网user
      - u
  - 其他网络
    - ssh -p 2222 u@13.231.121.201
- ssh 穿越跳板机
	```
	Host tMac 
	    Hostname 192.168.50.17
	    IdentityFile ~/.ssh/id_rsa
	    User u
	    Port 22
	    ForwardAgent yes
	    ProxyCommand ssh -p 7777 admin@zhaohedong.asuscomm.com -W %h:%p 2> /dev/null
	```
- diff打patch
	- 以linux-kernel的tools/build/Build.include文件为例
	- 准备两个文件夹a & b
	- diff 
- settings
	```
	credential.helper=osxkeychain
	user.email=zhaohedong@gmail.com
	user.name=zhaohedong
	filter.lfs.clean=git-lfs clean -- %f
	filter.lfs.smudge=git-lfs smudge -- %f
	filter.lfs.process=git-lfs filter-process
	filter.lfs.required=true
	http.https://github.com.proxy=socks5://127.0.0.1:1086
	https.https://github.com.proxy=socks5://127.0.0.1:1086
	oh-my-zsh.hide-status=1
	```

  

- make
- make warning
  - $(warning "abc")
- PHONY 作用
  - 通常来说，makefile的target是一个真实的文件，但是有些target也不是文件，比如clean，这样的不是真实文件的target可以用PHONY来管理
- sign 
  - kernel/scripts/sign-file sha512 signing_key.priv sg
  
