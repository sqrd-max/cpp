// Person.h
#ifndef PERSON_H
#define PERSON_H

#include <array>

enum class Gender { Male, Female };

struct Person {
    std::array<char, 50> name;
    int age;
    Gender gender;
};

#endif // PERSON_H
