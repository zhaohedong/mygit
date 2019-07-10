- git use flow 
  ```
  git add file.name
  git commit -m "log contents"
  git push
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
- merge remote branch to current branch
  - switch to target branch
  - git merge origin/serverfix
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


- make
- make warning
  - $(warning "abc")
- PHONY 作用
  - 通常来说，makefile的target是一个真实的文件，但是有些target也不是文件，比如clean，这样的不是真实文件的target可以用PHONY来管理
- sign 
  - kernel/scripts/sign-file sha512 signing_key.priv sg
