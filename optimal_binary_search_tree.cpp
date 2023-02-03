#include<iostream>
#include<string>
using namespace std;

class optimal_bst
{
private:
    struct Node
    {
        string name;
        double probility;
        Node* left;
        Node* right;

        Node():probility{0},left{nullptr},right{nullptr} {}
        Node(string st,double pr):name{st},probility{pr},left{nullptr},right{nullptr} {}
    };

    struct node_arr
    {
        int _name;
        double _pro;

        node_arr():_pro{0} {}
        node_arr(int st,double pr):_name{st},_pro{pr} {}
        node_arr(node_arr& noa):_name{noa._name},_pro{noa._pro} {}
        void operator=(node_arr& noa)
        {
            _name=noa._name;
            _pro=noa._pro;
        }
    };

    Node* root;
    int number;
    string* name_arr;
    double* probility_arr;
    node_arr** compute;

    void find_optimal()
    {
        node_arr temp;
        temp._pro=0;
        for(int i=0;i<number;i++)
        {
            compute[i]=new node_arr[number];
            for(int j=0;j<number;j++)
            {
                if(i==0)
                {
                    compute[i][j]._name=j;
                    compute[i][j]._pro=probility_arr[j];
                }
                else
                {
                    double sum_i_j=0;
                    for(int k=j;k<=j+i;k++)
                        sum_i_j+=probility_arr[k];
                    temp._pro=compute[i-1][j+1]._pro+sum_i_j;
                    temp._name=j;
                    for(int k=j+1;k<j+i;k++)
                    {
                        if(temp._pro>compute[k-j-1][j]._pro+compute[j+i-k-1][k+1]._pro+sum_i_j)
                        {
                            temp._pro=compute[k-j-1][j]._pro+compute[j+i-k-1][k+1]._pro+sum_i_j;
                            temp._name=k;
                        }
                    }
                    if(temp._pro>compute[i-1][j]._pro+sum_i_j)
                    {
                        temp._pro=compute[i-1][j]._pro+sum_i_j;
                        temp._name=j+i;
                    }
                    compute[i][j]=temp;
                }
            }
        }
    }

    void build_obst(Node* & rt,int index,int left,int right)
    {
        if(left>right)
            return;
        else if(left==right)
        {
            rt=new Node(name_arr[left],probility_arr[left]);
            return;
        }
        rt=new Node(name_arr[index],probility_arr[index]);
        if(index-left-1>=0)
            build_obst(rt->left,compute[index-left-1][left]._name,left,index-1);
        if(right-index-1>=0)
            build_obst(rt->right,compute[right-index-1][index+1]._name,index+1,right);
    }

    void show_obst(Node* rt)const
    {
        if(rt==nullptr)
            return;
        show_obst(rt->left);
        cout<<rt->name<<" ";
        show_obst(rt->right);
    }

    void makeEmpty(Node *& t)
    {
        if(t!=nullptr)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

public:
    optimal_bst():root{nullptr},number{0},name_arr{nullptr},probility_arr{nullptr},compute{nullptr} {}

    optimal_bst(int num,string* st,double* pr):number{num},root{nullptr},name_arr{st},probility_arr{pr},compute{nullptr}
    {
        compute=new node_arr*[num];
        find_optimal();
    }

    ~optimal_bst()
    {
        if(compute!=nullptr)
        {
            for(int i=0;i<number;i++)
                delete [] compute[i];
            delete [] compute;
        }

        makeEmpty(root);
    }

    void build_obst()
    {
        if(root==nullptr)
        {
            root=new Node(name_arr[compute[number-1][0]._name],probility_arr[compute[number-1][0]._name]);
            build_obst(root->left,compute[compute[number-1][0]._name-1][0]._name,0,compute[number-1][0]._name-1);
            build_obst(root->right,compute[compute[number-1][0]._name+1][compute[number-1][0]._name+1]._name,compute[number-1][0]._name+1,number-1);
        }
    }

    void show_table()const
    {
    	for(int i=0;i<number;i++)
    	{
    		for(int j=0;j<number-i;j++)
    			cout<<name_arr[compute[i][j]._name]<<" "<<compute[i][j]._pro<<"   ";
			cout<<endl;
		}
	}

    void show_obst()const
    {
        show_obst(root);
    }

    void show_min_cost()const
    {
        cout<<compute[number-1][0]._pro;
    }
};

int main()
{
    string str[7]={"a","am","and","egg","if","the","two"};
    double pro[7]={0.22,0.18,0.20,0.05,0.25,0.02,0.08,};

    optimal_bst obst(7,str,pro);
    obst.show_table();
    cout<<endl;
    obst.build_obst();
    obst.show_obst();
    cout<<endl;
    obst.show_min_cost();
    cout<<endl;

    system("pause");
    return 0;
}