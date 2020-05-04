#include <stdio.h>
#include <conio.h>

float freq[30];
float cb[30],rg[30],rs[30],rd[30],w[30],rgd[30]; //Extrinsic Parameter
float lg[30],ls[30],ld[30];
float cgd[30],cgs[30],ri[30],gm[30],tau[30],cds[30],cdc[30],rds[30]; //Intrinsic Parameter
float vgs,vth,rch;
float cs[30],cg[30],cd[30];

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

float mod (num number)
{
    float temp;
    temp=(number.x*number.x)+(number.y*number.y);
    temp=sqrt(temp);
    return temp;
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

void extrinsiccap(num y[30][4],float freq1[30])
{
    int i,j;
    float temp1,temp2;
    num t1,t2,t3;
    float cpd = 0.012838,cgsi=0.193435,cgdi=0.1152382978,cdsi=0.009828,cpg=0.0084347;
    for(i=0;i<30;i++)
        w[i]=6.2832*freq1[i];
    for(i=1;i<30;i++)
    {
        cgd[i]=(((-1)*y[i][1].y)/w[i]);
        cds[i]=(y[i][3].y/w[i]);
        //cds[i]=cds[i];
        cgs[i]=(y[i][0].y/w[i]);
        printf("\n\n Cgd = %f \t\t Cds = %f \t\t Cgs = %f\n",cgd[i],cds[i],cgs[i]);
    }
}

void modifiedy(num y[30][4],float freq1[30])
{
    int i,j;
    float cpd = 0.012838,cgsi=0.193435,cgdi=0.1152382978,cdsi=0.009828,cpg=0.0084347;
    for(i=0;i<30;i++)
        w[i]=6.2832*freq1[i];
    for(i=0;i<30;i++)
    {
        y[i][0].y=y[i][0].y-(w[i]*0.296*0.001);
       // y[i][1].y=y[i][1].y+(w[i]*cgdi);
       // y[i][2].y=y[i][2].y+(w[i]*cgdi);
        y[i][3].y=y[i][3].y-(w[i]*0.01712*0.001);
    }
}

void modifiedy1(num y[30][4],float freq1[30])
{
    int i,j;
    float cpd = 0.012838,cgsi=0.193435,cgdi=0.1152382978,cdsi=0.009828,cpg=0.0084347;
    for(i=0;i<30;i++)
        w[i]=6.2832*freq1[i];
    for(i=0;i<30;i++)
    {
        y[i][0].y=y[i][0].y-(w[i]*0.0674*0.001);
        y[i][1].y=y[i][1].y+(w[i]*0.057*0.001);
        y[i][2].y=y[i][2].y+(w[i]*0.057*0.001);
        y[i][3].y=y[i][3].y-(w[i]*0.1981*0.001);
    }
}


void extrinsic(num z[30][4],float freq1[30])
{
    int i,j;
    float temp1,temp2;
    float slopee[30];
    for(i=0;i<30;i++)
        w[i]=6.2832*freq1[i];
    for(i=1;i<30;i++)
    {
        temp1=((z[i][1].y)/w[i])-(z[i-1][1].y/w[i-1]);

        temp2=(1/(w[i]*w[i]))-(1/(w[i-1]*w[i-1]));
        slopee[i]=temp2/temp1;
        cs[i]=(-1)*slopee[i];

        temp1=((z[i][0].y)/w[i])-(z[i-1][0].y/w[i-1]);
        temp2=(1/(w[i]*w[i]))-(1/(w[i-1]*w[i-1]));
        cg[i]=(-1)*(temp1/temp2);
        cg[i]=cg[i]-(1/cs[i]);
        cg[i]=(1/cg[i]);

        temp1=((z[i][3].y)/w[i])-(z[i-1][3].y/w[i-1]);
        temp2=(1/(w[i]*w[i]))-(1/(w[i-1]*w[i-1]));
        cd[i]=(-1)*(temp1/temp2);
        cd[i]=cd[i]-(1/cs[i]);
        cd[i]=(-1)*(1/cd[i]);


        temp1=((z[i][1].y)*w[i])-(z[i-1][1].y*w[i-1]);
        temp2=((w[i]*w[i]))-((w[i-1]*w[i-1]));
        ls[i]=temp1/temp2;

        temp1=((z[i][0].y)*w[i])-(z[i-1][0].y*w[i-1]);
        temp2=((w[i]*w[i]))-((w[i-1]*w[i-1]));
        lg[i]=(temp1/temp2)-(ls[i]);

        temp1=((z[i][3].y)*w[i])-(z[i-1][3].y*w[i-1]);
        temp2=((w[i]*w[i]))-((w[i-1]*w[i-1]));
        ld[i]=(temp1/temp2)-(ls[i]);
        printf("\n\n Cs[%d] = %f\tCg[%d] = %f \t Cd[%d] = %f\tLs[%d] = %.10lf\t Lg[%d] = %.8lf \t Ld[%d] = %.8lf\n",i,cs[i],i,cg[i],i,cd[i],i,ls[i],i,lg[i],i,ld[i]);
        //printf("Cs[%d] = %f\tCg[%d] = %f\tCd[%d] = %f\tLs[%d] = %0.7lf\n",i,cs[i],i,cg[i],i,cd[i],i,ls[i]);
    }
}

void modifiedz(num z[30][4])
{
    int i,j;
    for(i=0;i<30;i++)
    {
        //z[i][0].x=z[i][0].x-rs[i]-rg[i];
        z[i][0].y=z[i][0].y-(w[i]*(lg[i]+ls[i]));
        //z[i][1].x=z[i][1].x-rs[i];
        z[i][1].y=z[i][1].y-(w[i]*ls[i]);
        //z[i][2].x=z[i][2].x-rs[i];
        z[i][2].y=z[i][2].y-(w[i]*ls[i]);
        //z[i][3].x=z[i][3].x-rs[i]-rd[i];
        z[i][3].y=z[i][3].y-(w[i]*(ls[i]+lg[i]));
    }
   // printf("\n\nZ Matrix for Intrinsic Parameter :\n");
    for(i=0;i<30;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",z[i][j].x,z[i][j].y);
    }
}

void modifiedz1(num z[30][4])
{
    int i,j;
    for(i=0;i<30;i++)
    {
        z[i][0].x=z[i][0].x-(rs[i]+rg[i]);
        //z[i][0].y=z[i][0].y-(w[i]*(0.00811));
        z[i][1].x=z[i][1].x-rs[i];
        //z[i][1].y=z[i][1].y-(w[i]*0.00811);
        z[i][2].x=z[i][2].x-rs[i];
        //z[i][2].y=z[i][2].y-(w[i]*0.00811);
        z[i][3].x=z[i][3].x-(rd[i]+rs[i]);
        //z[i][3].y=z[i][3].y-(w[i]*(0.00811));
    }
    printf("\n\nZ Matrix for Intrinsic Parameter :\n");
    for(i=0;i<30;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",z[i][j].x,z[i][j].y);
    }
}


void extrinsicresist(num z[30][4],float freq1[30])
{
    int i,j;
    float temp1,temp2;
    float slopee[30];
    for(i=0;i<30;i++)
        w[i]=6.2832*freq1[i];
    for(i=10;i<30;i++)
    {
        rs[i]=z[i][1].x;
        rg[i]=6.9;
        rd[i]=2.2159418+rs[i];
        printf("\nRs[%d] = %f\tRg[%d] = %f\tRd[%d] = %f",i,rs[i],i,rg[i],i,rd[i]);

    }
}

void intrinsic(num y[17][4])
{
    int i;
    float cgsarg,denri,gmt,gds[17];
    printf("\n\nIntrinsic Elements : \n");
    for(i=1;i<10;i++)
    {
        cgd[i]=(-1)*(y[i][1].y/w[i]);
        cgsarg=pow((y[i][0].y-(w[i]*cgd[i])),2);
        cgs[i]=(y[i][0].y-(w[i]*cgd[i]))*((1+((y[i][0].x*y[i][0].x)/cgsarg))/w[i]);
        denri=(y[i][0].y-(w[i]*cgd[i]))*(y[i][0].y-w[i]*cgd[i])+y[i][0].x*y[i][0].x;
        ri[i]=(y[i][0].x/denri);
        gmt=y[i][2].y+(w[i]*cgd[i]);
        gm[i]=sqrt((y[i][2].x*y[i][2].x+gmt*gmt)*(1.0+w[i]*w[i]*cgs[i]*cgs[i]*ri[i]*ri[i]));
        tau[i]=asin((-w[i]*cgd[i]-y[i][2].y-w[i]*cgs[i]*ri[i]*y[i][2].x)/gm[i])/w[i];
        cds[i]=(y[i][3].y-(w[i]*cgd[i]))/w[i];
        cdc[i]=cgs[i]*y[i][1].x/y[i][0].x;
        cds[i]=cds[i]-cdc[i];
        rds[i]=(1/y[i][3].x);
        //rds[i]=(1/gds[i]);
        printf("\n\n Cgd(nF) = %f\t\tCgs(nF) = %f\t\tRi(ohm) = %f\t\tgm(S) = %f\t\tTau = %f\t\tCds(nF) = %f\t\tRds(ohm) = %f",cgd[i],cgs[i],ri[i],gm[i],tau[i],cds[i],rds[i]);
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
        temp1=(y[i][0].x);
        temp2=w[i];
        fprintf(fp1,"%f\t%f\n",y[i][0].y,w[i]);
        fprintf(fp2,"%f\t%f\n",y[i][3].y,w[i]);
        fprintf(fp3,"%f\t%f\n",y[i][1].y,w[i]);
        fprintf(fp4,"%f\t%f\n",temp1,temp2);
    }
}

int main()
{
    int i,j,k;
    float arr[30][9];
    FILE *fp;

    /********* Section 1 *********/
    fp=fopen("coldfet2.txt","r");
    for(i=0;i<30;i++)
    {
        for(j=0;j<9;j++)
            fscanf(fp,"%f",&arr[i][j]);
    }
    printf("COLDFET TEXT READ DATA\n");
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
    printf("\n\nY-matrix\n");
    for(i=0;i<30;i++)
    {
        printf("\n");
        for(j=0;j<4;j++)
            printf("%f + i %f\t\t",y[i][j].x,y[i][j].y);
    }

    slope(y,freq);
    extrinsiccap(y,freq);
    fclose(fp);
    //modifiedy(y,freq);

    /********* Section 2 *********/
    fp=fopen("unbiased.txt","r");
    for(i=0;i<30;i++)
    {
        for(j=0;j<9;j++)
            fscanf(fp,"%f",&arr[i][j]);
    }
    printf("\nUNBAISED TEXT READ DATA\n");
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
    extrinsic(z,freq);
    extrinsicresist(z,freq);
    fclose(fp);

    /********* Section 3 *********/
    fp=fopen("hotfet2.txt","r");
    for(i=0;i<30;i++)
    {
        for(j=0;j<9;j++)
            fscanf(fp,"%f",&arr[i][j]);
    }
    printf("\n\nHOTFET TEXT READ DATA\n");
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
    printf("\nS Matrix (HOTFET)\n\n");
    for(i=0;i<4;i++)
    {
        printf("%f+i%f\n",s[4][i].x,s[4][i].y);
    }
    printf("\nY Matrix (HOTFET)\n\n");
    for(i=0;i<4;i++)
    {
        printf("%f+i%f\n",y[4][i].x,y[4][i].y);
    }
    modifiedy(y,freq);
    printf("\nY Matrix (HOTFET)\n\n");
    for(i=0;i<4;i++)
    {
        printf("%f+i%f\n",y[4][i].x,y[4][i].y);
    }
    y2z(y);
    printf("\nZ Matrix (HOTFET)\n\n");
    for(i=0;i<4;i++)
    {
        printf("%f+i%f\n",z[4][i].x,z[4][i].y);
    }
    modifiedz(z);
    z2y(z);
    modifiedy1(y,freq);
    y2z(y);
    modifiedz1(z);
    z2y(z);
    intrinsic(y);
    return 0;
}
