#include<iostream>
#include<fstream>

class matrix_cal
{
private:
    int _n;
    double ** matrix;
    double * _x;

public:
    matrix_cal():_n{0},matrix{nullptr},_x{nullptr} {}
    
    matrix_cal(int n, std::istream & in=std::cin):_n{n},matrix{nullptr},_x{nullptr}
    {
        matrix=new double*[n];
        for(int i=0;i<n;i++)
            matrix[i]=new double[n];
        _x=new double[n];

        if(in == std::cin)
        {
            std::cout<<"please enter n dimension square matrix:"<<std::endl;
            for(int i=0;i<n;i++)
                for(int j=0;j<n;j++)
                    in>>matrix[i][j];
            std::cout<<"please enter n dimension vector b:"<<std::endl;
            for(int i=0;i<n;i++)
                in>>_x[i];
        }
        else
        {
            for(int i=0;i<n;i++)
                for(int j=0;j<n;j++)
                    in>>matrix[i][j];
            for(int i=0;i<n;i++)
                in>>_x[i];
        }
    }

    ~matrix_cal()
    {
        delete [] _x;
        for(int i=0;i<_n;i++)
            delete [] matrix[i];
        delete [] matrix;
    }

    void up_triangular()
    {
        for(int i=_n-1;i>=0;i--)
        {
            _x[i]=1.0*_x[i]/matrix[i][i];
            for(int j=0;j<i;j++)
                _x[j]=_x[j]-_x[i]*matrix[j][i];
        }
    }

    void low_triangular()
    {
        for(int i=0;i<_n;i++)
        {
            _x[i]=1.0*_x[i]/matrix[i][i];
            for(int j=_n-1;j>i;j--)
                _x[j]=_x[j]-_x[i]*matrix[j][i];
        }
    }

    void gauss()
    {
        if(matrix[0][0]==0)
        {
            int i;
            for(i=1;i<_n;i++)
                if(matrix[i][0]!=0)
                    break;
            double * temp;
            temp=matrix[0];
            matrix[0]=matrix[i];
            matrix[i]=temp;

            if(i==_n)
            {
                std::cerr<<"Error: the first column of matrix are all 0!"<<std::endl;
                std::exit(-1);
            }
        }
        for(int i=0;i<_n;i++)
            for(int j=i+1;j<_n;j++)
            {
                matrix[j][i]=1.0*matrix[j][i]/matrix[i][i];
                for(int k=i+1;k<_n;k++)
                    matrix[j][k]=matrix[j][k]-matrix[j][i]*matrix[i][k];
            }
        for(int i=0;i<_n;i++)
        {
            _x[i]=1.0*_x[i];
            for(int j=_n-1;j>i;j--)
                _x[j]=_x[j]-_x[i]*matrix[j][i];
        }
        for(int i=_n-1;i>=0;i--)
        {
            _x[i]=1.0*_x[i]/matrix[i][i];
            for(int j=0;j<i;j++)
                _x[j]=_x[j]-_x[i]*matrix[j][i];
        }
    }

    void show_ans()const
    {
        for(int i=0;i<_n;i++)
            std::cout<<_x[i]<<" ";
        std::cout<<std::endl;
    }

};

int main()
{
    using namespace std;
    ifstream fin("matrix.txt");
    int n;
    fin>>n;
    matrix_cal mtr(n,fin);
    mtr.gauss();
    mtr.show_ans();
    system("pause");
    return 0;
}