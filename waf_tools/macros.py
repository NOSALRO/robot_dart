import fnmatch,re
import os, shutil, sys

def make_dirlist(folder, extensions):
    matches = []
    for root, dirnames, filenames in os.walk(folder):
        for ext in extensions:
            for filename in fnmatch.filter(filenames, '*' + ext):
                matches.append(os.path.join(root, filename))
    return matches

def remove_leading_whitespace(lines):
    min_whites = -1
    for line in lines:
        n = len(line) - len(line.lstrip())
        if n < min_whites or min_whites < 0:
            min_whites = n

    new_lines = []
    for line in lines:
        new_lines.append(line[min_whites:])

    return new_lines

def grab(delimiter='@'):
    # cpp
    cpp = make_dirlist('src', ['.hpp', '.cpp'])
    variables = {}

    for fname in cpp:
        input_data = open(fname, 'r')
        line_number = 0
        q = []
        curr = None

        ret = []
        lines = []
        for line in input_data:
            lines.append(line)
            ll = line.strip()
            dels = [m.start() for m in re.finditer(delimiter, ll)]
            line_number = line_number + 1
            if len(dels) < 2 or len(dels) >= 2 and ll[:2] != '//':
                continue
            start = dels[0] + 1
            end = dels[-1]

            val = ll[start:end]
            if curr == None:
                q.append((val, line_number - 1))
                curr = val
            else:
                if 'END' == val[-3:]:
                    vv, num = q.pop()
                    ret.append((vv, num, line_number - 1))
                    if len(q) == 0:
                        curr = None
                    else:
                        curr = q[-1][0]
                else:
                    q.append((val, line_number - 1))
                    curr = val

        for (v, s,e) in ret:
            my_lines = remove_leading_whitespace(lines[s+1:e])
            variables[v] = ''.join(my_lines)
            variables[v] = '```cpp\n' + variables[v] + '```'

    return variables
