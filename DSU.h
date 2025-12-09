#ifndef DSU_H
#define DSU_H


struct Node
{
    Node* parent = this;
    void find()
    {
        while (parent != this)
        {
            parent = parent->parent;
        }
    }
    void unitewith(Node* other)
    {
        this->find();
        other->find();
        Node* root1 = this->parent;
        Node* root2 = other->parent;
        if (root1 != root2)
        {
            root2->parent = root1;
        }
    }
};
#endif // DSU_H