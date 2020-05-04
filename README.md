# Parameter-Extraction
DRDO Internship project of SOFTWARE DEVELOPMENT FOR PARAMETER EXTRACTION OF GaN HEMT and GaAS MESFET

**Model description**

- **Reading of text file using file handling**

In this module we have read all the input file accordingly and store values in different variables in order to use it in later module.

This file handling part is done with the help of an inbuilt function _fscanf( )._

- **Data Structure:Structure num**

A sturucure is created using struct keyword and named num, in that structure there is two floating type variable x and y where x contain the real valur and yu conatain the imaginary part.

struct num

{

float x,y;

}

- **Add( ),Multi( ),Sub( ),Divi( ),Conj( ),Mod( )**

This module will add,subtract,divide,conjugate the complex number given to them as a parameter .

- **Y2Z( ), Z2Y( ), S2Y( )**

In this module the parameter conversion occur as the name indicate itself Y2Z convert Y parameter to Z with y parameter as formal arguments, similarly Z22 convert Z parameter to Y with z parameter as formal argument and S2Y convert S parameter to Y with s parameter as formal argument.

- **Extrinsiccap( )**

In this module, the extrinsic capacitance is calculated with the help of certain formula. This module takes Y parameter and frequency as formal argument.

- **Modifiedy**** ( )**

In this module the Y parameter, which has been calculated earlier, is modified accordingly.

- **Extrinsic** ( )

Under this function all other parameter such as Ld,Lg,Ls,Cs,Cd,Cg are calculated and printed in the order. This module takes Z parameter and frequency as formal argument.

- **Modifiedz( )**

As the name indicate this module, modify the Z parameter which has been calculated earlier and that modified Z parameter is used in further module.

- **Extrinsicresist** ( )

Under this module, the extrinsic resistance is calculated with the help of Z parameter and frequency as formal argument.

- **Intrinsic( )**

In this module, the intrinsic elements of the device is calculated. Intrinsic elements like Cgd(nF),Cgs(nF) ,Ri(ohm),Gm(S),Tau ,Cds(nF),Rds(ohm).

- **Main( )**

This is the most important module of the program as it is the driver&#39;s code of the program, which determines the order of calling of all the other modules.
