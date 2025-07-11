#include <vector>

// range(start, stop, step)
std::vector<int> range(int start, int stop, int step) {
    std::vector<int> result;
    if (step == 0) return result; // avoid infinite loop

    if (step > 0) {
        for (int i = start; i < stop; i += step)
            result.push_back(i);
    } else {
        for (int i = start; i > stop; i += step)
            result.push_back(i);
    }

    return result;
}

// range(stop)
std::vector<int> range(int stop) {
    return range(0, stop, 1);  // delegate to the full version
}

// range(start, stop)
std::vector<int> range(int start, int stop) {
    return range(start, stop, 1); // delegate to the full version
}