#pragma once

// Factory should not be based on name
#include <iostream>

template <typename T>
struct RegisteredDrawable {
    struct Registrator {
        Registrator() {
            std::cout << "Registering: " << T::name() << std::endl;
        }
    };
};
