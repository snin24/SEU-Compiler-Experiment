'''
包含多个字符的运算符。
ASSIGN  :=
RELOP   <>
RELOP   >=
RELOP   <=
我们顺便把以下单字符给识别了
RELOP   <
RELOP   >


函数返回'类别，符号，下一个位置'
'''
def recognize_multi_operator(src, pos):
    if not 0 <= pos < len(src):
        return None

    now = src[pos]
    following = src[pos+1:pos+2]

    if now == ":":
        if following=="=":
            return 'ASSIGN',':=',pos+2
        return None

    if now =='<':
        if following=='>':
            return 'RELOP','<>',pos+2
        elif following=='=':
            return 'RELOP','<=',pos+2
        return 'RELOP','<',pos+1
    if now=='>':
        if following=='=':
            return 'RELOP','>=',pos+2
        return 'RELOP','>',pos+1

    return None

