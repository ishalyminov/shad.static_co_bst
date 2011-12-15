#pragma once

extern "C"
{
    #include "van-emde-boas-layout/veb.h"
}

template<class TreeT>
void make_inorder_layout(const StaticBst& in_bst, TreeT* out_result)
{
    // pair.second == flag if left node is already scanned
    typedef std::pair<BstNode, bool> traverse_entry_t;

    out_result->nodes.reserve(in_bst.Size());
    std::stack<traverse_entry_t> remaining_subroots;
    const BstNode& current_node(in_bst.GetRootNode());
    remaining_subroots.push(std::make_pair(current_node, false));

    while (!remaining_subroots.empty())
    {
        traverse_entry_t top_node(remaining_subroots.top());

        if (!top_node.second && top_node.first.left_child != NIL_NODE)
        {
            remaining_subroots.top().second = true;
            remaining_subroots.push(std::make_pair(in_bst.GetNode(top_node.first.left_child), false));
        }
        else
        {
            typename TreeT::node_t new_node;
            new_node.key = top_node.first.key;
            link_children(&new_node, top_node.first.left_child, top_node.first.right_child);
            out_result->nodes.push_back(new_node);
            remaining_subroots.pop();

            if (top_node.first.right_child != NIL_NODE)
            {
                remaining_subroots.push(std::make_pair(in_bst.GetNode(top_node.first.right_child), false));
            }
        }
    }

    out_result->root_index = in_bst.GetRootIndex();
    out_result->layout_type = INORDER;
}

template<class TreeT>
void make_bfs_layout(const StaticBst& in_bst, TreeT* out_result)
{
    out_result->nodes.reserve(in_bst.Size());
    std::queue<uint64_t> nodes_to_visit;

    nodes_to_visit.push(in_bst.GetRootIndex());

    while (!nodes_to_visit.empty())
    {
        BstNode top_node(in_bst.GetNode(nodes_to_visit.front()));
        if (top_node.left_child != NIL_NODE)
        {
            nodes_to_visit.push(top_node.left_child);
        }

        if (top_node.right_child != NIL_NODE)
        {
            nodes_to_visit.push(top_node.right_child);
        }

        typename TreeT::node_t new_node;
        size_t new_node_index(out_result->nodes.size());
        new_node.key = top_node.key;
        uint64_t
            left_child = (new_node_index + 1) * 2 - 1,
            right_child = (new_node_index + 1) * 2;
        if (left_child >= in_bst.Size())
        {
            left_child = NIL_NODE;
        }
        if (right_child >= in_bst.Size())
        {
            right_child = NIL_NODE;
        }

        link_children(&new_node, left_child, right_child);

        out_result->nodes.push_back(new_node);

        nodes_to_visit.pop();
    }

    out_result->root_index = 0;
    out_result->layout_type = BFS;
}

template<typename TreeT>
void make_veb_layout(const StaticBst& in_bst, TreeT* out_result)
{
    out_result->nodes.resize(in_bst.Size());
    Veb veb_layout(vebnew(in_bst.Size()));
    TreeT bfs_tree;
    make_bfs_layout(in_bst, &bfs_tree);

    for (size_t bfs_node_index = 0; bfs_node_index < bfs_tree.nodes.size(); ++bfs_node_index)
    {
        const typename TreeT::node_t& bfs_node(bfs_tree.nodes[bfs_node_index]);
        typename TreeT::node_t new_node;
        new_node.key = bfs_node.key;

        uint64_t
            left_bfs_child(calculate_bfs_child_index(bfs_tree, bfs_node_index, LEFT)),
            right_bfs_child(calculate_bfs_child_index(bfs_tree, bfs_node_index, RIGHT));

        uint64_t
            left_child(vebpos(veb_layout, left_bfs_child)),
            right_child(vebpos(veb_layout, right_bfs_child));

        link_children(&new_node, left_child, right_child);

        size_t veb_index(vebpos(veb_layout, bfs_node_index));
        out_result->nodes[veb_index] = new_node;
    }

    out_result->root_index = vebpos(veb_layout, 0);
    out_result->layout_type = VEB;
}
