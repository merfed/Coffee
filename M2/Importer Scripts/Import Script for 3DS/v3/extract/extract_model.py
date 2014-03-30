from extract import *
from m2 import Model


def extract_model_textures(m, dir='', fullpath=False):
    for t in m.textures:
        extract(t.name, dir, fullpath)

def extract_model(wowname, dir='', fullpath=False):
    m = Model(wow_data.open(wowname))
    extract(wowname, dir, fullpath)
    extract_model_textures(m, dir, fullpath)


if __name__=='__main__':
    import random
    from wow_data import archives
    
    fn = random.choice(archives['model.MPQ'].namelist())
    
    extract_model(fn, fullpath=True)
    
