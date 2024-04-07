#include <iostream>
#include <assert.h>
#include <vector>



void print_array(const int *const array, const int lo, const int hi) {
    for (int i = lo; i < hi; i ++)
        printf("%d ", array[i]);

    printf("\n");
}

void insert_sort(int *const array, const int size) {
    assert(array);

    // iterating through whole array
    for (int i = 1; i < size; i ++) {
        int key = array[i];

        int j = i - 1;

        // moving elements to free insert pos
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

void insert_sort_backwards(int *const array, const int size) {
    assert(array);

    // iterating through whole array
    for (int i = 1; i < size; i ++) {
        int key = array[i];

        int j = i - 1;
        while(j >= 0 && array[j] < key) {
            array[j + 1] = array[j];
            j = j - 1;
        }

        array[j + 1] = key;
    }
}

int linear_search(int *const array, const int size, const int val) {
    for (int i = 0; i < size; i ++) {
        if (array[i] == val)
            return i;
    }
    return -1;
}

int max_int(int a, int b) {
    return a > b ? a : b;
}

class binary {
    public:
        binary(int dec) : decimal(dec) {
            if (dec == 0)
                bin.push_back(0);

            while (dec > 0) {
                bin.push_back(dec % 2);
                dec /= 2;
            }
        }

        binary() {};

        void print_bin() {
            for (int i = bin.size() - 1; i >= 0; i --) {
                std::cout << bin[i] << " ";
            }
            std::cout << "\n";
        }

        int get_decimal() {
            return decimal;
        }

        int decimal_from_bin() {
            int dec = 0;
            for (int i = 0; i < bin.size(); i ++)
                dec += pow(2, i) * bin[i];

            return dec;
        };

        // binary num itself
        std::vector<int> bin;

        // its decimal representation
        int decimal;
};

binary operator+ (binary& bin1, binary& bin2) {
    binary result = binary();

    if (bin1.get_decimal() < bin2.get_decimal()) {
        int overflow = 0;
        for (int i = 0; i < bin1.bin.size(); i ++){
            int sum = bin1.bin[i] + bin2.bin[i] + overflow;
            result.bin.push_back(sum % 2);

            overflow = 0;
            if (sum >= 2)
                overflow = 1;
        }

        for (int i = bin1.bin.size(); i < bin2.bin.size(); i ++) {
            int sum = bin2.bin[i] + overflow;
            result.bin.push_back(sum % 2);

            overflow = 0;
            if (sum >= 2)
                overflow = 1;
        }

        if (overflow)
            result.bin.push_back(overflow);
    }
    // everything same but bin1 <-> bin2 change
    else {
        int overflow = 0;
        for (int i = 0; i < bin2.bin.size(); i ++){
            int sum = bin1.bin[i] + bin2.bin[i] + overflow;
            result.bin.push_back(sum % 2);

            overflow = 0;
            if (sum >= 2)
                overflow = 1;
        }

        for (int i = bin2.bin.size(); i < bin1.bin.size(); i ++) {
            int sum = bin1.bin[i] + overflow;
            result.bin.push_back(sum % 2);

            overflow = 0;
            if (sum >= 2)
                overflow = 1;
        }

        if (overflow)
            result.bin.push_back(overflow);
    }

    result.decimal = result.decimal_from_bin();

    return result;
};


int main() {
    binary bin1 = binary(-1);
    binary bin2 = binary(0.5);

    bin1.print_bin();
    bin2.print_bin();

    binary sum = bin1 + bin2;

    sum.print_bin();
    std::cout << "result: " << sum.get_decimal() << "\n";

    return 0;
}