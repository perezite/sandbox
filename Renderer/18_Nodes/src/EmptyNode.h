#pragma once
#include "Node.h"

namespace sb {
	class EmptyNode : public Node<EmptyNode> { };
}
