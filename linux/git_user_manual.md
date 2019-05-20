- git use flow 
  ```
  git add file.name
  git commit -m "log contents"
  git push
  ```
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
  - git remote set-url origin http://10.4.47.98/montage-tech.com/qemu.git
- git move remote repo
  - git clone --mirror old.git 
  - cd old.git
  - git remote set-url --push origin  new.git 
  - git push --mirror 


- make
- make warning
  - $(warning "abc")
- PHONY 作用
  - 通常来说，makefile的target是一个真实的文件，但是有些target也不是文件，比如clean，这样的不是真实文件的target可以用PHONY来管理
- sign 
  - kernel/scripts/sign-file sha512 signing_key.priv sg
