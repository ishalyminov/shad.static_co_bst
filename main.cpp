#include <iostream>
#include <cstdlib>

#include "tree.h"
#include "static_bst.h"
#include "tree_search.h"
#include "layouts.h"

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
        tree_h(4),
        nodes_number((1 << tree_h) - 1);
    Veb veb_layout(vebnew(nodes_number));

    for (size_t i = 0; i < nodes_number; ++i)
    {
        std::cout << "BFS " << i << " :: vEB " << vebpos(veb_layout, i) << '\n';
    }

    vebfree(veb_layout);
}

template<typename TreeT>
void create_layout(const StaticBst& in_bst, TreeT* out_result, const std::string& in_layout_name)
{
    if (in_layout_name == "INORDER")
    {
        make_inorder_layout(in_bst, out_result);
        return;
    }
    else if (in_layout_name == "BFS")
    {
        make_bfs_layout(in_bst, out_result);
        return;
    }
    else if (in_layout_name == "VEB")
    {
        make_veb_layout(in_bst, out_result);
        return;
    }
    else
    {
        throw std::runtime_error("Wrong layout name");
    }
}

template<typename TreeT>
double search_queries(const TreeT& in_tree, const std::vector<uint64_t>& in_search_data)
{
    time_t
        begin,
        end;
    std::vector<uint64_t> search_requests(in_search_data);
    std::random_shuffle(search_requests.begin(), search_requests.end());

    begin = clock();
    for (size_t index = 0; index < search_requests.size(); ++index)
    {
        search(in_tree, search_requests[index]);
    }
    end = clock();

    return (end - begin) / static_cast<double>(CLOCKS_PER_SEC);
}

void perform_experiment(const std::string& in_layout_name,
                        const std::string& in_indexing_type,
                        size_t in_tree_levels)
{
    std::vector<uint64_t> tree_data((1 << in_tree_levels) - 1);

    for (size_t index = 0; index < tree_data.size(); ++index)
    {
        tree_data[index] = rand();
    }

    StaticBst bst(tree_data);
    std::cout << "BST created successfully\n";

    if (in_indexing_type == "implicit")
    {
        implicit_tree_t custom_layout_tree;
        create_layout(bst, &custom_layout_tree, in_layout_name);

        std::cout << "RESULT: " << search_queries(custom_layout_tree, tree_data) << " seconds taken\n";
    }
    else if (in_indexing_type == "explicit")
    {
        explicit_tree_t custom_layout_tree;
        create_layout(bst, &custom_layout_tree, in_layout_name);

        std::cout << "RESULT: " << search_queries(custom_layout_tree, tree_data) << " seconds taken\n";
    }
    else
    {
        std::cout << "Wrong indexing type\n";
        return;
    }
}

/*
 * The main routine.
 * ATTENTION: tree size must be 2^n - 1 (full BSTs) - with other configurations
 * index calculation will break
 */
int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout <<
            "Usage: static_co_bst <layout_type: inorder | bfs | veb> <node indexing type> <BST levels number>";
        return 0;
    }

    size_t levels_num(atol(argv[3]));

    // for randomized tree contents generation
    srand(239);

    perform_experiment(argv[1], argv[2], levels_num);
}
