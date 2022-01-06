#include "node.h"

FS::Flu::Node::Node()
{
	FluidumScene_Log_Constructor(::FS::Flu::Node);
}

FS::Flu::Node::~Node() {
	FluidumScene_Log_Destructor(::FS::Flu::Node);
}

void FS::Flu::Node::call() {

}
