def main():
    f = open("pi_hex_1b.txt","r")
    g = open("random_pi.dat","w")

    f.read(2)

    for i in xrange(1000000000):
        c = chr(int(f.read(2),16))
        g.write(c)


main()

