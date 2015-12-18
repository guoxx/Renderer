#pragma once

#include <vector>
#include <memory>
#include "RenderEnum.h"

class RenderCommand
{
public:
	RenderDrawMode mode;
	std::shared_ptr<std::vector<float>> indices;
};