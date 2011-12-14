This code navigates a binary tree in van Emde Boas layout according to the method in [1].
Veb contains precalculated values to be used in the navigation. Vebnew(n) creates a new
layout of a tree with n nodes. The allocated arrays are freed by vebfree(L). Vebpos(L,v)
takes a layout L and a Breadth-First-Order index of a node and returns the index of the node
in van Emde Boas layout. Vebpos returns 0 if the result is outside of this layout.
Vebsop(L,w) is the inverse operation returning the BFO index of a van Emde Boas index w.

[1] BRODAL, G. S., FAGERBERG, R., AND JACOB, R. 2002c. Cache oblivious search trees via binary trees of
small height. In Pro. 13th Annual ACM-SIAM Symposium on Discrete Algorithms. ACM-SIAM,
New York. 39–48.

