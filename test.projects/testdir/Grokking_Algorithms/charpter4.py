def sum(arr):
    if arr == []:
        return 0
    else:
        return arr[0]+sum(arr[1:])
array = [1,2,5,4,3]
print sum(array)

def max(list):
    if len(list) == 2:
        return list[0] if list[0] > list[1] else list[1]
    sub_max = max(list[1:])
    return list[0] if list[0] > sub_max else sub_max
print max(array)

def quicksort(arr):
    if len(arr) < 2:
        return arr
    pivot = arr[0]
    less = [i for i in arr[1:] if i < pivot ]
    greater = [i for i in arr[1:] if i > pivot]
    return quicksort(less)+[pivot]+quicksort(greater)
print quicksort(array)

def MergeSort(lists):
    if len(lists) <= 1:
        return lists
    num = int( len(lists)/2 )
    left = MergeSort(lists[:num])
    right = MergeSort(lists[num:])
    return Merge(left, right)
def Merge(left,right):
    r, l=0, 0
    result=[]
    while l<len(left) and r<len(right):
        if left[l] < right[r]:
            result.append(left[l])
            l += 1
        else:
            result.append(right[r])
            r += 1
    result += right[r:]
    result+= left[l:]
    return result
print MergeSort([1, 2, 3, 4, 5, 6, 7, 90, 21, 23, 45])
