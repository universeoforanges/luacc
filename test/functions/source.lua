function add(x, y)
    return x + y
end

function print_to_console(txt)
    print(txt)
end

print_to_console("This was printed via an unnecessary function!")
print(add(1, 2))
print("The above was printed via an unnecessary add function!")