import YmUtils

filename = "test.dat"

odo = YmUtils.FileODO()
if not odo.open(filename) :
  print "Could not open {0}".format(filename)

odo.write_32(1234)
odo.write_double(0.123456)

odo.close()


ido = YmUtils.FileIDO()
if not ido.open(filename) :
  print "Could not open {0}".format(filename)

val1 = ido.read_32()
val2 = ido.read_double()

ido.close()

print val1
print val2
