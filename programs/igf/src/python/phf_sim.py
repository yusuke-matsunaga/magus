# coding=UTF-8

# PHF/IGU をランダムシミュレーションプログラム

import sys
import utils
import igf


# ランダムな関数ベクタを生成する．
def gen_random_func(input_size, output_range, randgen ) :
    fv = igf.FuncVect(input_size, output_range)
    for i in range(0, input_size) :
        oval = randgen.int32() % output_range
        fv.set_val(i, oval)

    return fv


# PHF で構成できるかチェックする．
def check_phf_cond(func_list) :
    pg = igf.PhfGen()

    ans = pg.mapping(func_list)
    if ans is None :
        return False
    return True

# CF分割できるかチェックする．
def check_cfp_cond(func_list) :
    pg = igf.PhfGen()

    ans = pg.cf_partition(func_list)
    if ans is None :
        return False
    return True

# RowShift分解できるかチェックする．
def check_rsd_cond(func1, func2, use_xor) :
    pg = igf.PhfGen()

    ans = pg.displace_decomposition(func1, func2, use_xor)
    if ans is None :
        return False
    return True

# ランダムサンプリングを用いて成功確率を求める．
def sampling(input_size, output_range, m, n) :
    c1 = 0
    c2 = 0
    for i in range(0, n) :
        func_list = []
        for j in range(0, m) :
            fv = gen_random_func(input_size, output_range, randgen)
            func_list.append(fv)

        if check_phf_cond(func_list) :
            c1 = c1 + 1

        if check_cfp_cond(func_list) :
            c2 = c2 + 1

    return (c1, c2)


if len(sys.argv) != 6 :
    print "Usage: {} <k1> <k2> <d> <p> <m>".format(sys.argv[0])
    sys.exit(1)

k1 = int(sys.argv[1])
k2 = int(sys.argv[2])
d = int(sys.argv[3])
p = int(sys.argv[4])
m = int(sys.argv[5])

randgen = utils.RandGen()

n = 10000
for input_size in range(k1, k2, d) :
    (c1, c2) = sampling(input_size, p, m, n)
    d1 = float(c1) / n
    d2 = float(c2) / n
    print "{} {} {}".format(input_size, d1, d2)
