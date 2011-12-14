#include <iostream>
#include <cstdlib>

#include "tree.h"
#include "balanced_bst.h"
#include "tree_search.h"
#include "layout.h"

void check_tree_node_size()
{
    // may be false with some values of S due to alignment
    STATIC_ASSERT(sizeof(implicit_node_t) == 8 + S);
    STATIC_ASSERT(sizeof(explicit_node_t) == 24 + S);
}

int main()
{
    size_t h(15);
    for (size_t i = 1; i <= size_t((1 << h) - 1); ++i)
    {
        std::cout << "BFS " << i << " :: vEB " << bfs_to_veb(i, h) << '\n';
    }
    return 0;

    // for randomized tree contents generation
    srand(239);

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
