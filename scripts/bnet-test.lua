
local datadir = "/home/yusuke/var/srcs/magus/testdata"
local lib = new_clib(datadir .. "/library/lib2.mis2lib", "mislib")

local bnet = new_bnet()
assert(bnet:read_blif(datadir .. "/netblif/C432.netblif", {cell_library = lib}))
-- bnet:read_blif(datadir .. "/blif/C432.blif", {cell_library = lib})

bnet:write_blif("test.blif")