#! /usr/bin/env python
# coding=UTF-8

# 順列ごとの正規化テーブルを作るスクリプト

import sys

# サポートベクタに基づいて正規化を行う．
def perm_normalize(perm, support) :

    if support == 0 :
        # 決め打ち
        return (0, 1, 2, 3)

    if support == 1 :
        p1 = perm[1]
        p2 = perm[2]
        p3 = perm[3]
        if p1 > p2 :
            tmp = p1
            p1 = p2
            p2 = tmp
        if p2 > p3 :
            tmp = p2
            p2 = p3
            p3 = tmp
        if p1 > p2 :
            tmp = p1
            p1 = p2
            p2 = tmp
        return (perm[0], p1, p2, p3)

    if support == 2 :
        p2 = perm[2]
        p3 = perm[3]
        if p2 > p3 :
            tmp = p2
            p2 = p3
            p3 = tmp
        return (perm[0], perm[1], p2, p3)

    if support == 3 :
        return (perm[0], perm[1], perm[2], perm[3])

    if support == 4 :
        return perm

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

for perm in perm_list :
    line = ""
    comma = ""
    for sup in range(0, 4) :
        nperm = perm_normalize(perm, sup)
        if debug :
            print perm, sup, " ==> ", nperm
        else :
            line += comma
            line += "%d" % perm_map[nperm]
            comma = ", "
    if not debug :
        line += ","
        print line
