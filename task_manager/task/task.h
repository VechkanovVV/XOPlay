#pragma once

#include "task_params.h"

class Task
{
   public:
    virtual ~Task() = default;
    virtual void execute() = 0;
};