#pragma once

#include <limits>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stack>
#include <deque>
#include <queue>

const uint64_t NIL_NODE = std::numeric_limits<uint64_t>::max();

enum ChildPos
{
    LEFT,
    RIGHT
};

struct BstNode
{
    BstNode()
        : left_child(NIL_NODE),
          right_child(NIL_NODE),
          key(0) { }
    explicit BstNode(uint64_t in_key)
        : left_child(NIL_NODE),
          right_child(NIL_NODE),
          key(in_key) { }

    uint64_t left_child;
    uint64_t right_child;
    uint64_t key;
};

// it is always balanced just due to the way it's built
class StaticBst
{
public:
    StaticBst(const std::vector<uint64_t>& in_src)
    {
        Build(in_src);
    }

public:
    const BstNode& GetRootNode() const
    {
        return m_Nodes[m_RootNode];
    }

    uint64_t GetRootIndex() const
    {
        return m_RootNode;
    }

    const BstNode& GetNode(uint64_t in_index) const
    {
        return m_Nodes[in_index];
    }

    const size_t Size() const
    {
        return m_Nodes.size();
    }

private:
    void Build(const std::vector<uint64_t>& in_src)
    {
        std::deque<uint64_t> source(in_src.begin(), in_src.end());
        std::sort(source.begin(), source.end());
        CreateFromSortedVector(source);
    }

    int64_t CreateRecursively(std::deque<uint64_t>& in_src,
                               int64_t in_start,
                               int64_t in_end)
    {
        if (in_start > in_end)
        {
            return NIL_NODE;
        }

        // same as (start+end)/2, avoids overflow
        int64_t mid(in_start + (in_end - in_start) / 2);
        int64_t
            left_child(CreateRecursively(in_src, in_start, mid - 1)),
            parent(AddNode(in_src.front()));

        SetChild(parent, left_child, LEFT);
        in_src.pop_front();
        SetChild(parent, CreateRecursively(in_src, mid + 1, in_end), RIGHT);
        return parent;
    }

    void CreateFromSortedVector(std::deque<uint64_t>& in_src)
    {
        m_RootNode = CreateRecursively(in_src, 0, in_src.size() - 1);
        if (m_RootNode == NIL_NODE)
        {
            throw std::runtime_error("BST Build failed");
        }
    }

    uint64_t AddNode(uint64_t in_key)
    {
        m_Nodes.push_back(BstNode(in_key));
        return m_Nodes.size() - 1;
    }

    void SetChild(uint64_t in_index, uint64_t in_child_index, ChildPos in_type)
    {
        switch(in_type)
        {
        case LEFT:
            m_Nodes[in_index].left_child = in_child_index;
            break;
        case RIGHT:
            m_Nodes[in_index].right_child = in_child_index;
            break;
        }
    }

private:
    uint64_t m_RootNode;
    std::vector<BstNode> m_Nodes;
};

void print_inorder(const StaticBst& in_bst)
{
    // pair.second == flag if left node is already scanned
    typedef std::pair<BstNode, bool> traverse_entry_t;
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
            std::cout << top_node.first.key << ' ';
            remaining_subroots.pop();

            if (top_node.first.right_child != NIL_NODE)
            {
                remaining_subroots.push(std::make_pair(in_bst.GetNode(top_node.first.right_child), false));
            }
        }
    }
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
}
