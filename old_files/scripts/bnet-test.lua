
datadir = "/home/yusuke/var/srcs/magus/tests/data"
lib = read_mislib(datadir .. "/library/lib2.mis2lib")
filename = datadir .. "/netblif/C432.netblif"
print(filename)
bnet = assert(read_blif(filename, {cell_library = lib}))

bnet:write_blif()
