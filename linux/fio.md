- dd
  ```
  sudo dd if=/dev/zero of=~/tempfile bs=1M count=1024 conv=fdatasync,notrunc
  echo 3 | tee /proc/sys/vm/drop_caches
  sudo dd if=~/tempfile of=/dev/null bs=1M count=1024 
  ```
   
