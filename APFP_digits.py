from APFP import *

def main():
    """Files of digits"""

    APFP.dprec = 320000

    with open("constants/decimal_sqrt5.txt","w") as f:
        f.write(str(APFP("5").sqrt()))
    with open("constants/decimal_sqrt7.txt","w") as f:
        f.write(str(APFP("7").sqrt()))
    with open("constants/decimal_sqrt10.txt","w") as f:
        f.write(str(APFP("10").sqrt()))
    with open("constants/decimal_sqrt11.txt","w") as f:
        f.write(str(APFP("11").sqrt()))
    with open("constants/decimal_sqrt13.txt","w") as f:
        f.write(str(APFP("13").sqrt()))
    with open("constants/decimal_sqrt14.txt","w") as f:
        f.write(str(APFP("14").sqrt()))
    with open("constants/decimal_sqrt15.txt","w") as f:
        f.write(str(APFP("15").sqrt()))
    with open("constants/decimal_sqrt17.txt","w") as f:
        f.write(str(APFP("17").sqrt()))
    with open("constants/decimal_sqrt18.txt","w") as f:
        f.write(str(APFP("18").sqrt()))




main()

