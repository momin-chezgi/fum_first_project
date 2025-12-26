#ifndef DSU_H
#define DSU_H


struct Node
{
    Node* parent = this;
    void find()
    {
        Node* temp = parent;
        while (temp->parent != temp)
        {
            temp = temp->parent;    //segmention fault
        }
        parent = temp;
    }
    bool unitewith(Node* other)
    {
        this->find();
        other->find();
        Node* root1 = this->parent;
        Node* root2 = other->parent;
        if (root1 != root2)
        {
            root2->parent = root1;
            return true; // united
        }
        else{
            return false; // already united, no action taken
        }
    }
    bool isConnected(Node* other)
    {
        this->find();
        other->find();
        return this->parent == other->parent;
    }

    //optional variables for additional features can be added here:
    //1. For coordinates:
    int x = 0;
    int y = 0;
};
#endif // DSU_H