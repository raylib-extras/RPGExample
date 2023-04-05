//
// Created by Haohan Yang on 2023-04-05.
//

#ifndef EXTRA_H
#define EXTRA_H
#include "raylib.h"
#include <string>
struct Exit
{
    Rectangle Bounds;
    std::string Destination;
};

struct Chest
{
    Rectangle Bounds;
    std::string Contents;
    bool Opened = false;
};

struct MobInstance
{
    int MobId = -1;
    Vector2 Position;
    int Health;
    int SpriteId;

    bool Triggered = false;
    float LastAttack = -100;
};

#endif //EXTRA_H
