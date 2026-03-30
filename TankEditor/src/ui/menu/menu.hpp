#pragma once


namespace Tank::Editor
{
    class Menu : public Node
    {
    public:
        Menu(const std::string &name) : Node(name) {}

        virtual void draw() = 0;
    };
}