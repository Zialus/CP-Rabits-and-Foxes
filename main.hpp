#pragma once

#include <cstdio>

typedef enum { RABBIT, FOX, ROCK, EMPTY } ElementType;

struct Rock {

    Rock() = default;

    Rock(int x,int y) {
        pos_x = x;
        pos_y = y;
    }

    int pos_x;
    int pos_y;
};

struct Rabbit {

    Rabbit() = default;

    Rabbit(int a, int x, int y) {
        procAge = a;
        pos_x = x;
        pos_y = y;
    }

    int procAge;
    int pos_x;
    int pos_y;
};

struct Fox {

    Fox() = default;

    Fox(int a, int x, int y) {
        hungryAge = 0;
        procAge = a;
        pos_x = x;
        pos_y = y;
    }

    int hungryAge;
    int procAge;
    int pos_x;
    int pos_y;
};

struct MatrixElement{

    MatrixElement() = default;

    MatrixElement(ElementType type){
        element_type = type;
    }

    ElementType element_type;

    union {
        struct Rabbit rb;
        struct Fox fx;
        struct Rock rk;
    } elem;

    char getChar(){
        switch (this->element_type){
            case RABBIT: return 'R';
            case FOX: return 'F';
            case ROCK: return '*';
            case EMPTY: return ' ';
        }
    }
};

namespace std {
    template <>
    struct hash<Rabbit>
    {
        size_t operator()(Rabbit const & r) const noexcept
        {
            return (
                    ( (53 + std::hash<int>()(r.pos_y)) * 53 + std::hash<int>()(r.pos_x) )
                    * 53 + std::hash<int>()(r.procAge)
            );
        }
    };
}

namespace std {
    template <>
    struct hash<Fox>
    {
        size_t operator()(Fox const & f) const noexcept
        {
            return (
                    ( (53 + std::hash<int>()(f.pos_y)) * 53 + std::hash<int>()(f.pos_x) )
                    * 53 + std::hash<int>()(f.procAge)
            );
        }
    };
}

namespace std {
    template <>
    struct hash<Rock>
    {
        size_t operator()(Rock const & rk) const noexcept
        {
            return (
                    (3 + std::hash<int>()(rk.pos_y)) * 53 + std::hash<int>()(rk.pos_x)
            );
        }
    };
}