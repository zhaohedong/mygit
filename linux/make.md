##### 2018.05.09
- echo 命令可用单引号或双引号将文本字符圈起来。
- 要显示美元符号，你必须在它前面放置一个反斜线\
- 通过等号赋值给用户变量，在变量、等号和值之间不能出现空格。
- shell脚本中最有用的特性之一反引号(`) ,可以将shell命令的输出赋给变量。
- 管道最常用是后面接more命令分页
- 用美元符号和方括号($[ operation ])将数学表达式圈起来
- linux提供$?专属变量来保存上个执行命令的退出状态码
- bash shell中的if是测试上条命令的返回值是否为0（命令成功运行），如果为0则执行then后面的语句，这点与其他编程语言不同
- 
    ```
    if command
    then
        commands
    else
        commands
    fi

    if command
    then
        commands
    elif command2
    then
        more commands
    fi

    if [ codition ]
    then
        commands
    fi

    for var in list
    do 
        commands
    done

    case variable in
    pattern1 | pattern2) commands1::
    pattern3) commands2::
    *) default commands::
    esac

    for (( i=1; i <=10; i++ ))
    do
        echo "The next number is $i"
    done

    while test command
    do 
        other commands
    done

    until test command
    do 
        other commands
    done

   for (( a=1; a<=3; a++ ))
   do
        echo "Starting loop $a:"
        for(( b=1; b<=3; b++ ))
        do
            echo "  Inside loop: $b"
        done
    done

    for var1 in 1 2 3 4 5 6 7
    do
        if [ $var1 -eq 5 ]
        then
            break
        fi
    done

    for (( var1=1; var1<15; var1++ ))
    do
        if [ $var1 -gt 5] && [ $var1 -lt 10]
        then
            continue
        fi
        echo "Iteration number: $var1"
    done

    count=1
    cat file.txt | while read line
    do
        echo "Line $count: $line"
        count=$[ $count+1 ]
    done

    
    ```
- 使用标准的数学比较符号来表示字符串比较，而用文本代码表示数值比较
- 复合条件测试
    - [ condition1 ] && [ condition2 ]
    - [ condition1 ] || [ condition2 ]
- if-then的高级特性
    - 用于数学表达式的双圆括号
        - 不需要将双圆括号中的表达式里的大于号转义
    - 用于高级字符串处理功能的双方括号
        - [[ experssion ]]双方括号的expression使用了test命令中采用的标准字符串进行比较。
- IFS（internal field separator）内部字段分隔符，默认情况下bash shell会将下列字符当作字段分隔符
    - 空格
    - 制表符
    - 换行符
    - shell中可以重新对IFS赋值
- sed
    - 流编辑器
    - sed optihons script file
        - sed -f scrpt1 file
    - s/pattern/replacement/flags
    - flags
        - 数字，表明新文本将替换第几处模式匹配的地方
        - g，表明新文本将会替换所有文本出现的地方
        - p，表明原来行的内容要打印出来
        - w file， 将替换结果写到文件中
    - -n选项将禁止sed编辑器输出。但p替换标记会输出修改过的行，将二者配合使用则会只输出被substitute命令修改过的行
    - sed编辑器允许！作为分隔符
    - [address]commnd 使用地址
        - sed '2,3s/dog/cat/' data1
    - 使用文本模式过滤器
        - /pattern/command
    - 删除行
        - sed '2,3d' data1
        - sed '/number 1/d' data7
        - sed不会修改原始文件
    - 插入和附加文本
        - sed '[address]\command'
            - echo "Test Line 2" | sed 'i\Test Line 1'
    - 修改行
        - sedd '3c\This is a changed line of text.' data7
    - 转换命令
        - [addresss]/inchars/outchars/
        - sed 'y/123/789' data8
            - 17,28,39分别各自一一对应替换
    - 回顾打印
        - p 用来打印文本行
            - sed -n '3/{
            p
            s/line/test/p
            }' data7
            - 先查找包含3的行，利用p来打印行替换前的行内容，利用s来替换行内容，再p输出行内容
        - = 用来打印行号
            - sed '=' data1
        - l 用来列出行
            - 允许打印数据流中的文本和不可打印的ASCII字符
    - sed和文件一起工作
        - [address]w filename
            - sed '1,2w test' data7
                - 将数据流的前2行输出到文件
        - [address]r filename
            - sed '3r data12' data7
                - 将文件中的数据插入到数据流
    - 
    
- gawk
    - gawk options program file
    - $0代表整个问本行，$1代表文本行中第一个数据字段
    - gawk默认的字段分隔符是任意空白字符（例如空格和制表符）
    - “-F”可以指定分隔符
    - 要在命令行上的程序脚本中使用多条命令，只要在每条命令之间放个分号即可
        - echo "My name is Rich" | gawk '{$4="Christine"; print $0}'
    - 在处理数据前运行脚本
        - gawk 'BEGIN {print "Hello World!"}'
    - 在处理数据后运行脚本，END

- 正则表达式
    - 类型
        - BRE POSIX基本正则表达式引擎
        - ERE POSIX扩展正则表达式引擎
    - 纯文本
        - echo "This is test" | sed - n '/test/p'
        - echo "This is test" | gawk '/test/{print $0}'
    - 特殊字符
        - .*[]^${}+?()
    - 锚字符
        - 锁定行首尾
            - 脱字符(caret character, ^)
                - echo "The book store" | sed -n '/^book/p'
            - 美元符,锁定尾部
                - echo "This is a good book" | sed -n '/book$/p'
        - 组合锚点
            - sed '/^$d' data5 删除文本所有空白行
    - 点字符
        - 点字符用来匹配任意单字符
    - 字符组
        - 如果字符组中的某个字符出现在了数据流中，那它就匹配了该模式
        - sed -n '/[ch]at/p' data6
    - 排除字符组
        - 在字符组的开头加个脱字符
        - sed -n '/[^ch]at/p' data6
    - 使用区间
        - 单破折线符号用来在字符组中使用字符区间
        - sed -n '/[c-h]at/p' data6
        - sed -n '/[a-ch-m]at/p' data6
    - 特殊字符组
    - 星号
        - 在字符后面放置型号说明该字符将会在匹配模式的文本中出现0次或多次
        - echo "ieek" | sed -n '/ie*k/p'
        - 可以与.字符组合起来，匹配任意多个字符
    - 问号
        - ？表明前面的字符可以出现0次或1次。
        - echo "bet" | gawk '/be?t/{print $0}' 
        - echo "beet" | gawk '/b[ae]?t/{print $0}'
        - 如果字符组中的字符出现了0次或1次，匹配模式成立，如果2个都出现了，匹配失败
    - 加号
        - 加号表明前面字符可以出现1次或多次
    - 花括号
        - ERE中花括号允许你为可重复的正则表达式指定一个上限。这个特性可以微调允许字符或字符集在模式中具体出现多少次
        - echo "bet" | gawk --re-interval '/be{1}t/{print $0}'
        - echo "beat" | gawk --re-interval '/b[ae]{1,2}t/{print $0}'
    - 管道符号
        - 管道符号允许你在检查数据流时，用逻辑OR方式制定正则表达式引擎要用的两个或则多个模式。
        - echo "This cat is asleep" | gawk '/cat|dog/{print $0}'
    - 聚合表达式
        - 正则表达式的模式也可以用圆括号聚合起来。当你聚合正则表达式时，改组就会被当成标准字符。可以在组内使用特殊字符
        - echo "Sat" | gawk '/Sat(urday)?/{print $0}'

- date
    - date -d "2018-1-01 00:00:00" +%s
    - date -d "1969-1-01 00:00:00" +%s
    - date -d "2018-5-01 00:00:00" +%s
    - date -d "Wed May 30 15:21:35 CST 2018" +%s
  
- Makefile中打印信息
  - $(warning xxxxx) 或者 $(error xxxxx)
- 指定EXTRA_FLAGS
  - make extra_flags-=-O0 //删除O0优化
- gdb中可以查看宏值
  - make install T=x86_64-native-linux-gcc EXTRA_CFLAGS='-O0 -gdwarf-2 -g3' -j
  -  make CFLAGS='-g -O2 -w' CXXFLAGS='-g -O2 -w'
  -  make CFLAGS+='-g -O2 -w' CXXFLAGS+='-g -O2 -w'
- CFLAGS
  - CFLAGS=-I/usr/include -I/path/include
- LDFLAGS
  - LDFLAGS=-L/usr/lib -L/path/to/your/lib
  - 有时候LDFLAGS指定-L虽然能让链接器找到库进行链接，但是运行时链接器却找不到这个库，如果要让软件运行时库文件的路径也得到扩展，那么我们需要增加这两个库给"-Wl,R"：
    - LDFLAGS = -L/var/xxx/lib -L/opt/mysql/lib -Wl,R/var/xxx/lib -Wl,R/opt/mysql/lib
- LIBS
  - LIBS = -lpthread -liconv
- gcc 选项
  - ‘-S’
        只是编译不汇编，生成汇编代码
  - ‘-E'
        只进行预处理
- hello example
  ```
    # Define required macros here
    SHELL = /bin/sh
    OBJS =  main.o factorial.o hello.o
    CFLAG = -Wall -g
    CC = gcc
    INCLUDE =
    LIBS = -lm
    hello:${O:wBJ}
        ${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}
    clean:
        -rm -f *.o core *.core
    .cpp.o:
        ${CC} ${CFLAGS} ${INCLUDES} -c $<               
  ```