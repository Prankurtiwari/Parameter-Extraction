#include <stdio.h>
#include <conio.h>

float freq[30];
float cb[30],cpg[30],cpd[30],rg[30],rs[30],rd[30],lg[30],ls[30],ld[30],w[30]; //Extrinsic Parameter
float cgd[30],cgs[30],ri[30],gm[30],tau[30],cds[30],cdc[30],rds[30]; //Intrinsic Parameter
float vgs,vth,rch;

typedef struct num
{
    float x,y;
}num;

num s[30][4]; //Global Declaration
num y[30][4]; //Global Declaration
num z[30][4]; //Global Declaration
num Zo; //Characteristics Impedance
num Yo; //Characteristics Admittance

num add(num ca, num cb)
{
    num cc;
    cc.x=ca.x+cb.x;
    cc.y=ca.y+cb.y;
    return cc;
}

num mult(num ca, num cb)
{
    num cc;
    cc.x=ca.x*cb.x-ca.y*cb.y;
    cc.y=ca.x*cb.y+ca.y*cb.x;
    return cc;
}

num sub(num ca,num cb)
{
    num cc;
    cc.x=ca.x-cb.x;
    cc.y=ca.y-cb.y;
    return cc;
}

num conj(num ca)
{
    num cc;
    cc.x=ca.x;
    cc.y=-ca.y;
    return cc;
}

num divi(num ca,num cb)
{
    num cc,cc1,cc2,pro;
    cc1=conj(cb);
    cc2=mult(cc1,ca);
    pro=mult(cb,cc1);
    cc.x=cc2.x/pro.x;
    cc.y=cc2.y/pro.x;
    return cc;

}

/********* y to z *********/
num y2z(num y[30][4])
{
    num t0,t1,t2,t3,c1;
    num delta;
    int i;
    c1.x=(1.000000);
    c1.y=0.000000;
    c1.x=(-1)*c1.x;
    for(i=0;i<30;i++)
    {
        t0=y[i][0];
        t1=y[i][1];
        t2=y[i][2];
        t3=y[i][3];
        delta=sub(mult(t0,t3),mult(t1,t2));
        z[i][0]=divi(t3,delta);
        z[i][1]=mult(c1,divi(t1,delta));
        z[i][2]=mult(c1,divi(t2,delta));
        z[i][3]=divi(t0,delta);
    }
}

/********* z to y *********/
num z2y(num z[30][4])
{
    num t0,t1,t2,t3,c1;
    num delta;
    int i;
    c1.x=(1.000000);
    c1.y=0.000000;
    c1.x=(-1)*c1.x;
    for(i=0;i<30;i++)
    {
        t0=z[i][0];
        t1=z[i][1];
        t2=z[i][2];
        t3=z[i][3];
        delta=sub(mult(t0,t3),mult(t1,t2));
        y[i][0]=divi(t3,delta);
        y[i][1]=mult(c1,divi(t1,delta));
        y[i][2]=mult(c1,divi(t2,delta));
        y[i][3]=divi(t0,delta);
    }
}

/***************** s to y ****************/

num *s2y(num s[30][4])
{
    num c1,delta,two;
    num d1,d2,d3,ds1,ds2,ay11,ay12,ay21,ay22,s21,h21,h22;
    num t1,t2,t3,t0;
    int i;
    //num y[30][4];
    /*for(i=0;i<30;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f\t",s[i][j].x);
    }*/

    c1.x=1.000000;
    c1.y=0.000000;
    printf("\nEnter Value for Characteristics Impedance : ");
    scanf("%f",&Zo.x);
    Yo.x=(1/Zo.x);
    Zo.y=0.000000;
    Yo.y=0.000000;
    for(i=0;i<30;i++)
    {
        t0=s[i][0];
        t1=s[i][1];
        t2=s[i][2];
        t3=s[i][3];
        d1=add(c1,t0);
        d2=add(c1,t3);
        d3=mult(t1,t2);
        delta=sub(mult(d1,d2),d3);
        ds1=sub(c1,t0);
        ds2=sub(c1,t3);
        ay11=add(mult(d2,ds1),d3);
        two=add(c1,c1);
        two.x=((-1)*two.x);
        ay12=mult(two,t1);
        ay21=mult(two,t2);
        ay22=add(mult(d1,ds2),d3);
        y[i][0]=mult(divi(ay11,delta),Yo);
        y[i][1]=mult(divi(ay12,delta),Yo);
        y[i][2]=mult(divi(ay21,delta),Yo);
        y[i][3]=mult(divi(ay22,delta),Yo);
    }
    return y;
}

void inductance(num z[30][4])
{
    int i;
    float temp1,temp2;
    for(i=0;i<30;i++)
        w[i]=(6.2832*freq[i]);
    for(i=0;i<30;i++)
    {
        temp1=(z[i][1].y-z[i-1][1].y);
        temp2=w[i]-w[i-1];
        ls[i]=temp1/temp2;
        temp1=(z[i][0].y-z[i-1][0].y);
        temp2=w[i]-w[i-1];
        lg[i]=(temp1/temp2)-ls[i];
        temp1=(z[i][3].y-z[i-1][0].y);
        temp2=w[i]-w[i-1];
        ld[i]=(temp1/temp2)-ls[i];

        printf("Ls[%d] = %f\tLg[%d] = %f\tLd[%d] = %f\n",i,ls[i],i,lg[i],i,ld[i]);

    }
}

void modifiedz(num z[30][4],float freq1[30])
{
    int i,j;
    for(i=0;i<30;i++)
        w[i]=(6.2832*freq[i]);
    for(i=0;i<30;i++)
    {
        z[i][0].y=z[i][0].y-(w[i]*lg[i]);
        z[i][3].y=z[i][3].y-(w[i]*ld[i]);
    }
}

void modifiedy(num y[30][4],float freq1[30])
{
    int i,j;
    for(i=0;i<30;i++)
        w[i]=(6.2832*freq[i]);
    for(i=0;i<30;i++)
    {
        y[i][0].y=y[i][0].y-(w[i]*cpg[i]);
        y[i][3].y=y[i][3].y-(w[i]*cpd[i]);
    }
}


void extrinsiccap(num y[30][4],float freq1[30])
{
    int i;
    float temp1,temp2;
    for(i=0;i<30;i++)
        w[i]=(6.2832*freq1[i]);
    for(i=1;i<22;i++)
    {
        temp1=(y[i][1].y-y[i-1][1].y);
        temp2=w[i]-w[i-1];
        cb[i]=(temp1/temp2);
        temp1=(y[i][0].y-y[i-1][0].y);
        temp2=w[i]-w[i-1];
        cpg[i]=(temp1/temp2)-((2/3)*(cb[i]));
        temp1=(y[i][3].y-y[i-1][3].y);
        temp2=w[i]-w[i-1];
        cpd[i]=(temp1/temp2)+cb[i];

        printf("Cb[%d] = %f\tCpg[%d] = %f\tCpd[%d] = %f\n",i,cb[i],i,cpg[i],i,cpd[i]);

    }
}

void slope(num y[30][4],float freq[30])
{
    int i,j;
    float w[30];
    float temp1,temp2;
    FILE *fp1,*fp2,*fp3,*fp4;
    fp1=fopen("Slope1.txt","w+");
    fp2=fopen("Slope2.txt","w+");
    fp3=fopen("Slope3.txt","w+");
    fp4=fopen("Slope4.txt","w+");
    for(i=0;i<30;i++)
        w[i]=(6.2832*freq[i]);
    for(i=0;i<30;i++)
    {
        temp1=((y[i][1].y));
        temp2=w[i];
        fprintf(fp1,"%f\t%f\n",z[i][0].y,w[i]);
        fprintf(fp2,"%f\t%f\n",z[i][3].y,w[i]);
        fprintf(fp3,"%f\t%f\n",z[i][1].y,w[i]);
        fprintf(fp4,"%f\t%f\n",temp1,temp2);
    }
}

int main()
{
    int i,j,k;
    float arr[30][9];
    FILE *fp;
    fp=fopen("coldfet_pinchoff.txt","r");
    for(i=0;i<30;i++)
    {
        for(j=0;j<9;j++)
            fscanf(fp,"%f",&arr[i][j]);
    }
    for(i=0;i<30;i++)
    {
        printf("\n");
        for(j=0;j<9;j++)
            printf("%f\t",arr[i][j]);
    }
    for(i=0;i<30;i++)
    {
        freq[i]=arr[i][0];
    }
    for(i=0;i<30;i++)
    {
        s[i][0].x=arr[i][1];s[i][0].y=arr[i][2];
        s[i][1].x=arr[i][3];s[i][1].y=arr[i][4];
        s[i][2].x=arr[i][5];s[i][2].y=arr[i][6];
        s[i][3].x=arr[i][7];s[i][3].y=arr[i][8];
    }

    s2y(s);
    y2z(y);
    for(i=0;i<30;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",z[i][j].x,z[i][j].y);
    }
    inductance(z);
    modifiedz(z,freq);
    z2y(z);
    printf("\n\n");
    extrinsiccap(y,freq);
    modifiedy(y,freq);
    slope(y,freq);
    return 0;
}
