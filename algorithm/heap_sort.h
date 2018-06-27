#include <new>

template<class T>
class HeapSort {
public:
    HeapSort() {}
    ~HeapSort () 
    {
        _destroy(_root);
    }

    template<class VT>
    struct node_t {
        VT v;
        node_t * left;
        node_t * right;
    };

    int build(T * arr, int len)
    {
        _root = new(std::nothrow) node_t<T>;
        int pos = 0;
       return  _build(_root, 1, arr, pos, len);
    }

    T * max()
    {
        if (!_root) {
            return NULL;
        }
        _max(_root);
        return &(_root->v);
    }

    T * min()
    {
        if (!_root) {
            return NULL;
        }
        _min(_root);
        return &(_root->v);
    }

private:
   void _max(node_t<T> * p) 
    {
        if (p && p->left) {
            _max(p->left);
            if (p->v  < p->left->v) {
                swap(p->v, p->left->v);
            }
        }

        if (p && p->right ) {
            _max(p->right);
            if (p->v  < p->right->v) {
                swap(p->v, p->right->v);
            }
        }

    }

   void _min(node_t<T> * p) 
   {
       if (p && p->left) {
           _min(p->left);
           if (p->v  > p->left->v) {
               swap(p->v, p->left->v);
           }
       }

       if (p && p->right ) {
           _min(p->right);
           if (p->v  > p->right->v) {
               swap(p->v, p->right->v);
           }
       }

   }

    void swap(T & a, T & b)
    {
        T c = a;
        a = b;
        b = c;
    }

    // parent:i ; lef child:2*i; right child:2*i + 1
    int _build(node_t<T> * n, int parent_index, T * arr, int & pos, int len)
    {
        n->v = arr[pos];

        if (pos + 1 < len && 2 * parent_index <= len) {
            ++ pos;
            n->left = new(std::nothrow) node_t<T>;
            _build(n->left, 2 * parent_index, arr, pos, len);
        } else {
            return 0;
        }

        if (pos + 1 < len && 2 * parent_index + 1 <= len) {
            ++ pos;
            n->right = new(std::nothrow) node_t<T>;
            _build(n->right, 2 * parent_index + 1, arr, pos, len);
        }

        return 0;
    }

    int _destroy(node_t<T> * p)
    {
        if (p && p->left) {
            _destroy(p->left);
        }

        if (p && p->right) {
            _destroy(p->right);
        }

        delete p;
    }

    node_t<T> * _root;
};
