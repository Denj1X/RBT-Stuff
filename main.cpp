#include <bits/stdc++.h>
using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

struct Node {
    int val;
    bool color;
    Node *left, *right, *parent;
    Node(int val, bool col, Node * a, Node * b, Node * c) :
        val(val), color(col), left(a), right(b), parent(c){}
};

class RBT {
private:
    Node * root;
    Node * stop;
    void LeftRotate(Node* x){
        Node* y  = x -> right;
        x -> right = y -> left;
        if(y -> left != stop)
            y -> left -> parent = x;
        y -> parent = x -> parent;
        if(x -> parent== stop)
            root = y;
        else if(x == x -> parent -> left)
            x -> parent -> left = y;
        else
            x -> parent -> right = y;
        y -> left = x;
        x -> parent = y;
    }

    void RightRotate(Node* x) {
        Node* y = x -> left;
        x -> left = y -> right;
        if(y -> right != stop)
            y -> right -> parent = x;
        y -> parent = x -> parent;
        if(x -> parent == stop)
            root = y;
        else if(x == x -> parent -> right)
            x -> parent-> right = y;
        else
            x -> parent -> left = y;
        y -> right = x;
        x -> parent = y;
    }

    void fix_insert(Node* curent) {
        while (!curent -> parent -> color) {
            if (curent->parent == curent->parent->parent->left) {
                Node *auxiliar = curent->parent->parent->right;
                if (!auxiliar->color) {
                    curent->parent->color = true;
                    auxiliar->color = true;
                    curent->parent->parent->color = false;
                    curent = curent->parent->parent;
                } else {
                    if (curent == curent->parent->right) {
                        curent = curent->parent;
                        LeftRotate(curent);
                    }
                    curent->parent->color = true;
                    curent->parent->parent->color = false;
                    RightRotate(curent->parent->parent);
                }
            } else {
                Node *auxiliar = curent->parent->parent->left;
                if (!auxiliar->color) {
                    curent->parent->color = true;
                    auxiliar->color = true;
                    curent->parent->parent->color = false;
                    curent = curent->parent->parent;
                } else {
                    if (curent == curent->parent->left) {
                        curent = curent->parent;
                        RightRotate(curent);
                    }
                    curent->parent->color = true;
                    curent->parent->parent->color = false;
                    LeftRotate(curent->parent->parent);
                }
            }
        }
        root->color = true;
    }

    void swap(Node* first, Node* second) {
        if(first -> parent == stop)
            root = second;
        else if(first == first -> parent -> left)
            first -> parent -> left = second;
        else
            first -> parent -> right = second;
        second -> parent = first -> parent;
    }

    void fix_delete(Node* curent) {
        while(curent != root && curent -> color) {
            if (curent == curent->parent->left) {
                Node *auxiliar = curent->parent->right;
                if (!auxiliar->color) {
                    auxiliar->color = true;
                    curent->parent->color = false;
                    LeftRotate(curent->parent);
                    auxiliar = curent->parent->right;
                }
                if (auxiliar->left->color && auxiliar->right->color) {
                    auxiliar->color = false;
                    curent = curent->parent;
                } else {
                    if (auxiliar->right->color) {
                        auxiliar->left->color = true;
                        auxiliar->color = false;
                        RightRotate(auxiliar);
                        auxiliar = curent->parent->right;
                    }
                    auxiliar->color = curent->parent->color;
                    curent->parent->color = true;
                    auxiliar->right->color = true;
                    LeftRotate(curent->parent);
                    curent = root;
                }
            } else {
                Node *auxiliar = curent->parent->left;
                if (!auxiliar->color) {
                    auxiliar->color = true;
                    curent->parent->color = false;
                    RightRotate(curent->parent);
                    auxiliar = curent->parent->left;
                }
                if (auxiliar->left->color && auxiliar->right->color) {
                    auxiliar->color = false;
                    curent = curent->parent;
                } else {
                    if (auxiliar->left->color) {
                        auxiliar->right->color = true;
                        auxiliar->color = false;
                        LeftRotate(auxiliar);
                        auxiliar = curent->parent->left;
                    }
                    auxiliar->color = curent->parent->color;
                    curent->parent->color = true;
                    auxiliar->left->color = true;
                    RightRotate(curent->parent);
                    curent = root;
                }
            }
        }
        curent->color = true;
    }

public:
    RBT() {
        stop = new Node(0,true,NULL,NULL,NULL);
        root = stop;
    }

    Node * getRoot() {
        return root;
    }

    void tree_insert(int &val) {
        Node * nou = new Node(val,false,stop,stop,stop);
        Node* y = NULL;
        Node* x = this->root;
        while (x != stop) {
            y = x;
            if (nou->val < x->val)
                x = x->left;
            else
                x = x->right;
        }
        nou->parent = y;
        if (y == NULL) {
            nou->parent = stop;
            root = nou;
        } else if (nou->val < y->val)
            y->left = nou;
        else
            y->right = nou;
        if (nou->parent == stop){
            nou->color = true;
            return;
        }
        if (nou->parent->parent == NULL)
            return;
        fix_insert(nou);
    }

    void tree_delete(int &val) {
        Node* z = root;
        while(z != stop && z -> val != val){
            if(val < z -> val)
                z = z -> left;
            else
                z = z -> right;
        }
        if(z != stop) {
            Node* x, *y = z;
            bool color_originala = y -> color;
            if(z -> left == stop){
                x = z -> right;
                swap(z, z -> right);
            }
            else if(z -> right == stop){
                x = z -> left;
                swap(z, z -> left);
            }
            else{
                y = z -> right;
                while(y -> left != stop)
                    y = y -> left;
                color_originala = y -> color;
                x = y -> right;
                if(y -> parent == z)
                    x -> parent = y;
                else{
                    swap(y, y -> right);
                    y -> right = z -> right;
                    y -> right -> parent = y;
                }
                swap(z, y);
                y -> left = z -> left;
                y -> left -> parent = y;
                y -> color = z -> color;
            }
            if(color_originala)
                fix_delete(x);
        }
    }

    void tree_search(int &val) {
        Node* x = root;
        while(x != stop && x -> val != val){
            if(val < x -> val)
                x = x -> left;
            else
                x = x -> right;
        }
        if(x != stop)
            fout << "1\n";
        else
            fout <<  "0\n";
        }

    void succesor(int &val) {
        int nr = -1;
        Node *x = root;
        while (x != stop) {
            if (x->val >= val) {
                nr = x->val;
                if (nr == val)
                    break;
                x = x->left;
            } else
                x = x->right;
        }
        fout << nr << "\n";
    }

    void predecesor(int &val) {
        int nr = -1;
        Node *x = root;
        while (x != stop) {
            if (x->val <= val) {
                nr = x->val;
                if (nr == val)
                    break;
                x = x->right;
            } else
                x = x->left;
        }
        fout << nr << "\n";
    }

    void inorder(int &left, int &right, Node* nod) {
        if (left <= nod->val)
            if (nod->left != stop)
                inorder(left, right, nod->left);
        if (left <= nod-> val && right >= nod->val)
            fout << nod->val << " ";
        if (right >= nod->val)
            if (nod->right != stop)
                inorder(left, right, nod->right);
    }
};

int main() {
    int q, op, x, y;
    RBT tree;
    fin >> q;
    while(q--) {
        fin >> op >> x;
        if(op == 1)
            tree.tree_insert(x);
        else if(op == 2)
            tree.tree_delete(x);
        else if(op == 3)
            tree.tree_search(x);
        else if(op == 4)
            tree.predecesor(x);
        else if(op == 5)
            tree.succesor(x);
        else if(op == 6) {
            fin >> y;
            tree.inorder(x, y, tree.getRoot());
            fout << '\n';
        }
    }
    return 0;
}
