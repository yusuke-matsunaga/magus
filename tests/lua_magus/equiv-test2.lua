
datadir = script_dir .. "/../data/blif/"

filename1 = datadir .. "C499.blif"
net1 = assert(magus.read_blif(filename1))

filename2 = datadir .. "C499_reordered.blif"
net2 = assert(magus.read_blif(filename2))

ok = magus.check_equiv(net1, net2, {match_by_name = true})
assert( ok )
