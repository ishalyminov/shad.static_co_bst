#pragma once

#include <stdint.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <limits>
#include <cstring>

#include "static_assert.h"

const size_t S = 8;

enum TreeLayout
{
    INORDER,
    BFS,
    VEB,
};

template<typename NodeT>
struct tree_t
{
    typedef NodeT node_t;
    TreeLayout layout_type;
    static uint64_t NIL_INDEX;
    uint64_t root_index;
    std::vector<node_t> nodes;
};

template<typename NodeT>
uint64_t tree_t<NodeT>::NIL_INDEX = std::numeric_limits<uint64_t>::max();

struct implicit_node_t
{
    uint64_t key;
    char payload[S];
};

struct explicit_node_t
{
    uint64_t key;
    char payload[S];

    uint64_t left_child;
    uint64_t right_child;
};

typedef tree_t<implicit_node_t> implicit_tree_t;
typedef tree_t<explicit_node_t> explicit_tree_t;

template<typename NodeT>
void link_children(NodeT* out_result, uint64_t in_left, uint64_t in_right)
{
    out_result->left_child = in_left;
    out_result->right_child = in_right;
}

template<>
void link_children<implicit_node_t>(implicit_node_t* out_result,
                                    uint64_t in_left,
                                    uint64_t in_right) { }
