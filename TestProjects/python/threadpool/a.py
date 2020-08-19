import thread  
from time import sleep, ctime  
  
def loop0():  
    print 'Start loop 0 at:', ctime()  
    sleep(4)  
    print 'Loop 0 done at:', ctime()  
  
def loop1():  
    print 'Start loop 1 at:', ctime()  
    sleep(2)  
    print 'Loop 1 done at:', ctime()  
  
def main():  
    try:  
        print 'Starting at:', ctime()  
        thread.start_new_thread(loop0, ())  
        thread.start_new_thread(loop1, ())  
        sleep(6)  
        print 'All done at:', ctime()  
    except Exception,e:        
        print 'Error:',e    
    finally:      
        print 'END\n'    
  
if __name__ == '__main__':  
    main()

