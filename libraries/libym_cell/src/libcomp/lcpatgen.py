#! /usr/bin/env python
# coding=UTF-8

# LcPatMgr.cc の分解パタンを作るスクリプト

import sys

# n 入力の分解パタンを列挙する．
# 一つのパタンは整数のリスト
# この関数は i0 から始まる n 個のパタンのリストを返す．
def pg_sub(n, i0) :
    if n == 1 :
        return [[ i0 ]]

    elif n == 2 :
        return [[ -1, i0, i0 + 1 ]]

    else :
        pat_list = []
        for nl in range(1, n) :
            nr = n - nl
            pat_l_list = pg_sub(nl, i0)
            pat_r_list = pg_sub(nr, i0 + nl)

            for pat_l in pat_l_list :
                for pat_r in pat_r_list :
                    pat = [ -1 ] + pat_l + pat_r
                    pat_list.append(pat)

        return pat_list

# ni 個の分解パタン用の配列データを出力する．
def lcpatgen(ni) :
    if ni < 3 :
        print "Error ni < 3";
        return

    pat_list = pg_sub(ni, 0)
    nl = len(pat_list)
    ne = ni * 2 - 1

    print "// %d入力の分解パタン" % ni
    print "const ymuint n_pat%d = %d;" % (ni, nl)
    print "int pat%d[n_pat%d][%d] = {" % (ni, ni, ne)
    for p in range(0, nl) :
        pat = pat_list[p]
        line = "\t{"
        comma = ""
        for i in range(0, ne) :
            line += comma
            line += " %d" % pat[i]
            comma =","
        line += " }"
        if p < nl - 1 :
            line += ","
        print line
    print "};"
    print


# main 関数
#
# USAGE: lcpatgen.py <最大入力数>

if len(sys.argv) != 2 :
    print "Usage: %s <最大入力数>" % sys.argv[0]
    sys.exit(1)

maxni = int(sys.argv[1]) + 1
for ni in range(3, maxni) :
    lcpatgen(ni)
