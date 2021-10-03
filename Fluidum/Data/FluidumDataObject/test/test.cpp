#include "../FDobject.h"

#include <memory>
//FD::VertexLogWrite l;
//FD::VertexLogRead r;
//
//std::vector<FD::Object::Vertex> v;
//FD::Object::Indices indices;
//





int main() {





	//FVK::VertexBufferKey<std::string> key("a");
	//l.setTargetKey(key);


	//for (auto i = 0; i < 600; i++) {
	//	FD::Object::Vertex t = { {0.0f + i,0.0f + i,0.0f + i},{0.0f,0.0f,0.0f,0.0f} };
	//	v.emplace_back(t);

	//	std::vector<uint32_t> c;
	//	c.emplace_back(i);

	//	l.add(c, &v);

	//	//if (i % 10 == 0) {
	//	//	std::vector<uint32_t> c2;
	//	//	c2.emplace_back(indices.size());

	//	//	indices.emplace_back(i);
	//	//	l.add(c2, &indices);
	//	//}

	//}

	//auto [ver, ind] = r.create(key,700);

	//for (auto& x : ver) {
	//	std::cout << x.pos.x << " ";
	//}
	//std::cout << "\n\n\n\n";

	//for (auto& x : ind) {
	//	std::cout << x << " ";
	//}



	FD::Commands command = nullptr;
	FVK::Commands c = nullptr;
	;
	auto func = [](FVK::Commands*) {
		std::cout << "a";
	};
	func(command.operator FVK::Commands*());
}