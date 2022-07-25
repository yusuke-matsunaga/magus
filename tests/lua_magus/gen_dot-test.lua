local datadir = script_dir .. "/../data/"
local filename = datadir .. "truth/ex00.truth"
local f = assert(io.open(filename, "r"))

local mgr = magus.new_bddmgr()
local bdd_list = {}
local i = 1
for line in f:lines() do
   local bdd = assert(mgr:from_truth(line))
   bdd_list[i] = bdd
   i = i + 1
end

local attr = {}
attr["graph:bgcolor"] = "yellow"
attr["0edge:color"] = "blue"
attr["1edge:color"] = "red";
attr["node:color"] = "green";
attr["node:style"] = "filled";
attr["terminal:style"] = "filled";
attr["terminal0:color"] = "blue";
attr["terminal1:color"] = "red";

magus.gen_dot_for_bdds(bdd_list, "ex00.dot", attr)