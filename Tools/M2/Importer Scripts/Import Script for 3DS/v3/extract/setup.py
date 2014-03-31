# setup.py
from distutils.core import setup
import py2exe

setup(version = "0.1.0", description = "mpq extract", name = "extract", console=["extract.py"])
