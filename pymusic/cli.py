

def list_selector(l, msg="Enter index", value_getter=str, error_msg="Try again:"):
    for i in range(len(l)):
        print("#%i %s" % (i, value_getter(l[i])))
    print(msg)
    try:
        return l[int(input())]
    except ValueError:
        return list_selector(l, msg=error_msg, value_getter=value_getter, error_msg=error_msg)
