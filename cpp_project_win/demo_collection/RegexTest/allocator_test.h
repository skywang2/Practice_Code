#pragma once

#include <string>
#include <memory>
#include <iostream>

int TestAllocator1() {
	std::allocator<char> alloc;
	//int n{ 5 };
	char* p = alloc.allocate(1);

	auto q = p;
	//alloc.construct(q++);
	//alloc.construct(q++, 10, 'c');
	//alloc.construct(q++, "this is char[]");
	//alloc.construct(q++, "4444");
	//alloc.construct(q++, "55555");
	//alloc.construct(q++, "666666");

	//alloc.destroy(p);
	return 0;
}
