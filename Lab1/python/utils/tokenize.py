from .error import LexicalError
from .keywords_and_id import recognize_keys_and_ids
from .multi_operator import recognize_multi_operator
from .number import recognize_number
from .single_operator import recognize_single_operator

RECOGNIZERS = (
    recognize_keys_and_ids,
    recognize_multi_operator,
    recognize_number,
    recognize_single_operator,
)

BLANK = {' ', '\t', '\n', '\r'}

def tokenize(src):
    tokens = []

    size = len(src)

    pos = 0

    while pos < size:
        if src[pos] in BLANK:
            pos += 1
            continue

        for recognizer in RECOGNIZERS:
            result = recognizer(src, pos)
            if result is not None:
                token_type, lexeme, next_pos = result

                tokens.append((token_type, lexeme))

                pos = next_pos

                break

        else:  # 对应 for：全部识别器失败、未执行 break 时才运行。
            raise LexicalError(src, pos)
    return tokens

