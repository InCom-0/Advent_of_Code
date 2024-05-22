#include <AOC_commons.h>


namespace LC_commons
{
    Node::Node(int _val)
    {
        val = _val;
        next = nullptr;
        random = nullptr;
    }

    Node::Node(int _val, Node *n1)
    {
        val = _val;
        next = n1;
        random = nullptr;
    }

}