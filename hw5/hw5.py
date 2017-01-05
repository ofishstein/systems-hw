# Oliver Fishstein
# Homework 5
# Python Excercises


# Part 1 - Coding Bat

# List 1
def sum3(nums):
    return sum(nums)

def rotate_left3(nums):
    return nums[1:] + [nums[0]]

def max_end3(nums):
    return [max(nums[0], nums[2]), max(nums[0], nums[2]), max(nums[0], nums[2])]

def make_ends(nums):
    return [nums[0], nums[-1]]

def has23(nums):
    return (2 in nums) or (3 in nums)

# List 2
def count_evens(nums):
    return len([x for x in nums if x % 2 == 0])

def sum13(nums):
    nums += [0]
    return sum(n for i, n in enumerate(nums) if n != 13 and nums[i-1] != 13)

def big_diff(nums):
    return max(nums) - min(nums)

def sum67(nums):
    while 6 in nums:
        del nums[nums.index(6) : nums.index(7,nums.index(6)) + 1]
    return sum(nums)

def centered_average(nums):
    return (sum(nums) - max(nums) - min(nums)) / (len(nums) - 2)

def has22(nums):
    return any(nums[i] == nums[i + 1] == 2 for i in range(len(nums) - 1))

# String 1
def extra_end(str):
    return 3 * str[-2:]

def without_end(str):
    return str[1:-1]

# String 2
def double_char(str):
    return ''.join([ c+c for c in str ])

def count_code(str):
    return sum(1 for i in range(len(str) - 3) if str[i:i + 2] == 'co' and str[i + 3] == 'e')

def count_hi(str):
    return str.count('hi')

def end_other(a, b):
    return a.lower().endswith(b.lower()) or b.lower().endswith(a.lower())

def cat_dog(str):
    return str.count("cat") == str.count("dog")

def xyz_there(str):
    return str.count("xyz") != str.count(".xyz")


# Part 2 - HW 2 Rewrite

# Program 1
def p1(str):
    return [str.count('\n'), len(str.split), len(str)]

# Program 2
def p2(str):
    return [str.count('0'), str.count('1'), str.count('2'), str.count('3'), str.count('4'), str.count('5'), str.count('6'), str.count('7'), str.count('8'), str.count('9'), sum(c.isspace() for c in str), sum(c.isalpha() for c in str)]
