#pragma once

#include "tree.h"
#include "balanced_bst.h"

template <typename TreeT>
uint64_t get_child(const TreeT& in_tree, uint64_t in_node, ChildPos in_pos)
{
    return 4;
}

template <>
uint64_t get_child<explicit_tree_t>(const explicit_tree_t& in_tree, uint64_t in_node, ChildPos in_pos)
{
    if (in_pos ==LEFT)
    {
        return in_tree.nodes[in_node].left_child;
    }

    return in_tree.nodes[in_node].right_child;
}

template <typename TreeT>
uint64_t search(const TreeT& in_tree, const uint64_t in_key)
{
    uint64_t current_node(in_tree.root_index);

    bool in_leaf_node(false);

    while (!in_leaf_node)
    {
        if (current_node == TreeT::NIL_INDEX)
        {
            in_leaf_node = true;
            break;
        }
        uint64_t
            left_child(get_child(in_tree, current_node, LEFT)),
            right_child(get_child(in_tree, current_node, RIGHT));

        if (in_key == in_tree.nodes[current_node].key)
        {
            break;
        }
        else if (in_key < in_tree.nodes[current_node].key)
        {
            current_node = left_child;
        }
        else
        {
            current_node = right_child;
        }
    }

    return current_node;
}
