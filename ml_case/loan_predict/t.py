#! /usr/bin/env python
# -*- coding: utf-8 -*-

class Node:
    def __init__(self, low, up):
        self.low = low
        self.up = up
    def __str__(self):
        return "%s\t%s" %(self.low, self.up)

    def __cmp__(self, node):
        if self.low > node.low:
            return 1
        if self.low < node.low:
            return -1
        return 0

    def __cmp__(self, val):
        if self.low > val:
            return 1
        if self.up <= val:
            return -1
        return 0

    def __eq__(self, val):
        if self.low <= val and self.up > val:
            return True
        return False


def bin_search(fea_val_vec, left, right, fea_value):
    offset = (right - left) / 2
    if offset == 0:
        if fea_value == fea_val_vec[left]:
            return left
        if fea_value == fea_val_vec[right]:
            return right
        return -1
    mid = left + offset
    if fea_value > fea_val_vec[mid]:
        left = mid
    elif fea_value < fea_val_vec[mid]:
        right = mid
    else:
        return mid

    return bin_search(fea_val_vec, left, right, fea_value)


if __name__ == "__main__":
    a = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    print bin_search(a, 0, 8, 8)
#    a = []
#    a.append(Node(3, 4))
#    a.append(Node(1, 2))
#    a.append(Node(8, 19))
#
#    a.sort()
#
#    print a[0]
#    print a[1]
#    print a[2]
#    if 3 > a[0]:
#        print 'llllllllll'
#    if 3 == a[1]:
#        print '11111111111'
#    if 3 in a:
#        print "00000000"
