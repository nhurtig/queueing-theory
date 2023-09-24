import os

outfile = open('doc/all_headers.h', 'w')

for file in os.listdir('sim'):
    if '.h' in file:
        f = open(f'sim/{file}')
        outfile.writelines(f.readlines())
        f.close()

outfile.close()

os.system('hpp2plantuml -i doc/all_headers.h -o doc/plantuml.txt')
