#include <iostream>
#include <vector>


std::vector<int> Merge(std::vector<int> vec1, std::vector<int> vec2) {
    std::vector<int> merged;
    int pointer_1 = 0;
    int pointer_2 = 0;

    while (pointer_1 < vec1.size() && pointer_2 < vec2.size()) {
        if (vec1[pointer_1] < vec2[pointer_2]) {
            merged.push_back(vec1[pointer_1]);
            pointer_1 += 1;
        }
        else if (vec1[pointer_1] > vec2[pointer_2]) {
            merged.push_back(vec2[pointer_2]);
            pointer_2 += 1;
        }
        else {
            merged.push_back(vec1[pointer_1]);
            merged.push_back(vec2[pointer_2]);
            pointer_1 += 1;
            pointer_2 += 1;
        }
    }

    // pading with left elems 
    if (pointer_1 >= vec1.size()) {
        for (int i = pointer_2; i < vec2.size(); i ++)
            merged.push_back(vec2[i]);
    }
    else {
        for (int i = pointer_1; i < vec1.size(); i ++)
        merged.push_back(vec1[i]);
    }

    return merged;
}


int binary_search(std::vector<int> source, int key, int elem_id) {
    if (source.size() == 1 && source[0] == key)
        return elem_id;
    else if (source.size() <= 1)
        return -1;

    else {
        int pivot_id = source.size() / 2; 

        if (key == source[pivot_id])
            return pivot_id + elem_id;

        else if (key > source[pivot_id]) {
            std::vector<int> right_part(source.begin() + pivot_id, source.end());
            elem_id += pivot_id;
            return binary_search(right_part, key, elem_id);
        }
        else {
            std::vector<int> left_part(source.begin(), source.begin() + pivot_id);
            return binary_search(left_part, key, elem_id);
        }
    }
}




int main () {
    std::vector<int> test_1;

    test_1.push_back(11);
    test_1.push_back(1);
    test_1.push_back(7);

    std::vector<int> test_2;

    test_2.push_back(1);
    test_2.push_back(18);
    test_2.push_back(9);

    std::sort(test_1.begin(), test_1.end());
    std::sort(test_2.begin(), test_2.end());

    std::vector<int> merged = Merge(test_1, test_2);

    for (int i = 0; i < merged.size(); i ++) {
        std::cout << merged[i] << " ";
    }

    std::cout << "\n";

    std::vector<int> test;
    test.push_back(1);
    test.push_back(2);
    test.push_back(3);

    std::cout << binary_search(test, 0, 0) << "\n";

    return 0;
}