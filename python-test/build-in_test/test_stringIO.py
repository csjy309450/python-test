import StringIO
import cStringIO

t_file = cStringIO.StringIO()
t_file.write('a test string\n')
t_file.write('a test line\n')

a = t_file.readline()
print repr(a)

realFile = file('./test.txt', 'w')
realFile.write(t_file.getvalue())