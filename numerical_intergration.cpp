#include<iostream>
#include<vector>
#include<cmath>
#include<iomanip>
#include<ctime>
using namespace std;

const double euler_number=2.718281828459045;

class numerical_intergration
{
private:
    const int extrapolate_ply=10;

    double (*func)(double x);
    double upper_bound;
    double lower_bound;
    int division=1;
    double precision;

    double trapezoid(double low,double up)
    {
        return 0.5*(up-low)*((*func)(up)+(*func)(low));
    }

    template<typename Function>
    double trapezoid(double low,double up,Function f)       //以下有 Function f 参数的皆为函数对象重载
    {
        return 0.5*(up-low)*(f(up)+f(low));
    }
    
    double trapezoid(double low,double up,int div)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=trapezoid(lower_bound+i*h,lower_bound+(i+1)*h);
        return sum;
    }

    template<typename Function>
    double trapezoid(double low,double up,int div,Function f)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=trapezoid(lower_bound+i*h,lower_bound+(i+1)*h,f);
        return sum;
    }

    double simpson(double low,double up)
    {
        return (up-low)*((*func)(up)+(*func)(low)+4*(*func)((up+low)*0.5))/6.0;
    }

    template<typename Function>
    double simpson(double low,double up,Function f)
    {
        return (up-low)*(f(up)+f(low)+4*f((up+low)*0.5))/6.0;
    }

    double simpson(double low,double up,int div)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=simpson(lower_bound+i*h,lower_bound+(i+1)*h);
        return sum;
    }

    template<typename Function>
    double simpson(double low,double up,int div,Function f)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=simpson(lower_bound+i*h,lower_bound+(i+1)*h,f);
        return sum;
    }

    double cotes(double low,double up)
    {
        return 1.0/90*(up-low)*(7*((*func)(low)+(*func)(up))+12*(*func)((up+low)*0.5)
               +32*((*func)(0.75*low+0.25*up)+(*func)(0.25*low+0.75*up)));
    }

    double cotes(double low,double up,int div)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=cotes(lower_bound+i*h,lower_bound+(i+1)*h);
        return sum;
    }

    template<typename Function>
    double cotes(double low,double up,Function f)
    {
        return 1.0/90*(up-low)*(7*(f(low)+f(up))+12*f((up+low)*0.5)
               +32*(f(0.75*low+0.25*up)+f(0.25*low+0.75*up)));
    }

    template<typename Function>
    double cotes(double low,double up,int div,Function f)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=cotes(lower_bound+i*h,lower_bound+(i+1)*h,f);
        return sum;
    }

    double romberg(double low,double up)
    {
        double a[55];
        int count=0,temp_i=1;
        int temp=1;

        a[count++]=trapezoid(low,up,temp_i);
        for(int i=1;i<10;i++)
            a[count++]=trapezoid(low,up,temp_i*=2);
        for(int i=1;i<10;i++)
            for(int j=0;j<9-i;j++)
            {
                temp*=4;
                a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
            }
        return a[count-1];
    }

    template<typename Function>
    double romberg(double low,double up,Function f)
    {
        double a[55];
        int count=0,temp_i=1;
        int temp=1;

        a[count++]=trapezoid(low,up,temp_i,f);
        for(int i=1;i<10;i++)
            a[count++]=trapezoid(low,up,temp_i*=2,f);
        for(int i=1;i<10;i++)
            for(int j=0;j<9-i;j++)
            {
                temp*=4;
                a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
            }
        return a[count-1];
    }

    double romberg(double low,double up,double pre)
    {
        double a[55];
        int count=0,temp_i=1;
        double temp=1;

        while(true)
        {
            a[count++]=trapezoid(low,up,temp_i);
            for(int i=1;i<10;i++)
                a[count++]=trapezoid(low,up,temp_i*=2);
            for(int i=1;i<10;i++)
                for(int j=0;j<9-i;j++)
                {
                    temp*=4;
                    a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
                }
            if(abs(a[count-1]-a[count-3])<precision)
                return a[count-1];
            else
            {
                count=0;
                temp=1;
                temp_i=2;
                int internal=2;

                while(true)
                {
                    a[count++]=trapezoid(low,up,temp_i);
                    for(int i=1;i<10;i++)
                        a[count++]=trapezoid(low,up,temp_i*=2);
                    for(int i=1;i<10;i++)
                        for(int j=0;j<9-i;j++)
                        {
                            temp*=4;
                            a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
                        }
                    if(abs(a[count-1]-a[count-3])<precision)
                        return a[count-1];
                    else
                    {
                        internal*=2;
                        count=0;
                        temp=1;
                        temp_i=internal;
                    }
                }
            }
        }
    }

    template<typename Function>
    double romberg(double low,double up,double pre,Function f)
    {
        double a[55];
        int count=0,temp_i=1;
        double temp=1;

        while(true)
        {
            a[count++]=trapezoid(low,up,temp_i,f);
            for(int i=1;i<10;i++)
                a[count++]=trapezoid(low,up,temp_i*=2,f);
            for(int i=1;i<10;i++)
                for(int j=0;j<9-i;j++)
                {
                    temp*=4;
                    a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
                }
            if(abs(a[count-1]-a[count-3])<precision)
                return a[count-1];
            else
            {
                count=0;
                temp=1;
                temp_i=2;
                int internal=2;

                while(true)
                {
                    a[count++]=trapezoid(low,up,temp_i,f);
                    for(int i=1;i<10;i++)
                        a[count++]=trapezoid(low,up,temp_i*=2,f);
                    for(int i=1;i<10;i++)
                        for(int j=0;j<9-i;j++)
                        {
                            temp*=4;
                            a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
                        }
                    if(abs(a[count-1]-a[count-3])<precision)
                        return a[count-1];
                    else
                    {
                        internal*=2;
                        count=0;
                        temp=1;
                        temp_i=internal;
                    }
                }
            }
        }
    }

public:
    numerical_intergration() {}
    numerical_intergration(int low,int up,int div,double (*f)(double x))
    :func{f},upper_bound{up},lower_bound{low},division{div},precision{-1} {}
    numerical_intergration(double low,double up,int div,double (*f)(double x))
    :func{f},upper_bound{up},lower_bound{low},division{div},precision{-1} {}
    numerical_intergration(int low,int up,double pre,double (*f)(double x))
    :func{f},upper_bound{up},lower_bound{low},precision{pre} {}
    numerical_intergration(double low,double up,double pre,double (*f)(double x))
    :func{f},upper_bound{up},lower_bound{low},precision{pre} {}
    numerical_intergration(int low,int up,int div=1)
    :func{nullptr},upper_bound{up},lower_bound{low},division{div},precision{-1} {}
    numerical_intergration(double low,double up,int div=1)
    :func{nullptr},upper_bound{up},lower_bound{low},division{div},precision{-1} {}
    numerical_intergration(int low,int up,double pre)
    :func{nullptr},upper_bound{up},lower_bound{low},precision{pre} {}
    numerical_intergration(double low,double up,double pre)
    :func{nullptr},upper_bound{up},lower_bound{low},precision{pre} {}

    double trapezoid()
    {
        if(division==1)
            return trapezoid(lower_bound,upper_bound);
        else
            return trapezoid(lower_bound,upper_bound,division);
    }

    template<typename Function>
    double trapezoid(Function f)
    {
        if(division==1)
            return trapezoid(lower_bound,upper_bound,f);
        else
            return trapezoid(lower_bound,upper_bound,division,f);
    }

    double simpson()
    {
        if(division==1)
            return simpson(lower_bound,upper_bound);
        else
            return simpson(lower_bound,upper_bound,division);
    }

    template<typename Function>
    double simpson(Function f)
    {
        if(division==1)
            return simpson(lower_bound,upper_bound,f);
        else
            return simpson(lower_bound,upper_bound,division,f);
    }

    double cotes()
    {
        if(division==1)
            return cotes(lower_bound,upper_bound);
        else
            return cotes(lower_bound,upper_bound,division);
    }

    template<typename Function>
    double cotes(Function f)
    {
        if(division==1)
            return cotes(lower_bound,upper_bound,f);
        else
            return cotes(lower_bound,upper_bound,division,f);
    }

    double romberg()
    {
        if(precision==-1)
            return romberg(lower_bound,upper_bound);
        else
            return romberg(lower_bound,upper_bound,precision);
    }

    template<typename Function>
    double romberg(Function f)
    {
        if(precision==-1)
            return romberg(lower_bound,upper_bound,f);
        else
            return romberg(lower_bound,upper_bound,precision,f);
    }

};

template<typename T>
class funct
{
public:
    T operator() (T x)
    {
        return 4.0/(1+x*x);
    }
};

double function_f(double x)
{
    return 4.0/(1+x*x);
}

int main()
{
    
    funct<double> fu;    //函数对象方式
    numerical_intergration cal(0,1,1e-8);
    //cout.setf(ios_base::showpoint);         //显示小数后的0，两种使用方式
    cout<<showpoint;
    cout.precision(8);      //cout.precision() 设置小数点后精度；cout<<setprecision() 设置输出小数位数
    cout<<cal.trapezoid(fu)<<endl;
    cout<<cal.simpson(fu)<<endl;
    cout<<cal.cotes(fu)<<endl;
    cout<<cal.romberg(fu)<<endl<<endl;

    //函数指针方式
    clock_t start_2=clock();
    numerical_intergration cal2(0,1,8,function_f);
    cout<<setprecision(16)<<cal2.cotes(function_f)<<endl;
    clock_t end_2=clock();
    cout<<(double)(end_2-start_2)/CLOCKS_PER_SEC<<endl<<endl;
  
    clock_t start_3=clock();
    numerical_intergration cal3(0,1,1e-9,function_f);
    cout<<cal3.romberg()<<endl;
    clock_t end_3=clock();
    cout<<(double)(end_3-start_3)/CLOCKS_PER_SEC<<endl<<endl;

    clock_t start_4=clock();
    numerical_intergration cal4(0,1,1e-9,[](double x){return 4.0/(1+x*x);});        //lambda函数方式
    cout<<cal4.romberg()<<endl;
    clock_t end_4=clock();
    cout<<(double)(end_4-start_4)/CLOCKS_PER_SEC<<endl<<endl;
    
    system("pause");
    return 0;
}