#include <memory>
#include <iostream>
#include <vector>

class Warper {
public:
    class Good // note: public inheritance
    //class Good: std::enable_shared_from_this<Good> // note: public inheritance
    {
    public:
        // std::shared_ptr<Good> getptr() {
        //     return shared_from_this();
        // }
        ~Good() {
            std::cout << "Good::~Good() called\n";
        }
    };

    void add() {
        auto gp1 = std::make_shared<Warper::Good>();
        Warper::good_vcs.push_back(gp1);
    }

    void Get() {
        auto gp2 = Warper::GetGood(); 
        auto gp3 = gp2;
    }

    static std::shared_ptr<Good> GetGood() {
        for(auto it : good_vcs) {
            if(it) {
                std::cout << "return it\n";
                return it;
            }
        }
        return nullptr;
    }
    static std::vector<std::shared_ptr<Warper::Good>> good_vcs;

    class Bad
    {
    public:
        std::shared_ptr<Bad> getptr() {
            return std::shared_ptr<Bad>(this);
        }
        ~Bad() { std::cout << "Bad::~Bad() called\n"; }
    };
    static std::vector<std::shared_ptr<Warper::Bad>> bad_vcs;
};

std::vector<std::shared_ptr<Warper::Good>> Warper::good_vcs;
std::vector<std::shared_ptr<Warper::Bad>> Warper::bad_vcs;




 
int main()
{
    auto warp1 = std::make_shared<Warper>();
    warp1->add();
    warp1->Get();

    // Good: the two shared_ptr's share the same object
    //std::shared_ptr<Good> gp1 = std::make_shared<Good>();
    //Warper::good_vcs.push_back(gp1);
    //std::shared_ptr<Good> gp2 = gp1->getptr();
    //std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';
 
    // Bad: shared_from_this is called without having std::shared_ptr owning the caller 
    // try {
    //     Good not_so_good;
    //     std::shared_ptr<Good> gp1 = not_so_good.getptr();
    // } catch(std::bad_weak_ptr& e) {
    //     // undefined behavior (until C++17) and std::bad_weak_ptr thrown (since C++17)
    //     std::cout << e.what() << '\n';    
    // }
 
    //Bad, each shared_ptr thinks it's the only owner of the object
    // std::shared_ptr<Warper::Bad> bp1 = std::make_shared<Warper::Bad>();
    // std::shared_ptr<Warper::Bad> bp2 = bp1->getptr();
    // std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
} // UB: double-delete of Bad