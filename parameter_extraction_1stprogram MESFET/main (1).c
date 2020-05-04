#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

int freq[17];
float cb[17],cpg[17],cpd[17],rg[17],rs[17],rd[17],lg[17],ls[17],ld[17],w[17]; //Extrinsic Parameter
float cgd[17],cgs[17],ri[17],gm[17],tau[17],cds[17],cdc[17],rds[17]; //Intrinsic Parameter
float vgs,vth,rch;

typedef struct num
{
    float x,y;
}num;

num s[17][4]; //Global Declaration
num y[17][4]; //Global Declaration
num z[17][4]; //Global Declaration
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
num y2z(num y[17][4])
{
    num t0,t1,t2,t3,c1;
    num delta;
    int i;
    c1.x=(1.000000);
    c1.y=0.000000;
    c1.x=(-1)*c1.x;
    for(i=0;i<17;i++)
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
num z2y(num z[17][4])
{
    num t0,t1,t2,t3,c1;
    num delta;
    int i;
    c1.x=(1.000000);
    c1.y=0.000000;
    c1.x=(-1)*c1.x;
    for(i=0;i<17;i++)
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

num *s2y(num s[17][4])
{
    num c1,delta,two;
    num d1,d2,d3,ds1,ds2,ay11,ay12,ay21,ay22,s21,h21,h22;
    num t1,t2,t3,t0;
    int i;
    //num y[17][4];
    /*for(i=0;i<17;i++)
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
    for(i=0;i<17;i++)
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

void modifiedy(num y[17][4],float cpg[17],float cpd[17],float narr[17][9])
{
    int i,j;
    for(i=0;i<17;i++)
    {
        y[i][0].y=y[i][0].y-(w[i]*cpg[i]);
        y[i][3].y=y[i][3].y-(w[i]*cpd[i]);
    }
}

void extrinsiccap(num y[17][4],float narr[17][9])
{
    int i,j;
    float temp,gradient[17];
    num t1,t2,t3;
    for(i=0;i<17;i++)
    {
        freq[i]=narr[i][0];
        w[i]=(2*3.1412*freq[i]);
    }
    for(i=1;i<17;i++)
    {
        temp=(y[i][1].y-y[i-1][1].y);
        gradient[i]=temp/(w[i]-w[i-1]);
        cb[i]=gradient[i]/3;
        temp=(y[i][3].y-y[i-1][3].y);
        gradient[i]=temp/(w[i]-w[i-1]);
        cpd[i]=gradient[i]-((2/3)*cb[i]);
        temp=(y[i][0].y-y[i-1][0].y);
        gradient[i]=temp/(w[i]-w[i-1]);
        cpg[i]=gradient[i]-((2/3)*cb[i]);
    }
    printf("\n\nExtrinsic Capacitance : Cb,Cpg,Cpd\n");
    for(i=0;i<17;i++)
    {
        printf("\nCb(Depletion Cap.) = %f pF;\t\tCpg(Gate Pad Cap.) = %f pF;\t\tCpd(Drain Pad Cap.) = %f pF\n",cb[i],cpg[i],cpd[i]);
    }
}

void extrinsicinduct(num z[17][4])
{
    int i;
    float temp,gradient[17];
    num t;
    printf("\n\nExtrinsic Inductance : Ls,Lg,Ld\n");
    for(i=0;i<17;i++)
    {
        temp=(z[i][1].y-z[i-1][1].y);
        gradient[i]=temp/(w[i]-w[i-1]);
        ls[i]=gradient[i];
       // ls[i]=(z[i][2].y/w[i]);
        temp=(z[i][3].y-z[i-1][3].y);
        gradient[i]=temp/(w[i]-w[i-1]);
        ld[i]=gradient[i]-ls[i];
        temp=(z[i][0].y-z[i-1][0].y);
        gradient[i]=temp/(w[i]-w[i-1]);
        lg[i]=gradient[i]-ls[i];
        printf("\nLs(Source Insuctance)=%f nH;\t\tLg(Gate Inductance)=%f nH;\t\tLd(Drain Inductance)=%f nH;\n",ls[i],lg[i],ld[i]);
    }
}

void extrinsicresist(num z[17][4])
{
    int i;
    /*for(i=0;i<17;i++)
    {
        printf("\n%f",w[i]);
    }*/
    printf("\n\n\nEnter the value for Gate to Source Voltage(Vgs)(V):");
    scanf("%f",&vgs);
    printf("\nEnter the value for Threshold Voltage(Vth)(V):");
    scanf("%f",&vth);
    rch=(1/(vgs-vth));
    for(i=0;i<17;i++)
    {
        rs[i]=(z[i][2].x-(rch/2));
        rd[i]=(-1)*((z[i][3].x)-rs[i]-rch);
        rg[i]=(-1)*((z[i][0].x)-rs[i]-(rch/3));
        printf("\nRs(Source Resistance)=%f ohm;\t\tRg(Gate Resistance)=%f ohm;\t\tRd(Drain Resistance)=%f ohm;\n",rs[i],rg[i],rd[i]);
    }
}

void modifiedz(num z[17][4])
{
    int i,j;
    for(i=0;i<17;i++)
    {
        z[i][0].x=z[i][0].x-rs[i]-rg[i];    z[i][0].y=z[i][0].y-(w[i]*(ls[i]+lg[i]));
        z[i][1].x=z[i][1].x-rs[i];          z[i][1].y=z[i][1].y-(w[i]*ls[i]);
        z[i][2].x=z[i][2].x-rs[i];          z[i][2].y=z[i][2].y-(w[i]*ls[i]);
        z[i][3].x=z[i][3].x-rs[i]-rd[i];    z[i][3].y=z[i][3].y-(w[i]*(ls[i]+ld[i]));
    }
    printf("\n\nZ Matrix for Intrinsic Parameter :\n");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",z[i][j].x,z[i][j].y);
    }
}

void intrinsic(num y[17][4])
{
    int i;
    float cgsarg,denri,gmt,gds[17];
    printf("\n\nIntrinsic Elements : \n");
    FILE *fptr;
    fptr = fopen("G:\\intrinsic.txt","w");
    for(i=0;i<17;i++)
    {
        /*Cgd=-y[1].y/w;

        cgsarg=(y[0].y-w*Cgd)*(y[0].y-w*Cgd);
        Cgs=(y[0].y-w*Cgd)*(1.+y[0].x*y[0].x/cgsarg)/w;

        denri=(y[0].y-w*Cgd)*(y[0].y-w*Cgd)+y[0].x*y[0].x;
        Ri=y[0].x/denri;
        printf("\nRi=%f",Ri);
        gma1=y[2].y+w*Cgd;
        gm=sqrt((y[2].x*y[2].x+gma1*gma1)*(1.+w*w*Cgs*Cgs*Ri*Ri));
        tau=asin((-w*Cgd-y[2].y-w*Cgs*Ri*y[2].x)/gm)/w;
        Cds=(y[3].y-w*Cgd)/w;
        Cdc=Cgs*y[1].x/y[0].x;Cds=Cds-Cdc;
        gds=y[3].x;Rds=1./gds*/

        cgd[i]=(-1)*(y[i][1].y/w[i]);
        cgsarg=pow((y[i][0].y-(w[i]*cgd[i])),2);
        cgs[i]=(-1)*(y[i][0].y-(w[i]*cgd[i]))*((1+((y[i][0].x*y[i][0].x)/cgsarg))/w[i]);
        denri=(y[i][0].y-(w[i]*cgd[i]))*(y[i][0].y-(w[i]*cgd[i]))+(y[i][0].x*y[i][0].x);
        ri[i]=(-1)*(y[i][0].x/denri);
        gmt=y[i][2].y+(w[i]*cgd[i]);
        gm[i]=sqrt((y[i][2].x*y[i][2].x+gmt*gmt)*(1.0+w[i]*w[i]*cgs[i]*cgs[i]*ri[i]*ri[i]));
        tau[i]=(-1)*asin((-w[i]*cgd[i]-y[i][2].y-w[i]*cgs[i]*ri[i]*y[i][2].x)/gm[i])/w[i];
        cds[i]=(y[i][3].y-(w[i]*cgd[i]));
        cdc[i]=cgs[i]*y[i][1].x/y[i][0].x;
        cds[i]=cdc[i]-cds[i];
        rds[i]=(-1 )*(1/y[i][3].x);
        //rds[i]=(1/gds[i]);
        printf("\n\n Cgd = %f\t\tCgs = %f\t\tRi = %f\t\tgm = %f\t\tTau = %f\t\tCds = %f\t\tRds = %f",cgd[i],cgs[i],ri[i],gm[i],tau[i],cds[i],rds[i]);

        fprintf(fptr,"\n\n Cgd = %f\t\tCgs = %f\t\tRi = %f\t\tgm = %f\t\tTau = %f\t\tCds = %f\t\tRds = %f",cgd[i],cgs[i],ri[i],gm[i],tau[i],cds[i],rds[i]);
    }
}

void slope(num y[17][4],float narr[17][9])
{
    int i;
    FILE *fp1;
    fp1=fopen("Finding_Slope2.txt","w+");
    float temp1,temp2,slopee[40];
    for(i=0;i<17;i++)
        w[i]=6.2832*narr[i][0];
    for(i=1;i<17;i++)
    {
        temp1=y[i][1].y;
        temp2=w[i];
        slopee[i]=temp2/temp1;
        fprintf(fp1,"\n\n%f \t %f\t %f\n",temp2,temp1,slopee[i]);
    }
}


int main()
{
    int freq[17];
    float arr[17][9],narr[17][9];
    int i,j,k;
    num *Y;
    FILE *fp,*fp1;
    fp=fopen("coldfet_pinchoff_sparameter.txt","r");
    fp1=fopen("UMS S parameters_HOT.txt","r");
    for(i=0;i<=16;i++)
    {
        for(j=0;j<=8;j++)
            fscanf(fp,"%f",&arr[i][j]);
    }
    for(i=0;i<17;i++)
    {
        for(j=0;j<9;j++)
            narr[i][j]=arr[i][j];
    }
    for(i=0;i<=16;i++)
    {
        printf("\n");
        for(j=0;j<=8;j++)
            printf("%f\t",narr[i][j]);
    }
    for(i=0;i<17;i++)
    {
        for(j=0;j<9;j++)
        {
            freq[i]=arr[i][j];
            j=8;
        }
    }
    printf("\n");
    //for(i=0;i<17;i++)
    //    printf("%d",freq[i]);

    /********* Arranging Text File *********/
    for(i=0;i<17;i++)
    {
        narr[i][2]=arr[i][4];
        narr[i][3]=arr[i][2];
        narr[i][4]=arr[i][3];
        narr[i][5]=arr[i][7];
        narr[i][6]=arr[i][5];
        narr[i][7]=arr[i][8];
        narr[i][8]=arr[i][6];
    }

    /*************** Degree to Radian Conversion ***************/
    for(i=0;i<17;i++)
    {
        for(j=5;j<9;j++)
            narr[i][j]=(narr[i][j]*0.0174533);
    }
    printf("\nConverted Angle ...........\n");
    for(i=0;i<=16;i++)
    {
        printf("\n");
        for(j=0;j<=8;j++)
            printf("%f\t",narr[i][j]);
    }
    for(i=0;i<17;i++)
    {
        k=0;
        for(j=1;j<=4;j++)
        {
            s[i][k].x=(narr[i][j]*cos(narr[i][j+4]));
            s[i][k].y=(narr[i][j]*sin(narr[i][j+4]));
            k++;
        }
    }
    printf("\n\n S Matrix \n\n");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",s[i][j].x,s[i][j].y);
    }

    printf("\n");
    s2y(s);

    printf("\n\nY Matrix");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",y[i][j].x,y[i][j].y);
    }
    y2z(y);
    printf("\n\nZ Matrix");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",z[i][j].x,z[i][j].y);
    }
    extrinsiccap(y,narr);
    slope(y,narr);
    modifiedy(y,cpg,cpd,narr);
    printf("\n\nModified Y-Matrix");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",y[i][j].x,y[i][j].y);
    }
    y2z(y);
    printf("\n\nModified Z-Matrix");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",z[i][j].x,z[i][j].y);
    }
    extrinsicinduct(z);
    extrinsicresist(z);

    /********* We have to eliminate extrinsic elements to determine intrinsic elements *********/

    for(i=0;i<=16;i++)
    {
        for(j=0;j<=8;j++)
            fscanf(fp1,"%f",&arr[i][j]);
    }
    for(i=0;i<17;i++)
    {
        for(j=0;j<9;j++)
            narr[i][j]=arr[i][j];
    }
    for(i=0;i<=16;i++)
    {
        printf("\n");
        for(j=0;j<=8;j++)
            printf("%f\t",narr[i][j]);
    }
    for(i=0;i<17;i++)
    {
        for(j=0;j<9;j++)
        {
            freq[i]=arr[i][j];
        }
    }
    for(i=0;i<17;i++)
    {
        narr[i][2]=arr[i][4];
        narr[i][3]=arr[i][2];
        narr[i][4]=arr[i][3];
        narr[i][5]=arr[i][7];
        narr[i][6]=arr[i][5];
        narr[i][7]=arr[i][8];
        narr[i][8]=arr[i][6];
    }
    for(i=0;i<17;i++)
    {
        for(j=5;j<9;j++)
            narr[i][j]=(narr[i][j]*0.0174533);
    }
    printf("\nConverted Angle ...........\n");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<9;j++)
            printf("%f\t",narr[i][j]);
    }
    for(i=0;i<17;i++)
    {
        k=0;
        for(j=1;j<=4;j++)
        {
            s[i][k].x=(narr[i][j]*cos(narr[i][j+4]));
            s[i][k].y=(narr[i][j]*sin(narr[i][j+4]));
            k++;
        }
    }
    printf("\n\n S Matrix \n\n");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",s[i][j].x,s[i][j].y);
    }

    printf("\n");
    s2y(s);
    y2z(y);
    modifiedz(z);
    z2y(z);
    printf("\n\nModified Y-Matrix\n");
    for(i=0;i<17;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",y[i][j].x,y[i][j].y);
    }

    intrinsic(y);
    fclose(fp);
    return 0;
}
