#include <iostream>
#include "python_for_loop.cpp"
using namespace std;

#include <vector>
#include <string>


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
}
