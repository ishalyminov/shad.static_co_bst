#include <iostream>
#include <cstdlib>

#include "tree.h"
#include "static_bst.h"
#include "tree_search.h"

extern "C"
{
    #include "van-emde-boas-layout/veb.h"
}

void check_tree_node_size()
{
    // may be false with some values of S due to alignment
    STATIC_ASSERT(sizeof(implicit_node_t) == 8 + S);
    STATIC_ASSERT(sizeof(explicit_node_t) == 24 + S);
}

void check_things()
{
    size_t
        tree_h(20),
        nodes_number((1 << tree_h) - 1);
    Veb veb_layout(vebnew(nodes_number));

    for (size_t i = 1; i <= nodes_number; ++i)
    {
        std::cout << "BFS " << i << " :: vEB " << vebpos(veb_layout, i) << '\n';
    }

    vebfree(veb_layout);
}

int main()
{
    // for randomized tree contents generation
    srand(239);

    check_things();

    std::vector<uint64_t> tree_data(15);
    for (size_t index = 0; index < tree_data.size(); ++index)
    {
        tree_data[index] = rand();
    }

    StaticBst bst(tree_data);
    std::cout << "Created successfully\n";

    explicit_tree_t inorder_layout_tree;
    make_bfs_layout(bst, &inorder_layout_tree);
    std::cout << tree_data[1] << ": " << search(inorder_layout_tree, tree_data[1]) << '\n';
}
