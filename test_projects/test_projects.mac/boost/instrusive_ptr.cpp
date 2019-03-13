// instrusive_ptr.cpp : 定义控制台应用程序的入口点。  
//  
  
#include <boost/intrusive_ptr.hpp>            // 主要介绍的对象——智能指针intrusive_ptr头文件  
#include <boost/detail/atomic_count.hpp>      // Boost库提供的线程安全的原子计数器  
#include <boost/checked_delete.hpp>           // 对指针进行静态检查的函数  
#include <iostream>  
#include <cassert>  
  
  
// 使用intrusive_ptr智能指针需要指向的类继承自intrusive_ptr_base，  
// 以便于自动实现引用计数。  
template<class T>  
class intrusive_ptr_base  
{  
private:  
    // 引用计数——初始化为0  
    // mutable：为了突破引用计数的获取函数intrusive_ptr_add_ref，intrusive_ptr_release  
    // 类型atomic_count保证了线程安全。  
    mutable boost::detail::atomic_count ref_count;  
public:  
    // 缺省构造函数  
    intrusive_ptr_base() : ref_count(0)  
    {  
        // 下面代码只是为了演示  
        std::cout << " Default constructor " << std::endl;  
    }  
    // 复制构造函数，啥都不实现，也就是不允许此操作  
    // 实现此函数无功能的目的是为了子类进行复制构造函数时不复制引用计数ref_count  
    intrusive_ptr_base(intrusive_ptr_base<T> const& rhs) : ref_count(0)  
    {  
        // 下面代码只是为了演示  
        std::cout << " Copy constructor... " << std::endl;  
    }  
    // 赋值操作，啥都不实现，也就是不允许此操作  
    // 实现此函数无功能的目的是为了子类进行复制构造函数时不复制引用计数ref_count  
    intrusive_ptr_base& operator=(intrusive_ptr_base<T> const& rhs)  
    {  
        // 下面代码只是为了演示  
        std::cout << " Assignment operator... " << std::endl;  
        return *this;  
    }  
    // 递增引用计数  
    // 用友元函数的方式来实现，避免了函数指针类型的转换  
    // 例如：  
    // template<typename T> void intrusive_ptr_add_ref(T* t)  
    // {  
    //     T->add_ref(); // 此处需要定义T的原型  
    // }  
    friend void intrusive_ptr_add_ref(intrusive_ptr_base<T> const* s)  
    {  
        // 下面代码只是为了演示  
        std::cout << " intrusive_ptr_add_ref... " << std::endl;  
        // 判断合法性并自增  
        assert(s != 0);  
        assert(s->ref_count >= 0);  
        ++s->ref_count;  
    }  
    // 递增引用计数  
    // 用友元函数的方式来实现，避免了函数指针类型的转换  
    // 例如：  
    // template<typename T> void intrusive_ptr_release(T* t)  
    // {  
    //     T->release(); // 此处需要定义T的原型  
    // }  
    friend void intrusive_ptr_release(intrusive_ptr_base<T> const* s)  
    {  
        // 下面代码只是为了演示  
        std::cout << " intrusive_ptr_release... " << std::endl;  
        // 判断合法性并自减  
        assert(s != 0);  
        assert(s->ref_count >= 0);  
        if (--s->ref_count == 0)  
        {  
            // 进行静态类型检查，防止出错  
            boost::checked_delete(static_cast<T const*>(s));  
        }  
    }  
    // this指针 可改变操作  
    boost::intrusive_ptr<T> self()  
    {  
        return boost::intrusive_ptr<T>((T*)this);  
    }  
    // this指针 不可改变操作  
    boost::intrusive_ptr<T> self() const  
    {  
        return boost::intrusive_ptr<const T>((T const*)this);  
    }  
    // 获取引用计数  
    int refcount() const  
    {  
        return ref_count;  
    }  
};  
  
// 子类，继承自intrusive_ptr_base  
// 自动获得引用计数功能  
class Connection : public intrusive_ptr_base<Connection>  
{  
private:    // 数据定义  
    int connection_id;  
    std::string connection_tag;  
public:  
    // 构造函数  
    Connection(int id, std::string tag)  
        : connection_id(id), connection_tag(tag) {}  
    // 复制构造函数，因为父类实现了无操作函数，导致引用计数不被复制  
    Connection(const Connection& rhs)  
        : connection_id(rhs.connection_id),  
        connection_tag(rhs.connection_tag) {}  
    // 赋值操作函数，因为父类实现了无操作函数，导致引用计数不被复制  
    const Connection operator=(const Connection& rhs)  
    {  
        if (this != &rhs)  
        {  
            connection_id = rhs.connection_id;  
            connection_tag = rhs.connection_tag;  
        }  
        return *this;  
    }  
};  
  
  
int main()  
{  
    // 添加这个大括号是为了能够在getchar()函数执行之前就获取到Connect类三个  
    // 智能指针销毁的操作输出字符。  
    {  
        std::cout << "Create an intrusive ptr" << std::endl;  
        boost::intrusive_ptr<Connection> con0(new Connection(4, "sss"));  
        std::cout << "Create an intrusive ptr. Refcount = " << con0->refcount() << std::endl;  
  
        // 复制构造函数的调用  
        boost::intrusive_ptr<Connection> con1(con0);  
        std::cout << "Create an intrusive ptr. Refcount = " << con1->refcount() << std::endl;  
        // 赋值函数的调用  
        boost::intrusive_ptr<Connection> con2 = con0;  
        std::cout << "Create an intrusive ptr. Refcount = " << con2->refcount() << std::endl;  
  
        std::cout << "Destroy an intrusive ptr" << std::endl;  
  
    }  
  
    getchar();  
  
    return 0;  
}  
