#define MAX_LEVELS 20
#define BLOCK_SIZE 2048

typedef struct TreeNode {
    int size;
    int used;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent; // Added parent pointer
} TreeNode;

TreeNode *root = NULL;

void initialize_tree(){
    root = (TreeNode*) malloc(sizeof(TreeNode));
    root->size = BLOCK_SIZE;
    root->used = 0 ;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL; // Initialize parent pointer to NULL
    printf("root %p\n",root);
}

int log_2(int n) {
    return (int) (log(n) / log(2));
}

int next_power_of_2(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

TreeNode* find_smallest_treeNode(TreeNode* treeNode, int size) {
    if (treeNode == NULL) {
        return NULL;
    } else if (treeNode->size < size) {
        return NULL;
    } else if (treeNode->left == NULL && treeNode->right == NULL && !treeNode->used) {
        return treeNode;
    } else {
        TreeNode* leftNode = find_smallest_treeNode(treeNode->left, size);
        TreeNode* rightNode = find_smallest_treeNode(treeNode->right, size);
        if (leftNode && rightNode) {
            return leftNode->size < rightNode->size ? leftNode : rightNode;
        } else if (leftNode) {
            return leftNode;
        } else {
            return rightNode;
        }
    }
}

TreeNode* find_treeNode(TreeNode* treeNode, int size) {
    if (treeNode == NULL) {
        return NULL;
    } else if (treeNode->size < size) {
        return NULL;
    } else if (!treeNode->used && treeNode->left == NULL && treeNode->right == NULL) {
        return treeNode;
    } else {
        TreeNode* smallestNode = find_smallest_treeNode(treeNode->left, size);
        if (smallestNode) {
            return smallestNode;
        }
        return find_smallest_treeNode(treeNode->right, size);
    }
}


void split_tree(TreeNode* treeNode, int size) {
    if (treeNode->size == size) {
        treeNode->used = 1;
        return;
    }
    treeNode->used = 1;

    if (treeNode->left == NULL && treeNode->right == NULL) {
        treeNode->left = (TreeNode*)malloc(sizeof(TreeNode));
        treeNode->left->size = treeNode->size / 2;
        treeNode->left->used = 0;
        treeNode->left->left = NULL;
        treeNode->left->right = NULL;
        treeNode->left->parent = treeNode;

        treeNode->right = (TreeNode*)malloc(sizeof(TreeNode));
        treeNode->right->size = treeNode->size / 2;
        treeNode->right->used = 0;
        treeNode->right->left = NULL;
        treeNode->right->right = NULL;
        treeNode->right->parent = treeNode;
    }
    
    treeNode->size /= 2; // Update the parent node's size

    printf("Block split: size=%d, used=%d\n", treeNode->size, treeNode->used);
    printf("Left child: size=%d, used=%d\n", treeNode->left->size, treeNode->left->used);
    printf("Right child: size=%d, used=%d\n", treeNode->right->size, treeNode->right->used);

    if (treeNode->left->size >= treeNode->right->size) {
        split_tree(treeNode->left, size);
    } else {
        split_tree(treeNode->right, size);
    }
}

void *buddy_malloc(int size){
    int blockSize = next_power_of_2(size);
    TreeNode *treeNode = find_treeNode(root , blockSize);
    // printf("blocksiZe: %d treenode: %p\n" , blockSize , treeNode);
    if(treeNode){
        split_tree(treeNode , blockSize);
        return (void*) (treeNode + 1); // Offset by size of TreeNode struct
    }else{
        printf("Error: Could not find free block of size %d\n", blockSize);
        return NULL;
    }
}

void merge_node(TreeNode* treeNode) {
    if (treeNode->parent && treeNode->parent->left && treeNode->parent->right &&
        !treeNode->parent->used && !treeNode->parent->left->used && !treeNode->parent->right->used) {
        treeNode->parent->used = 0;
        free(treeNode->parent->left);
        free(treeNode->parent->right);
        treeNode->parent->left = NULL;
        treeNode->parent->right = NULL;
        merge_node(treeNode->parent);
    } else if (treeNode->parent) {
        // Only merge if the parent pointer is valid
        if (treeNode->parent->left == treeNode) {
            free(treeNode->parent->left);
            treeNode->parent->left = NULL;
        } else if (treeNode->parent->right == treeNode) {
            free(treeNode->parent->right);
            treeNode->parent->right = NULL;
        }
        merge_node(treeNode->parent);
    }
    // No parent, just return
}


void buddy_free(void *ptr) {
    if (ptr == NULL){
        // printf("memory to be freed is NULL, it seems you didnt allocate it or the memory was full when you was allocating\n");
        return; 
    }
    TreeNode *treeNode = (TreeNode*) ((char*) ptr - sizeof(TreeNode)); // Offset by size of TreeNode struct
    treeNode->used = 0;
    merge_node(treeNode);
}