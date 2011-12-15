#pragma once

#include <cmath>
#include "tree.h"
#include "static_bst.h"

extern "C"
{
    #include "van-emde-boas-layout/veb.h"
}

template<typename TreeT>
uint64_t calculate_inorder_child_index(const TreeT& in_tree, uint64_t in_node, ChildPos in_pos)
{
    size_t tree_levels(int(log(in_tree.nodes.size() + 1) / log(2)));
    uint64_t index_delta(1 << (tree_levels - 2));
    uint64_t target_index(in_tree.root_index);

    while (target_index != in_node && index_delta)
    {
        if (in_node < target_index)
        {
            target_index -= index_delta;
        }
        else
        {
            target_index += index_delta;
        }
        index_delta >>= 1;
    }

    // we found it
    if (index_delta > 0)
    {
        if (in_pos == LEFT)
        {
            return target_index - index_delta;
        }
        return target_index + index_delta;
    }

    return TreeT::NIL_INDEX;
}

template<typename TreeT>
uint64_t calculate_bfs_child_index(const TreeT& in_tree, uint64_t in_node, ChildPos in_pos)
{
    uint64_t index(0);
    if (in_pos == LEFT)
    {
        index = (in_node + 1) * 2 - 1;
    }
    else
    {
        index = (in_node + 1) * 2;
    }

    if (index >= in_tree.nodes.size())
    {
        index = TreeT::NIL_INDEX;
    }
    return index;
}

template<typename TreeT>
uint64_t calculate_veb_child_index(const TreeT& in_tree, uint64_t in_node, ChildPos in_pos)
{
    Veb veb_layout(vebnew(in_tree.nodes.size()));
    uint64_t bfs_index(vebsop(veb_layout, in_node));
    uint64_t child_index(calculate_bfs_child_index(in_tree, bfs_index, in_pos));

    uint64_t result_child_index(child_index);
    if (child_index != TreeT::NIL_INDEX)
    {
        result_child_index = vebpos(veb_layout, child_index);
    }

    vebfree(veb_layout);

    return result_child_index;
}

template<typename TreeT>
uint64_t get_child(const TreeT& in_tree, uint64_t in_node, ChildPos in_pos)
{
    switch(in_tree.layout_type)
    {
    case INORDER:
        return calculate_inorder_child_index(in_tree, in_node, in_pos);
    case BFS:
        return calculate_bfs_child_index(in_tree, in_node, in_pos);
    case VEB:
        return calculate_veb_child_index(in_tree, in_node, in_pos);
    default:
        throw std::runtime_error("Invalid layout type specified");
    }
}

template<>
uint64_t get_child<explicit_tree_t>(const explicit_tree_t& in_tree, uint64_t in_node, ChildPos in_pos)
{
    if (in_pos ==LEFT)
    {
        return in_tree.nodes[in_node].left_child;
    }

    return in_tree.nodes[in_node].right_child;
}

template <typename TreeT>
uint64_t search(const TreeT& in_tree, uint64_t in_key)
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
