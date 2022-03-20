#include <string>
#include <iostream>

class Container {
 private:
  typedef std::string Resource;

 public:
  Container() {
    resource_ = new Resource;
    std::cout << "default constructor." << std::endl;
  }
  explicit Container(const Resource& resource) {
    resource_ = new Resource(resource);
    std::cout << "explicit constructor." << std::endl;
  }
  ~Container() {
    delete resource_;
    std::cout << "destructor" << std::endl;
  }
  Container(const Container& rhs) {
    resource_ = new Resource(*(rhs.resource_));
    std::cout << "copy constructor." << std::endl;
  }
  Container& operator=(const Container& rhs) {
    delete resource_;
    resource_ = new Resource(*(rhs.resource_));
    std::cout << "copy assignment." << std::endl;
    return *this;
  }
  Container(Container&& rhs) : resource_(rhs.resource_) {
    rhs.resource_ = nullptr;
    std::cout << "move constructor." << std::endl;
  }
  Container& operator=(Container&& rhs) {
    Resource* tmp = resource_; resource_ = rhs.resource_; rhs.resource_ = tmp;
    std::cout << "move assignment." << std::endl;
    return *this;
  }

 private:
  Resource* resource_ = nullptr;
};

Container get() {
  Container ret("tag");
  return ret;
}

int main() {
  Container foo;
  // ...
  foo = get();
  return 0;
}