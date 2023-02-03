#include<iostream>
#include<vector>
#include<cmath>
#include<iomanip>
using namespace std;

const double euler_number=2.718281828459045;

class numerical_intergration
{
private:
    const int extrapolate_ply=10;

    double (*func)(double x);
    double upper_bound;
    double lower_bound;
    int division;
    double precision;

    double trapezoid(double low,double up)
    {
        return 0.5*(up-low)*((*func)(up)+(*func)(low));
    }

    double trapezoid(double low,double up,int div)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=trapezoid(lower_bound+i*h,lower_bound+(i+1)*h);
        return sum;
    }

    double simpson(double low,double up)
    {
        return (up-low)*((*func)(up)+(*func)(low)+4*(*func)((up+low)*0.5))/6.0;
    }

    double simpson(double low,double up,int div)
    {
        double sum=0,h=(upper_bound-lower_bound)/div*1.0;
        for(int i=0;i<div;i++)
            sum+=simpson(lower_bound+i*h,lower_bound+(i+1)*h);
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

public:
    numerical_intergration() {}
    numerical_intergration(double (*f)(double x),int div=1):func{f},division{div} {}
    numerical_intergration(double (*f)(double x),double low,double up,int div=1)
    :func{f},upper_bound{up},lower_bound{low},division{div} {}
    numerical_intergration(double (*f)(double x),double low,double up,double pre)
    :func{f},upper_bound{up},lower_bound{low},precision{pre} {}

    double trapezoid()
    {
        if(division==1)
            return trapezoid(lower_bound,upper_bound);
        else
            return trapezoid(lower_bound,upper_bound,division);
    }

    double simpson()
    {
        if(division==1)
            return simpson(lower_bound,upper_bound);
        else
            return simpson(lower_bound,upper_bound,division);
    }

    double cotes()
    {
        if(division==1)
            return cotes(lower_bound,upper_bound);
        else
            return cotes(lower_bound,upper_bound,division);
    }

    double romberg()
    {
        double a[55];
        int count=0;
        while(true)
        {
            for(int i=0;i<10;i++)
                a[count++]=trapezoid(lower_bound,upper_bound,pow(2,i));
            for(int i=1;i<10;i++)
                for(int j=0;j<9-i;j++)
                {
                    double temp=pow(4,i);
                    a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
                }
            if(abs(a[count-1]-a[count-3])<precision)
                return a[count-1];
            else
            {
                count=0;
                int cur=1;
                int internal=pow(2,cur);

                while(true)
                {
                    for(int i=0;i<10;i++)
                        a[count++]=trapezoid(lower_bound,upper_bound,pow(internal,i));
                    for(int i=1;i<10;i++)
                        for(int j=0;j<9-i;j++)
                        {
                            double temp=pow(4,i);
                            a[count++]=(temp*a[count-(10-i)]-a[count-(11-i)])/(temp-1);
                        }
                    if(abs(a[count-1]-a[count-3])<precision)
                        return a[count-1];
                    else
                    {
                        cur++;
                        count=0;
                    }
                }
            }
        }
    }
};

double function(double x)
{
    return 4.0/(1+x*x);
}

int main()
{
    double (*fu)(double);
    fu=function;
    numerical_intergration cal1(fu,0,1,4);
    cout<<fixed<<setprecision(16);
    cout<<cal1.trapezoid()<<endl;
    cout<<cal1.simpson()<<endl;
    cout<<cal1.cotes()<<endl<<endl;

    numerical_intergration cal2(fu,0,1,8);
    cout<<cal2.trapezoid()<<endl;
    cout<<cal2.simpson()<<endl;
    cout<<cal2.cotes()<<endl<<endl;

    numerical_intergration cal3(fu,0,1,1e-8);
    cout<<cal3.romberg()<<endl<<endl;

    system("pause");
    return 0;
}