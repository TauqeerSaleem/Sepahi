# Sepahi
Sepahi (read C-py) is a C++ with pythonic syntax, made for helping learners to move from python to C++.

1. Make sure sepahi.sh is executable by running chmod +x sepahi.sh
2. To just create the output (.out) file do: /.sepahi.sh code.cpy 
2. To create and run the output file (.out) do: /.sepahi.sh code.cpy -m
2. To create a cpp file, create and run an output file (.out) file do: /.sepahi.sh code.cpy  -m -d

## range()
Sepahi comes with range() which basically functions like its counterpart in python. Example usage:
for const auto& x : range(10)
    cout << x << endl


## Future additions:
1. Instead of writing ./sepahi.sh [COMMAND], do sepahi [COMMAND], making it easier for the user to use the psudo-compiler. This is not very difficult, all we have to do is make the file executable, and then move it to bin OR add a custom folder to $PATH
