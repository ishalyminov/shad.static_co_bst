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
void search_queries(const TreeT& in_tree, size_t in_queries_number)
{
    for (size_t index = 0; index < in_queries_number; ++index)
    {
        search(in_tree, rand());
    }
}

void perform_experiment(const std::string& in_layout_name,
                        const std::string& in_indexing_type,
                        size_t in_tree_levels, int in_queries_num)
{
    std::vector<uint64_t> tree_data((1 << in_tree_levels) - 1);
    size_t queries_number(in_queries_num != -1 ? in_queries_num : tree_data.size());

    for (size_t index = 0; index < tree_data.size(); ++index)
    {
        tree_data[index] = rand();
    }

    StaticBst bst(tree_data);
    std::cout << "BST created successfully\n";

    double time(0.);
    if (in_indexing_type == "implicit")
    {
        implicit_tree_t custom_layout_tree;
        create_layout(bst, &custom_layout_tree, in_layout_name);

        time_t
            begin,
            end;
        begin = clock();
        search_queries(custom_layout_tree, queries_number);
        end = clock();
        time = (end - begin) / (double) CLOCKS_PER_SEC;
    }
    else if (in_indexing_type == "explicit")
    {
        explicit_tree_t custom_layout_tree;
        create_layout(bst, &custom_layout_tree, in_layout_name);

        time_t
            begin,
            end;
        begin = clock();
        search_queries(custom_layout_tree, queries_number);
        end = clock();
        time = (end - begin) / (double) CLOCKS_PER_SEC;
    }
    else
    {
        std::cout << "Wrong indexing type\n";
        return;
    }

    std::cout << "RESULT: " << time << " seconds taken\n";
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
            "Usage: static_co_bst <layout_type: inorder | bfs | veb> <node indexing type> <BST levels number> [search queries_number: default = # tree nodes]";
        return 0;
    }

    int queries_num(-1);
    if (argc == 5)
    {
        queries_num = atol(argv[4]);
    }

    size_t levels_num(atol(argv[3]));

    // for randomized tree contents generation
    srand(239);

    perform_experiment(argv[1], argv[2], levels_num, queries_num);
}
