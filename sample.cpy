include <vector>
include <string>


main()
    for const auto& x : range(10)
        if x%2 == 0
            cout << "even: " << x << endl
        else
            cout << "odd : " << x << endl
    cout << "Done!" << endl