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
            fv = gen_random_func(k, p, randgen)
            func_list.append(fv)

        if check_phf_cond(func_list) :
            c1 = c1 + 1

        if check_cfp_cond(func_list) :
            c2 = c2 + 1

    return (c1, c2)


if len(sys.argv) != 4 :
    print "Usage: {} <k> <p> <m>".format(sys.argv[0])
    sys.exit(1)

k = int(sys.argv[1])
p = int(sys.argv[2])
m = int(sys.argv[3])

randgen = utils.RandGen()

n = 2000
(c1, c2) = sampling(k, p, m, n)

print "Total {} trials".format(n)
print " # of PHF:  {}".format(c1)
print " # of CFP:  {}".format(c2)
