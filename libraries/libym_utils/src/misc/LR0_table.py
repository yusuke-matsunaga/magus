# coding=UTF-8

from grammer import *

# LR(0)構文解析表を作るモジュール
#
# - LR(0) 項は (rule_id, pos) という2つ組で表す．

class LR0_table :
    # コンストラクタ
    def __init__(self) :
        self.clear()

    # 内容をクリアする．
    def clear(self) :

        # 状態リスト
        # 状態とは項の集合
        self._StateList = []

        # ACTION マップ
        self._ActionMap = []

    # @brief LR(0)構文解析表を作る．
    def make_table(self, grammer) :
        self.clear()
        self._StateList = LR0_state_list(grammer)
        for state in self._StateList :
            state_id = self.state2id(state)
            self._ActionMap.append({})

            for (rule_id, pos) in state :
                (left, right) = grammer.id2rule(rule_id)
                if len(right) > pos :
                    # 次のトークンを読む．
                    token_id = right[pos]
                    tmp_state = LR0_next_state(grammer, state, token_id)
                    if grammer._TerminalList[token_id] :
                        next_id = self.state2id(tmp_state)
                        assert next_id != -1
                        if not self.set_action(state_id, token_id, 'shift', next_id) :
                            print "Error: not an LR(0)"
                    else :
                        if len(tmp_state) > 0 :
                            next_id = self.state2id(tmp_state)
                            if not self.set_action(state_id, token_id, 'goto', next_id) :
                                print "Error: not an LR(0)"
                else :
                    # 末尾の場合
                    if left == grammer._StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0) :
                            print "Error: not an LR(0)"
                    else :
                        # FOLLOW を調べる．
                        for token_id in grammer._FollowMap[left] :
                            if not self.set_action(state_id, token_id, 'reduce', rule_id) :
                                print "Error: not an LR(0)"

    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id) :
        if self._ActionMap[state_id].has_key(token_id) :
            if self._ActionMap[state_id][token_id] != (action, action_id) :
                return False
            else :
                return True
        else :
            self._ActionMap[state_id][token_id] = (action, action_id)
            return True

    # @brief LR(0)状態リストを表示する．
    def print_states(self, grammer) :
        for state in self._StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            LR0_print_terms(grammer, state)
            print ""
            for key in self._ActionMap[state_id].keys() :
                (action, action_id) = self._ActionMap[state_id][key]
                token = grammer.id2token(key)
                print '  %s: %s %d' % (token, action, action_id)
        print ''

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state_id = 0
        for state1 in self._StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1

if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    id = g.add_token('id')
    plus = g.add_token('+')
    times = g.add_token('*')
    lpar = g.add_token('(')
    rpar = g.add_token(')')
    expr = g.add_token('expr')
    term = g.add_token('term')
    factor = g.add_token('factor')

    g.add_rule(expr, (expr, plus, term))
    g.add_rule(expr, (term, )) # (term) ではタプルにならない

    g.add_rule(term, (term, times, factor))
    g.add_rule(term, (factor, )) # (factor) ではタプルにならない

    g.add_rule(factor, (lpar, expr, rpar))
    g.add_rule(factor, (id, )) # (id) ではタプルにならない

    g.set_start(expr)

    g.print_rules()

    g.print_tokens()

    lr0_tab = LR0_table()

    lr0_tab.make_table(g)

    lr0_tab.print_states(g)

    state_list = LR0_kernel_list(g)
    LR0_print_states(g, state_list)
    for state in state_list :
        print "State#%d" % id
        LR0_print_terms(g, state)
        print ""
        id += 1
