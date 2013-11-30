#! /usr/bin/env python
# coding=UTF-8

# 順列ごとの正規化テーブルを作るスクリプト

import sys

# サポートベクタに基づいて正規化を行う．
def perm_normalize(perm, sup_vec) :

    p0 = perm[0]
    p1 = perm[1]
    p2 = perm[2]
    p3 = perm[3]

    if sup_vec == 0 :
        # 決め打ち
        return (0, 1, 2, 3)

    pos_list = []
    tmp_list = []
    if (sup_vec & 1) == 0 :
        pos_list.append(0)
        tmp_list.append(p0)

    if (sup_vec & 2) == 0 :
        pos_list.append(1)
        tmp_list.append(p1)

    if (sup_vec & 4) == 0 :
        pos_list.append(2)
        tmp_list.append(p2)

    if (sup_vec & 8) == 0 :
        pos_list.append(3)
        tmp_list.append(p3)

    # ダミー
    np = [p0, p1, p2, p3]
    tmp_list.sort()
    n = len(pos_list)
    for i in range(n) :
        np[pos_list[i]] = tmp_list[i]

    # リストからタプルにコピーする
    # もっとうまいやり方があると思う．
    nperm = (np[0], np[1], np[2], np[3])
    return nperm

perm_list = [
    (0, 1, 2, 3),
    (0, 1, 3, 2),
    (0, 2, 1, 3),
    (0, 2, 3, 1),
    (0, 3, 1, 2),
    (0, 3, 2, 1),
    (1, 0, 2, 3),
    (1, 0, 3, 2),
    (1, 2, 0, 3),
    (1, 2, 3, 0),
    (1, 3, 0, 2),
    (1, 3, 2, 0),
    (2, 0, 1, 3),
    (2, 0, 3, 1),
    (2, 1, 0, 3),
    (2, 1, 3, 0),
    (2, 3, 0, 1),
    (2, 3, 1, 0),
    (3, 0, 1, 2),
    (3, 0, 2, 1),
    (3, 1, 0, 2),
    (3, 1, 2, 0),
    (3, 2, 0, 1),
    (3, 2, 1, 0)
]

perm_map = {}
id = 0
for perm in perm_list :
    perm_map[perm] = id
    id = id + 1

debug = 0

line = ""
for perm in perm_list :
    if line != "" :
        print "%s," % line
    line = "  {"
    comma = ""
    for sup_vec in range(0, 16) :
        nperm = perm_normalize(perm, sup_vec)
        if debug :
            sup_str = ""

            if (sup_vec & 1) :
                s0 = "1"
            else :
                s0 = "-"

            if (sup_vec & 2) :
                s1 = "1"
            else :
                s1 = "-"

            if (sup_vec & 4) :
                s2 = "1"
            else :
                s2 = "-"

            if (sup_vec & 8) :
                s3 = "1"
            else :
                s3 = "-"

            print perm, "%s%s%s%s" % (s0, s1, s2, s3), " ==> ", nperm
        else :
            line += comma
            line += "%d" % perm_map[nperm]
            comma = ", "
    if not debug :
        line += "}"

print line
