#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys

class Node:
    def __init__(self, low, up, code):
        self.low = low
        self.up = up
        self.code = code

    ## for list sort
    def __cmp__(self, node):
        if self.low > node.low:
            return 1
        if self.low < node.low:
            return -1

        return 0

    def more_than(self, val):
        if self.low > val:
            return True
        return False

    def less_than(self, val):
        if self.up <= val:
            return True
        return False

    def eq(self, val):
        if self.low <= val and self.up > val:
            return True
        return False

    def __str__(self):
        return "%s\t%s\t%s" %(self.low, self.up, self.code)

class RangeCode:
    ## code file format:
    ## fea_type/index fea_low fea_high code
    ## and which is include fea_low and less than fea_high
    def __init__(self, file, sep='\t'):
        self.code_file = file
        ## fea_type -> [] feature_node in vector
        self.fea_type_dict = {}
        self.sep = sep
        self.load_file(self.code_file)

    def load_file(self, file):
        with open(file, 'r') as fd:
            for line in fd:
                items = line.split(self.sep)
                if len(items) != 4:
                    continue
                fea_type = items[0]
                if items[1].isdigit():
                    low = int(items[1])
                else:
                    low = ''

                if items[2].isdigit():
                    up = int(items[2])
                else:
                    up = ''

                code = items[3].strip()

                ## for null default code
                if low == up and low == '':
                    self.fea_type_dict["%s##"%(fea_type)] = code

                if fea_type in self.fea_type_dict:
                    fea_val_vec = self.fea_type_dict[fea_type]
                    fea_val_vec.append(Node(low, up, code))
                else:
                    self.fea_type_dict[fea_type] = [Node(low, up, code)]

        for fea_type, val_vec in self.fea_type_dict.items():
            if type(val_vec) == type([]):
                val_vec.sort()


    def get_code(self, fea_type, fea_value):
        if fea_type not in self.fea_type_dict:
            return None
        if fea_value == '':
            key = "%s##"%(fea_type)
            if key in self.fea_type_dict:
                return self.fea_type_dict[key]
            else:
                return 0
        fea_val_vec = self.fea_type_dict[fea_type]
        return self.bin_search(fea_val_vec, 0, len(fea_val_vec)-1, fea_value)

    def bin_search(self, fea_val_vec, left, right, fea_value):
        offset = (right - left) / 2
        if offset == 0:
            if fea_val_vec[left].eq(fea_value):
                return fea_val_vec[left].code
            if fea_val_vec[right].eq(fea_value):
                return fea_val_vec[right].code
            ## fail to find
            return None
        mid = left + offset
        if fea_val_vec[mid].less_than(fea_value):
            left = mid
        elif fea_val_vec[mid].more_than(fea_value):
            right = mid
        elif fea_val_vec[mid].eq(fea_value):
            return fea_val_vec[mid].code

        return self.bin_search(fea_val_vec, left, right, fea_value)
        

class DictCode:
    ## code file format:
    ## fea_type/index fea_value code
    def __init__(self, file, sep='\t'):
        self.code_file = file
        self.fea_val_dict = {}
        self.sep = sep
        self.load_file(self.code_file)

    def load_file(self, file):
        with open(file, 'r') as fd:
            for line in fd:
                items = line.split(self.sep)
                if len(items) != 3:
                    continue
                index = items[0]
                val = items[1]
                code = int(items[2].strip())

                key = "%s##%s" %(index, val)
                self.fea_val_dict[key] = code

    def get_code(self, fea_type, fea_value):
        key = "%s##%s" %(fea_type, fea_value.strip())
        if key in self.fea_val_dict:
            return self.fea_val_dict[key]
        return None

if __name__ == '__main__':
    fea_dict = sys.argv[1]
    col = sys.argv[2]
    ## 1:range replace
    ## 2:dict replace
    fea_t = int(sys.argv[3])
    if 1 == fea_t:
        rc = RangeCode(fea_dict)
    elif 2== fea_t:
        rc = DictCode(fea_dict)
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        ## no strip which will delete null beginning
        items = line.split('\t')
        if len(items) < int(col):
            continue
        if 1 == fea_t:
            if items[int(col)-1].replace(".", "").isdigit():
                code = rc.get_code(col, float(items[int(col)-1]))
            elif items[int(col)-1] == '':
                code = rc.get_code(col, '')
            else:
                code = items[int(col)-1].strip()
        elif 2 == fea_t:
            code = rc.get_code(col, items[int(col)-1])

        if None == code:
            print >> sys.stderr, "%s at %s not found" %(items[int(col)-1], col)
            code = items[int(col)-1]
        items[int(col)-1] = str(code)
        print '\t'.join(items).strip()

