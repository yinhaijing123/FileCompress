#pragma once
#include<iostream>
#include<queue>
using namespace std;
#include<assert.h>

template <class W>
struct HuffmanTreeNode
{
    W _w;
    HuffmanTreeNode<W> *_left;
    HuffmanTreeNode<W> *_right;
    HuffmanTreeNode<W> *_parent;

    HuffmanTreeNode(const W& w)
        :_w(w)
        ,_left(NULL)
        ,_right(NULL)
        ,_parent(NULL)
    {}
};

template<class W>
class HuffmanTree 
{
    typedef HuffmanTreeNode<W> Node;
public:
    HuffmanTree()                                        
        :_root(NULL)
    {}
    HuffmanTree(W *a, size_t size,const W& invalid)
    {
        Create(a, size, invalid);
    }
	
    struct pNodeHuff                                                    
    {
        bool operator()(Node *l, Node *r)                               //仿函数
        {
            return l->_w > r->_w;
        }
    };
       

    void Create(W *a, size_t size, const W& invalid)
    {
        assert(a);
           
        priority_queue<Node *,vector<Node *>, pNodeHuff> min_heap;                      //堆内传节点，而非W（传W无法将整个树链接在一起）

        //1.构建最小堆
        for (size_t i = 0; i < size; i++)                    
        {
            if (a[i] != invalid)                                                        //invalid:保证W类型的有效值进入堆（使用参数invalid保证Huffman树的通用性）
            {
                min_heap.push(new Node(a[i]));
            }
        }
		
        //2.建Huffman树
		while (min_heap.size() > 1)                                 
        {
            Node *left = min_heap.top();
            min_heap.pop();
            Node *right = min_heap.top();
            min_heap.pop();

            Node *parent = new Node(left->_w + right->_w);
            parent->_left = left;
            parent->_right = right;
            left->_parent = parent;
            right->_parent = parent;
            min_heap.push(parent);
        }
        if (!min_heap.empty())
        {
            _root = min_heap.top();
        }
    }

    ~HuffmanTree()
    {
        Destroy(_root);
        _root = NULL;
    }

    void Destroy(Node *root)
    {
        if (root == NULL)
        {
            return;
        }
        Destroy(root->_left);
        Destroy(root->_right);
        delete root;
    }

    Node *GetRoot()
    {
        return _root;
    }
private:
    HuffmanTree(const HuffmanTree<W> &h);
    HuffmanTree<W> &operator=(const HuffmanTree<W> &h);

private:
    Node * _root;
};


#ifdef TESTHUFFMAN
void TestHuffmanTree()
{
    int a[] = { 0,1,2,3,4,5,6,7,8,9};
    HuffmanTree<int> h(a, sizeof(a)/sizeof(a[0]), -1);
}
#endif