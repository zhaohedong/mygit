#include <memory>

void func1(std::unique_ptr<int> p){
	*p = 10;
}

void func2(int* p) {
	*p = 10;
}

int main() {
	std::unique_ptr<int> pInt = std::make_unique<int>(3);
	// auto p2 = std::move(pInt);
	func2(pInt.get());
	func1(std::move(pInt));
}