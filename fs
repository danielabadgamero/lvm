/main.ll
LightLang v1.1
var:
a 97
b 104
src:
print a
print 10
set a 98
print a
print b
set a b
print a
halt

/main.exe
a 97
b 104
%%
1
a
4
4
0
4
10
3
a
98
1
a
4
4
0
1
b
4
4
0
1
b
5
3
a
0
1
a
4
4
0
0

