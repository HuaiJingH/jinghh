#include<iostream>
using namespace std;
class AVLtree
{
private:
    struct node
    {
        int data;
        node * right;
        node * left;
        int height;
        
        node(int da):data{da},right{nullptr},left{nullptr},height{-1} {}
    };
    node * root;

    void insert(int _da,node *& _rt)
    {
        if(_rt==nullptr)
        {
            _rt=new node(_da);
            _rt->height=0;
            return;
        }
        
        if(_rt->data > _da)
        {
            insert(_da,_rt->left);
            if(_rt->height < _rt->left->height + 1)
                _rt->height= _rt->left->height + 1;
        }
        else if(_rt->data < _da)
        {
            insert(_da,_rt->right);
            if(_rt->height < _rt->right->height + 1)
                _rt->height= _rt->right->height + 1;
        }
        balance(_rt);

        return;
    }

    void balance(node *& _rt)
    {
        if(_rt==nullptr || (_rt->left==nullptr && _rt->right==nullptr))
            return;
        
        if(_rt->left==nullptr && _rt->right->height >=1)
        {
            if(_rt->right->right != nullptr)
                rotate_rr(_rt);
            else if(_rt->right->left != nullptr)
                rotate_rl(_rt);
        }
        else if(_rt->right==nullptr && _rt->left->height >=1) 
        {
            if(_rt->left->right != nullptr)
                rotate_lr(_rt);
            else if(_rt->left->left != nullptr)
                rotate_ll(_rt);
        }
        else if((_rt->left!=nullptr && _rt->right!=nullptr) && ( _rt->right->height - _rt->left->height > 1))
        {
            if((_rt->right->right->height > _rt->right->left->height))
                rotate_rr(_rt);
            else if((_rt->right->right->height < _rt->right->left->height))
                rotate_rl(_rt);
        }
        else if((_rt->left!=nullptr && _rt->right!=nullptr) && (_rt->right->height - _rt->left->height < -1))
        {
            if((_rt->left->right->height < _rt->left->left->height))
                rotate_ll(_rt);
            else if((_rt->left->right->height > _rt->left->left->height))
                rotate_lr(_rt);
        }
        else
            return;
    }

    void _reheight(node *& _rt)
    {
        if(_rt->left==nullptr && _rt->right==nullptr)
            _rt->height=0;
        else if(_rt->left==nullptr && _rt->right!=nullptr)
            _rt->height=_rt->right->height+1;
        else if(_rt->left!=nullptr && _rt->right==nullptr)
            _rt->height=_rt->left->height+1;
        else
            _rt->height=(_rt->left->height > _rt->right->height ? _rt->left->height : _rt->right->height) +1;
    }

    void reheight(node *& _rt)
    {
        if(_rt->right!=nullptr)
            _reheight(_rt->right);
        if(_rt->left!=nullptr)
            _reheight(_rt->left);
        _reheight(_rt);
    }

    void rotate_ll(node *& _rt)
    {
        node* temp=_rt;
        _rt=_rt->left;
        temp->left=_rt->right;
        _rt->right=temp;
        reheight(_rt); 
    }

    void rotate_rr(node *& _rt)
    {
        node* temp=_rt;
        _rt=_rt->right;
        temp->right=_rt->left;
        _rt->left=temp;
        reheight(_rt);
    }

    void rotate_rl(node *& _rt)
    {
        node* temp=_rt;
        _rt=_rt->right->left;
        temp->right->left=_rt->right;
        _rt->right=temp->right;
        temp->right=_rt->left;
        _rt->left=temp;
        reheight(_rt);
    }
    
    void rotate_lr(node *& _rt)
    {
        node* temp=_rt;
        _rt=_rt->left->right;
        temp->left->right=_rt->left;
        _rt->left=temp->left;
        temp->left=_rt->right;
        _rt->right=temp;
        reheight(_rt);
    }

    void empty(node *& _rt)
    {
        if(_rt->right != nullptr)
            empty(_rt->right);
        else if(_rt->left != nullptr)
            empty(_rt->left);
        else if(_rt->right == nullptr && _rt->left == nullptr)
            delete _rt;
    }

    void show(node* _rt)const
    {
        if(_rt==nullptr)
            return;
        show(_rt->left);
        cout<<_rt->data<<" ";
        show(_rt->right);
    }

public:
    AVLtree():root{nullptr} {}
    ~AVLtree()
    {
        empty(root);
    }
    
    void insert(int da)
    {
        insert(da,root);
    }

    int root_data()const
    {
        return root->data;
    }

    int root_height()const
    {
        return root->height;
    }

    void show()const
    {
        show(root);
    }
};

int main()
{
    int n;
    cin>>n;
    int a[n];
    for(int i=0;i<n;i++)
        cin>>a[i];
    AVLtree avl;

    for(int i=0;i<n;i++)
        avl.insert(a[i]);
    cout<<avl.root_data()<<endl;
    avl.show();
    system("pause");
    return 0;
}