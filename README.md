# Sepahi
Sepahi (read C-py) is a front-end for C++ with pythonic syntax. The main idea of Sepahi is to help students move from python to C++ withot getting bogged down by the syntax of C++ and rather focus on the fundamentals of C++. Sepahi simplifies the syntax of C++ by allowing you to:

1. Write structured code without semicolons (;)
2. Use indentation instead of {} braces
3. Reduce keystrokes from boilerplate code like #include or int main()
4. Use Python-style control structures (if, for, else) without parentheses
5. Use a built-in range() function just like Python!

## A sample code in Sepahi
```
include vector
include string
include sample_h.cpp

main()
    for const auto& x : range(10)
        if x%2 == 0
            cout << "even: " << x << endl
        else
            cout << "odd : " << x << endl
    cout << "Done!" << endl
    cout << say_hello(2) << endl;
```

## How to use?
Using CLI
```
./sepahi sample.cpy        # Just compile
./sepahi sample.cpy -m     # Compile & run
./sepahi sample.cpy -d     # Save intermediate .cpp file
./sepahi sample.cpy -m -d  # Compile, run, and save .cpp
```
Note: Make sure sepahi.sh is executable by running chmod +x sepahi.sh

## Built in range() function
Defined in python_for_loop.cpp. Sepahi comes with range() which basically functions like its counterpart in python. Example usage:
```
for const auto& x : range(10)
    cout << x << endl
```

## How to use Sepahi globally?
All you have to do is make the file executable, and then move it to bin.
```
chmod +x sepahi.sh
sudo mv sepahi.sh /usr/local/bin/sepahi
```
Then use it from anywhere
`sepahi my_code.cpy -m`
