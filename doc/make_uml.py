import os

outfile = open('doc/all_headers.h', 'w')
SRC_DIR = 'soap'

for file in os.listdir(SRC_DIR):
    if '.h' in file:
        f = open(f'{SRC_DIR}/{file}')
        outfile.writelines(f.readlines())
        f.close()

outfile.close()

os.system(f'hpp2plantuml -i doc/all_headers.h -o doc/plantuml_{SRC_DIR}.txt')
