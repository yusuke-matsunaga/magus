
datadir = script_dir .. "/../data/"
lib = assert(magus.read_mislib(datadir .. "library/lib2.mis2lib"))
filename = datadir .. "netblif/C432.netblif"
bnet = assert(magus.read_blif(filename, {cell_library = lib}))

stats = bnet:stats()
assert( stats["input_num"] == 36 )
assert( stats["output_num"] == 7 )
assert( stats["logic_num"] == 209 )
