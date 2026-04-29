#include <iostream>

struct node{
    int key;
    node* right;
    node* left;
};

struct tree{
    node* root;
};

node* n_search(tree t, int val)
{
    node* current = t.root;
    while(current)
    {
        if(current->key == val)
            return current;
        if(current->key > val)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

node* n_search_parent(tree t, int val)
{
    node* current = t.root;
    node* parent = NULL;
    while(current)
    {
        if(current->key ==  val)
            return parent;
        parent = current;
        if(current->key > val)
            current = current->left;
        else
            current = current->right;
    }
    return NULL;
}

node* minimum(tree t)
{
    if(t.root == NULL)
        return NULL;
    node* current = t.root;
    while(current->left != NULL)
        current = current->left;
    return current;
}

node* maximum(tree t)
{
    if(t.root == NULL)
        return NULL;
    node* current = t.root;
    while(current->right != NULL)
        current = current->right;
    return current;
}

void inorder(tree t)
{
    if(t.root == NULL)
        return;
    
        tree left;
        tree right;
        left.root = t.root->left;
        right.root = t.root->right;

        inorder(left);
        std::cout << t.root->key;
        inorder(right);
}

void insert(tree* t, int k)
{
    node* new_node = new node;
    new_node->key = k;
    new_node->right = NULL;
    new_node->left = NULL;

    if(t->root == NULL)
    {
        t->root = new_node;
        return;
    }
    
    node* parent = NULL;
    node* current = t->root;

    while(current != NULL)
    {
        parent = current;
        if(k <= current->key)
            current = current->left;
        else
            current = current->right;
    }

    if(k <= parent->key)
        parent->left = new_node;
    else
        parent->right = new_node;
}

void delete_node(tree* t, int val)
{
    node* parent = n_search_parent(*t, val);
    node* current = n_search(*t, val);

    if(current == NULL)
        return;
    if(current->left == NULL && current->right == NULL)
    {
        if(parent == NULL)
            t->root = NULL;
        else if(parent->left == current)
            parent->left = NULL;
        else
            parent->right = NULL;
        
        delete current;
    }
    else if(current->left == NULL || current->right == NULL)
    {
        node* child = (current->left != NULL) ? current->left : current->right;
        if(parent == NULL)
            t->root = child;
        else if(parent->left == current)
            parent->left = child;
        else
            parent->right = child;

        delete current;
    }
    else
    {
        node* succ_parent = current;
        node* succ = current->right;
        while(succ->left != NULL)
        {
            succ_parent = succ;
            succ = succ->left;
        }
    current->key = succ->key;

    if(succ_parent->left == succ)
        succ_parent->left = succ->right;
    else
        succ_parent->right = succ->right;

    delete succ;
    }
}

int main()
{
    tree t;
    t.root = NULL;
    insert(&t, 5);
    insert(&t, 3);
    insert(&t, 7);
    insert(&t, 1);
    insert(&t, 4);
    insert(&t, 6);
    insert(&t, 9);


    std::cout << "Inorder: ";
    inorder(t);
    std::cout << std::endl;

    delete_node(&t, 5);
    std::cout << "After deleting node 5: ";
    inorder(t);
    std::cout << std::endl;

    std::cout << "Minimum: " << minimum(t)->key << std::endl;
    std::cout << "Maximum: " << maximum(t)->key << std::endl;
    return 0;

}



