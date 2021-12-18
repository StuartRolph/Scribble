//
// Created by Stuart Rolph on 12/18/21.
//

#include <iostream>
#include "Image/Image.h"

int main() {
    Image ven("../data/venru.jpg");
    printf("%d %d %d %d\n", ven.w, ven.h, ven.channels, ven.size);

    return 0;
}