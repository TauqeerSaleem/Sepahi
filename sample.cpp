#include <iostream>
#include "python_for_loop.cpp"
using namespace std;

#include <vector>
#include <string>
#include "sample_h.cpp"

int main()
{
for (const auto& x : range(10))
{
if (x%2 == 0)
{
            cout << "even: " << x << endl;
        }
        else
{
            cout << "odd : " << x << endl;
        }
    }
    cout << "Done!" << endl;
    cout << say_hello(2) << endl;
}
