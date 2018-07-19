#include "stable.h"
#include "more.h"

static int x = 100;

Simple::Simple()
{
    std::cout << "before main" << std::endl;
}

Simple::~Simple()
{
    std::cout << "after main" << std::endl;
}

void Simple::fun()
{
    x = 101;
}
