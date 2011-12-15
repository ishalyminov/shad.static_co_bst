if [ $# -lt 1 ]; then 
    echo 'Usage: experiment.sh <path_to_binary>';
    exit
fi

for tree_levels in '2' '4' '8' '12' '16' '20' '24' ' 28' '32'; do
    echo Experiment with $tree_levels levels;
    $1 INORDER implicit $tree_levels;
done
