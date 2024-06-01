#include <iostream>
#include <math.h>

int main()
{
    double result;
    // Use variables to avoid compiler optimization
    auto x = 8.0;
    auto y = 2.0;
    result = pow(x, y);
    std::cout << "pow(8.0, 2.0) = " << result << std::endl;

    auto e = 2.718282;
    result = log(e);
    std::cout << "log(e) = " << result << std::endl;
}
